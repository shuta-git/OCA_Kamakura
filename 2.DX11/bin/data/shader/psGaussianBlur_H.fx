#include "shader.h"
#include "blur.h"

//---------------------------------------------------------------------------
// ピクセルシェーダー (テクスチャあり)
//---------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{
	// 9x9 高速ガウシアンフィルタ
	uint	w;
	uint	h;
	uint mipLevels;
	Texture0.GetDimensions(0, w, h, mipLevels);

	return blur9_reference(Texture0, input.uv_, float2(w, h), float2(1,0));	// 水平ブラー
}
