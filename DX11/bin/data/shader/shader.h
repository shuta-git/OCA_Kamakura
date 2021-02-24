//===========================================================================
//!	@file	shader.h
//!	@brief	HLSL�w�b�_�[
//===========================================================================

static const float PI =3.14159265359f;

//---------------------------------------------------------------------------
//	�e�N�X�`��
//---------------------------------------------------------------------------
Texture2D		Texture0           : register(t0);
Texture2D		Texture1           : register(t1);
Texture2D		Texture2           : register(t2);
Texture2D		Texture3           : register(t3);

SamplerState	LinearSampler : register(s0);
SamplerState	ClampSampler : register(s1);


Texture2D		AlbedoTexture      : register(t4);
Texture2D		NormalTexture      : register(t5);
Texture2D		RoughnessTexture   : register(t6);
Texture2D		MetalnessTexture   : register(t7);
Texture2D		AoTexture          : register(t8);

Texture2D		Gbuffer0           : register(t5);
Texture2D		Gbuffer1           : register(t6);
Texture2D		Gbuffer2           : register(t7);
Texture2D		Gbuffer3           : register(t8);
Texture2D		DepthTexture       : register(t9);



Texture2D		ShadowTexture      : register(t10);

TextureCube		cubeEnvironment    : register(t16);
TextureCube		iblDiffuseTexture  : register(t17);
TextureCube		iblSpecularTexture : register(t18);

//---------------------------------------------------------------------------
//	�萔�o�b�t�@
//---------------------------------------------------------------------------

//---- �V�[���P�ʂōX�V�����萔�o�b�t�@
cbuffer cbCamera : register(b0)
{
	matrix	matView_;			// �r���[�s��
	matrix	matProj_;			// ���e�s��

	matrix	matLightView_;		// �����p�r���[�s��
	matrix	matLightProj_;		// �����p���e�s��

	float3	cameraPosition_;	// �J�����̃��[���h���W
};

//---- ���f���P�ʂōX�V�����萔�o�b�t�@
cbuffer cbWorld : register(b1)
{
	matrix	matWorld_;	// ���[���h�s��
};

//--------------------------------------------------------------
//	�V�F�[�_�[�ւ̓��͍\����
//--------------------------------------------------------------

//---- ���_�V�F�[�_�[
struct VS_INPUT {
	float4	position_ : POSITION;		// ���W�ϊ��O�̈ʒu���W
										// �i���̓��C�A�E�g����float3��ݒ肵����ԂŃV�F�[�_�[����
										//   float4�Ƃ��Ďg����W������1.0f�������œ��͂����j
	float3	normal_   : NORMAL;			// �@���x�N�g��
	float3	tangent_  : TANGENT;		// �ڃx�N�g��
	float3	binormal_ : BINORMAL;		// �]�@���x�N�g��
	float4	color_    : COLOR;
	float2	uv_       : TEXCOORD0;		// �e�N�X�`��UV���W
};

//---- �s�N�Z���V�F�[�_�[
struct PS_INPUT {
	float4	position_      : SV_Position;	// ���_�V�F�[�_�[����̏o�͂�SV_Position�ɏo�͂��邱��
	float4	color_         : COLOR;
	float3	normal_        : NORMAL;		// �@��
	float3	tangent_       : TANGENT;		// �ڃx�N�g��
	float3	binormal_      : BINORMAL;      // �]�@���x�N�g��
	float2	uv_            : TEXCOORD0;		// �e�N�X�`��UV���W
	float3	worldPosition_ : WorldPos;		// ���[���h���W 
	float4	shadowmapUv_   : ShadowmapUv;	// �V���h�E�}�b�v��UV�l
};




//===========================================================================
// Octahedron Normal Vector Encoding 
// https://knarkowicz.wordpress.com/2014/04/16/octahedron-normal-vector-encoding/
//===========================================================================
float2 OctWrap( float2 v )
{
    return ( 1.0 - abs( v.yx ) ) * ( v.xy >= 0.0 ? 1.0 : -1.0 );
}

float2 Octahedron_Encode( float3 n )
{
    n /= ( abs( n.x ) + abs( n.y ) + abs( n.z ) );
    n.xy = n.z >= 0.0 ? n.xy : OctWrap( n.xy );
    n.xy = n.xy * 0.5 + 0.5;
    return n.xy;
}

float3 Octahedron_Decode( float2 encN )
{
    encN = encN * 2.0 - 1.0;

    float3 n;
    n.z = 1.0 - abs( encN.x ) - abs( encN.y );
    n.xy = n.z >= 0.0 ? encN.xy : OctWrap( encN.xy );
    n = normalize( n );
    return n;
}




