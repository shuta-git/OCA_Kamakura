#include "shader.h"

//---------------------------------------------------------------------------
// ピクセルシェーダー (テクスチャあり)
//---------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{
	return Texture0.Sample(LinearSampler, input.uv_) * input.color_;
}
