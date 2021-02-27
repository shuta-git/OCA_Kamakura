#include "shader.h"

cbuffer CBFilter : register(b4)
{
	float	stride_;
	float	attenuation_;
	float2	dir_;			// グレアの方向
}

//---------------------------------------------------------------------------
// ピクセルシェーダー (テクスチャあり)
//---------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{
	uint	w;
	uint	h;
	uint mipLevels;
	Texture0.GetDimensions(0, w, h, mipLevels);

	float2	dir = dir_ / float2(w,h) * stride_;	// 1ピクセル分の移動量

	float4	color = 0;

	static const int SAMPLE_COUNT = 8;
	for( int i=0; i<SAMPLE_COUNT; ++i ) {
		float	a = pow(saturate(attenuation_), i);
		color += Texture0.Sample(ClampSampler, input.uv_ + dir * i * (1.0/SAMPLE_COUNT)) * a;
	}
	return color;
}
