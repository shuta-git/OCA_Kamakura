#include "shader.h"

//---------------------------------------------------------------------------
//	�v���~�e�B�u�p���_�V�F�[�_�[
//---------------------------------------------------------------------------
PS_INPUT main(VS_INPUT input)
{
	PS_INPUT	output = (PS_INPUT)0;	// ���_�V�F�[�_�[�o�͌��ʂ̓s�N�Z���V�F�[�_�[�Ɏg����

	//-------------------------------------------------------------
	// ���_�̍��W�ϊ�
	//-------------------------------------------------------------
	float4	position = input.position_;

	position = mul(matWorld_, position);
	position = mul(matView_, position);
	position = mul(matProj_, position);

	output.position_ = position;		// ���W�͓��e���xyzw���W�l (GPU�ɂ���Ē��_�V�F�[�_�[���s��Ɏ�����xyz��w�����s�����)
	output.color_    = input.color_;	// �p�X�X���[
	output.uv_       = input.uv_;		// UV�����H���Ȃ��ꍇ�͂��̂܂ܑ���ŃX���[������

	return output;
}
