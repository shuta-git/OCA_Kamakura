#include "shader.h"

//---------------------------------------------------------------------------
// �s�N�Z���V�F�[�_�[ (�e�N�X�`������)
//---------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{
	return Texture0.Sample(ClampSampler, input.uv_) * input.color_;
}
