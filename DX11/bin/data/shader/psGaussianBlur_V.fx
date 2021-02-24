#include "shader.h"
#include "blur.h"

//---------------------------------------------------------------------------
// ピクセルシェーダー (テクスチャあり)
//---------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{
	uint	w;
	uint	h;
	uint mipLevels;
	Texture0.GetDimensions(0, w, h, mipLevels);

	// 9x9 高速ガウシアンフィルタ
	return blur9_reference(Texture0, input.uv_, float2(w, h), float2(0,1));	// 垂直ブラー
}
