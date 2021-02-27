#include "shader.h"

// MRT�o�͐�p
struct PS_OUTPUT_MRT
{
	float4	color0_ : SV_Target0;
	float4	color1_ : SV_Target1;
	float4	color2_ : SV_Target2;
	float4	color3_ : SV_Target3;
};

//---------------------------------------------------------------------------
// GBuffer����
//---------------------------------------------------------------------------
PS_OUTPUT_MRT main(PS_INPUT input)
{
	float4	color = AlbedoTexture.Sample(LinearSampler, input.uv_);
	color *= input.color_;	// ���_�J���[

	float3	albedo = color.rgb;

	float3	N = normalize(input.normal_);	// �@���x�N�g�� N
	float1	AO = AoTexture.Sample(LinearSampler, input.uv_).r;
	float	roughness = RoughnessTexture.Sample(LinearSampler, input.uv_).r;
	float	metalness = MetalnessTexture.Sample(LinearSampler, input.uv_).r;

	//-------------------------------------------------------------
	// �@���}�b�s���O
	//-------------------------------------------------------------
	{
		float3	normalTexture = NormalTexture.Sample(LinearSampler, input.uv_).rgb;
		normalTexture = normalize(normalTexture * 2.0 - 1.0);

		// �@���x�N�g���̉�]�s����쐬
		float3 T = normalize(input.tangent_ );
		float3 B = normalize(input.binormal_);

		float3x3 TBN = { T, B, N };

		N = mul(normalTexture, TBN);
	}

	float3	L = normalize(float3(1,1,1));	// �����̕��� L
	float3	V = normalize(cameraPosition_ - input.worldPosition_);	// �����x�N�g�� V
	float3	H = normalize(L + V);			// �n�[�t�x�N�g�� H
	float3	R = reflect(-V, N);				// ���˃x�N�g�� R

	float	NdotV = max( dot(N, V), 0.00001 );
	float	NdotL = max( dot(N, L), 0.00001 );

	// �A���t�@�e�X�g
	if (color.a < 0.5) {
		discard;
	}

	//-------------------------------------------------------------
	// MRT�o��
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
