//---------------------------------------------------------------------------
//!	@file	Shader.fx
//!	@brief	�V�F�[�_�[
//---------------------------------------------------------------------------

Texture2D<float4> tex:register(t0);//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> sph:register(t1);//1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��(��Z)
Texture2D<float4> spa:register(t2);//2�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��(���Z)
Texture2D<float4> toon:register(t3);//3�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��(�g�D�[��)

SamplerState smp:register(s0);//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v��
SamplerState smpToon:register(s1);//1�ԃX���b�g�ɐݒ肳�ꂽ�T���v��

//�萔�o�b�t�@0
struct SceneConstantBuffer{
	matrix world_;//���[���h�ϊ��s��
	matrix view_;
	matrix proj_;//�r���[�v���W�F�N�V�����s��
	float3 eye_;
};

ConstantBuffer<SceneConstantBuffer> scene : register(b0);

struct TransformConstantBuffer {
	matrix world_;//���[���h�ϊ��s��
	matrix bones_[256];
};

ConstantBuffer<TransformConstantBuffer> transform : register(b1);

//�萔�o�b�t�@1
//�}�e���A���p
struct  MaterialConstantBuffer {
	float4 diffuse_;//�f�B�t���[�Y�F
	float4 specular_;//�X�y�L����
	float3 ambient_;//�A���r�G���g
};

ConstantBuffer<MaterialConstantBuffer> material : register(b2);

// ���_�V�F�[�_�[����
struct VS_INPUT
{
	float4 pos_		    : POSITION;	// �|�W�V����
	float4 normal_		: NORMAL;	// �@��
	float2 uv_		    : TEXCOORD;	// UV
	min16uint2 boneno_  : BONENO;	// �{�[��
	min16uint weight_	: WEIGHT;
};

//�s�N�Z���V�F�[�_�[����
struct PS_INPUT {
	float4 svpos_   : SV_POSITION; //�V�X�e���p���_���W
	float4 pos_		: POSITION;	   //�V�X�e���p���_���W
	float4 normal_  : NORMAL0;	   //�@���x�N�g��
	float4 vnormal_ : NORMAL1;	   //�@���x�N�g��
	float2 uv_	    : TEXCOORD;	   //UV�l
	float3 ray_	    : VECTOR;	   //�x�N�g��
};

// ���_�V�F�[�_�[
PS_INPUT VS(VS_INPUT input) {
	
	// �s�N�Z���V�F�[�_�[�֓n���l
	PS_INPUT output;

	float w = (float)input.weight_ / 100.0f;
	// �{�[���s��
	matrix bm = transform.bones_[input.boneno_[0]] * w + transform.bones_[input.boneno_[1]] * (1.0f - w);

	input.pos_ = mul(bm,input.pos_);
	input.pos_ = mul(transform.world_,input.pos_);
	// ���V�F�[�_�ł͗�D��Ȃ̂Œ���
	output.svpos_ = mul(mul(scene.proj_, scene.view_),input.pos_);
	output.pos_ = mul(scene.view_, input.pos_);
	input.normal_.w = 0;//�����d�v(���s�ړ������𖳌��ɂ���)
	output.normal_ = mul(transform.world_,input.normal_);//�@���ɂ����[���h�ϊ����s��
	//output.normal_ = mul(transform.world_, mul(bm, input.normal_));//�@���ɂ����[���h�ϊ����s��
	output.vnormal_ = mul(scene.view_, output.normal_);
	output.uv_ = input.uv_;//UV
	output.ray_ = normalize(input.pos_.xyz - scene.eye_);	// �����x�N�g��
	//output.ray_ = normalize(output.pos_.xyz - mul(scene.view_, scene.eye_));	// �����x�N�g��

	return output;
}

// �s�N�Z���V�F�[�_�[
float4 PS(PS_INPUT input) : SV_TARGET{

	// ���̌������x�N�g���i���s�����j
	float3 light = normalize(float3(1,-1,1));
	float3 lightColor = float3(1, 1, 1);//���C�g�̃J���[(1,1,1�Ő^����)

	// �f�B�t���[�Y�v�Z
	float diffuseB = saturate(dot(-light, input.normal_.xyz));
	float4 toonDif = toon.Sample(smpToon, float2(0, 1.0 - diffuseB));
	
	// ���̔��˃x�N�g��
	float3 refLight = normalize(reflect(light, input.normal_.xyz));
	float specularB = pow(saturate(dot(refLight, - input.ray_)), material.specular_.a);

	// �X�t�B�A�}�b�v�p UV
	float2 sphereMapUV = input.vnormal_.xy;
	sphereMapUV = (sphereMapUV + float2(1, -1)) * float2(0.5, -0.5);


	float4 ambCol = float4(material.ambient_ * 0.6, 1);
	// �e�N�X�`���J���[
	float4 texColor = tex.Sample(smp, input.uv_);

	return saturate((
		toonDif // �P�x�i�g�D�[���j
		* material.diffuse_ + ambCol * 0.5) // �f�B�t���[�U�[�F
		* texColor// �e�N�X�`���J���[
		* sph.Sample(smp, sphereMapUV)// �X�t�B�A�}�b�v(��Z)
		+ spa.Sample(smp, sphereMapUV)// �X�t�B�A�}�b�v(���Z)
		+ float4(specularB * material.specular_.rgb, 1));// �X�y�L�����[
}


