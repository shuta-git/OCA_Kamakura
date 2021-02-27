#include "shader.h"

// MRT出力専用
struct PS_OUTPUT_MRT
{
	float4	color0_ : SV_Target0;
	float4	color1_ : SV_Target1;
	float4	color2_ : SV_Target2;
	float4	color3_ : SV_Target3;
};

//---------------------------------------------------------------------------
// GBuffer生成
//---------------------------------------------------------------------------
PS_OUTPUT_MRT main(PS_INPUT input)
{
	float4	color = AlbedoTexture.Sample(LinearSampler, input.uv_);
	color *= input.color_;	// 頂点カラー

	float3	albedo = color.rgb;

	float3	N = normalize(input.normal_);	// 法線ベクトル N
	float1	AO = AoTexture.Sample(LinearSampler, input.uv_).r;
	float	roughness = RoughnessTexture.Sample(LinearSampler, input.uv_).r;
	float	metalness = MetalnessTexture.Sample(LinearSampler, input.uv_).r;

	//-------------------------------------------------------------
	// 法線マッピング
	//-------------------------------------------------------------
	{
		float3	normalTexture = NormalTexture.Sample(LinearSampler, input.uv_).rgb;
		normalTexture = normalize(normalTexture * 2.0 - 1.0);

		// 法線ベクトルの回転行列を作成
		float3 T = normalize(input.tangent_ );
		float3 B = normalize(input.binormal_);

		float3x3 TBN = { T, B, N };

		N = mul(normalTexture, TBN);
	}

	float3	L = normalize(float3(1,1,1));	// 光源の方向 L
	float3	V = normalize(cameraPosition_ - input.worldPosition_);	// 視線ベクトル V
	float3	H = normalize(L + V);			// ハーフベクトル H
	float3	R = reflect(-V, N);				// 反射ベクトル R

	float	NdotV = max( dot(N, V), 0.00001 );
	float	NdotL = max( dot(N, L), 0.00001 );

	// アルファテスト
	if (color.a < 0.5) {
		discard;
	}

	//-------------------------------------------------------------
	// MRT出力
	//          R        G        B        A
	//     +--------+--------+--------+--------+
	// RT0 |        Albedo.RGB        |EmsvMetal| R8G8B8A8_Unorm
	//     +--------+--------+--------+--------+
	// RT1 |  WorldNormal.xy | Rough  |   AO   | R8G8B8A8_Unorm
	//     +--------+--------+--------+--------+
	// RT2 |    velocity.xy  |  ----  |  ----  | R16G16_Float
	//     +--------+--------+--------+--------+
	//
	//     +--------+--------+--------+--------+
	//DEPTH|           fp32 depth              | D32_Float
	//     +--------+--------+--------+--------+
	//-------------------------------------------------------------
	PS_OUTPUT_MRT	output;

	output.color0_ = float4(albedo.rgb, metalness);
	output.color1_ = float4(Octahedron_Encode(N.xyz), roughness, AO);
	output.color2_ = 0;
	output.color3_ = float4(input.worldPosition_, 1.0);

	return output;
}
