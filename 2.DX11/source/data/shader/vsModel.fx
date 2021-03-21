#include "shader.h"

//---------------------------------------------------------------------------
//	���f���p���_�V�F�[�_�[
//---------------------------------------------------------------------------
PS_INPUT main(VS_INPUT input)
{
	PS_INPUT	output = (PS_INPUT)0;	// ���_�V�F�[�_�[�o�͌��ʂ̓s�N�Z���V�F�[�_�[�Ɏg����

	//-------------------------------------------------------------
	// ���_�̍��W�ϊ�
	//-------------------------------------------------------------
	float4	position = input.position_;

	position = mul(matWorld_, position);

	float3	worldPosition = position.xyz;	// ���[���h���W

	position = mul(matView_, position);
	position = mul(matProj_, position);

	// �V���h�E�p���W�ϊ�
	float4	shadowPosition = float4(worldPosition, 1.0);
	shadowPosition = mul(matLightView_, shadowPosition);
	shadowPosition = mul(matLightProj_, shadowPosition);

	// �X�N���[�����W��UV���W�ɕϊ�
	shadowPosition.xy = shadowPosition.xy * float2(0.5, -0.5) - 0.5;
#if 0
	// (1)���W�n�̏㉺���]
	shadowPosition.y *= -1.0;
	// (2)���_�𒆉����獶��Ɉړ�
	shadowPosition.x -= 1.0;
	shadowPosition.y -= 1.0;
	// (3)�X�P�[����UV���W�n�ɍ��킹�� (0.5�{)
	shadowPosition.x *= 0.5;
	shadowPosition.y *= 0.5;
#endif
	//-------------------------------------------------------------
	// �o��
	//-------------------------------------------------------------
	output.position_      = position;		// ���W�͓��e���xyzw���W�l (GPU�ɂ���Ē��_�V�F�[�_�[���s��Ɏ�����xyz��w�����s�����)
	output.color_         = input.color_;	// �J���[
	output.normal_        = mul(matWorld_, float4(input.normal_  , 0)).xyz;	// �@��
	output.tangent_       = mul(matWorld_, float4(input.tangent_ , 0)).xyz;	// �ڃx�N�g��
	output.binormal_      = mul(matWorld_, float4(input.binormal_, 0)).xyz;	// �]�@���x�N�g��
	output.uv_            = input.uv_;		// UV�����H���Ȃ��ꍇ�͂��̂܂ܑ���ŃX���[������
	output.worldPosition_ = worldPosition;
	output.shadowmapUv_   = shadowPosition;

	return output;
}
