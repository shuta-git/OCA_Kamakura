#include "shader.h"
#include "blur.h"

//---------------------------------------------------------------------------
// �s�N�Z���V�F�[�_�[ (�e�N�X�`������)
//---------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{
	uint	w;
	uint	h;
	uint mipLevels;
	Texture0.GetDimensions(0, w, h, mipLevels);

	// 9x9 �����K�E�V�A���t�B���^
	return blur9_reference(Texture0, input.uv_, float2(w, h), float2(0,1));	// �����u���[
}
