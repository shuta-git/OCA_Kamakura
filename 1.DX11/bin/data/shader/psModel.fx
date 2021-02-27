#include "shader.h"

float	D_BlinnPhong(float roughness, float NdotH)
{
	// 正規化blinn-Phong鏡面反射モデル
	float	specularPower = exp2(10 * (1 - roughness) + 1);

	float	normalizeFactor = (specularPower + 2) / (2 * PI);
	return pow(saturate(NdotH), specularPower) * normalizeFactor;
}

float	D_GGX(float roughness, float NdotH)
{
	float	alpha = roughness * roughness +0.000001;	// 0除算対策

	float	denom = NdotH * (alpha *alpha - 1) + 1;

	float	result = alpha / denom;
	return result * result * (1.0/PI);
}

float G_Smith_Schlick_GGX(float roughness, float NdotV, float NdotL)
{
	float	alpha = roughness * roughness +0.000001;	// 0除算対策

//  float k = alpha * 0.5f;							// もとの式
    float k = (roughness+1) * (roughness+1) / 8;	// UE4版
	
    float GV = NdotV / (NdotV * (1 - k) + k);
    float GL = NdotL / (NdotL * (1 - k) + k);

    return GV * GL;
}

// Schlickのフレネル近似式 (Fresnel approximation)
float3 F_Schlick(float3 f0, float NdotV)
{
	return f0 + (1.0 - f0) * pow(1.0 - NdotV, 5.0);
}



float	shadowSample(float3 shadowmapUv)
{
	float	shadowDepth = ShadowTexture.Sample(LinearSampler, shadowmapUv.xy).r;

	float	shadow = 1.0;
	static const float	SHADOW_BIAS = 0.00005;
	if( shadowDepth + SHADOW_BIAS < shadowmapUv.z) {
		shadow = 0.0;		// 影
	}

	return shadow;
}

float rand(float2 st)
{
	return frac(sin(dot(st.xy, float2(12.9898,78.233))) * 43758.5453123);
}


float2 vogelDiskSample(int sampleIndex, int samplesCount, float phi)
{
  float goldenAngle = 2.4f;	// 黄金角

  float r = sqrt(sampleIndex + 0.5f) / sqrt(samplesCount);
  float theta = sampleIndex * goldenAngle + phi;

  float sine, cosine;
  sincos(theta, sine, cosine);
  
  return float2(r * cosine, r * sine);
}




//---------------------------------------------------------------------------
// モデル用
//---------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{
	//return float4(input.tangent_*10, 1);	// 接ベクトルデバッグ
	//	return float4(input.normal_,1);	// 法線デバッグ

	float3	lightColor = float3(1.0,1.0,1.0) * 50*0;

	float4	color = AlbedoTexture.Sample(LinearSampler, input.uv_);
	color.rgb = pow(saturate(color.rgb), 2.2);	// リニア空間に変換

	color *= input.color_;	// 頂点カラー

	float3	N = normalize(input.normal_);	// 法線ベクトル N



	float1	AO = AoTexture.Sample(LinearSampler, input.uv_).r;
	//return float4(AO.rrr, 1);	// AOデバッグ

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



	//-------------------------------------------------------------
	// シャドウ適用
	//-------------------------------------------------------------
	float	shadow = 0.0;
#if 0
	// BOXフィルター
	for( int y=-3; y<=+3; y++) {
		for( int x=-3; x<=+3; x++) {
	
			float2	uv = input.shadowmapUv_.xy;
			uv += float2(x, y) * float2(1.0/4096.0, 1.0/4096);

			shadow += shadowSample( float3(uv, input.shadowmapUv_.z) );
		}
	}
	shadow /= 7*7;
#else
	// Vogel Disk Sampling
	// 黄金角で螺旋形状でサンプリング。円形で均一にサンプリングされる。
	static const float scale = 4.0;
	static const int SAMPLE_COUNT = 16;

	float	phi = rand(input.position_.xy) * 2.0 * PI ;

	for( int i=0; i<SAMPLE_COUNT; i++) {
		float2	uv = input.shadowmapUv_.xy;
		uv += vogelDiskSample(i, SAMPLE_COUNT, phi) * float2(1.0/4096.0, 1.0/4096) * scale;
		shadow += shadowSample( float3(uv, input.shadowmapUv_.z) );
	}
	shadow *= 1.0/SAMPLE_COUNT;

	// 通常の陰影と合成
	float3	shadowLightDir = float3(matLightView_[0].z, matLightView_[1].z, matLightView_[2].z) * -1.0;
	float	lambert = saturate( dot(N, shadowLightDir) );

	shadow = min(shadow, lambert);
#endif
	
//	return float4(shadow.xxx * 2, 1);

	//-------------------------------------------------------------
	// Image Based Lighting
	//-------------------------------------------------------------
	float	roughness = RoughnessTexture.Sample(LinearSampler, input.uv_).r;
	float	metalness = MetalnessTexture.Sample(LinearSampler, input.uv_).r;

	float3	specularColor = lerp( float3(0.04, 0.04, 0.04), color.rgb, metalness);

	float3	iblSpecular = iblSpecularTexture.SampleLevel(LinearSampler, R, roughness * 8).rgb;
	float3	iblDiffuse  = iblDiffuseTexture.Sample(LinearSampler, R).rgb;

	// Environment BRDF (各方向によってフレネル値が異なるための補正式)
	// SIGGRAPH 2015 "Optimizing PBR" より
	float3	environmentBRDF = 1-max(roughness, NdotV);
	environmentBRDF = environmentBRDF * environmentBRDF * environmentBRDF + specularColor;

	iblSpecular *= environmentBRDF;
	iblDiffuse  *= color.rgb      ;
	
	// 正規化Lambert拡散反射モデル
	static const float	PI = 3.141592;
	float	diffuse = saturate(NdotL) * (1.0 / PI);

	//----------------------------------------------------------
	// Cook Torrance モデル
	//----------------------------------------------------------
#if 0
	float   D = D_GGX(roughness, dot(N,H));
	float   G = G_Smith_Schlick_GGX(roughness, NdotV, NdotL);
	float3	F = F_Schlick(specularColor, NdotV);

	float3	specular =
			          (D * G * F)
			/ //------------------------
			      (4 * NdotV * NdotL);
#else
	float	r4 = roughness * roughness * roughness * roughness + 0.000001;
	float	NdotH = dot(N, H);
	float	LdotH = dot(L, H);
	float	LdotH2 = LdotH * LdotH + 0.000001;

	float	denom = NdotH * NdotH * (r4 - 1.0) + 1.0;

	float3	specular =				r4
		/ //------------------------------------------------------
			(4.0 * PI * denom * denom * LdotH2 * (roughness + 0.5));

	specular *= specularColor;
#endif
	specular *= NdotL;

	//-------------------------------------------------------------
	// 最終合成	
	//-------------------------------------------------------------
	float3	diffuseTerm = lightColor * color.rgb * diffuse;		// ディフューズ項
	float3	specularTerm = lightColor * specular;

//	diffuseTerm  *= shadow;
//	specularTerm *= shadow;

	float	iblScale = 0.5;
	diffuseTerm  += iblDiffuse  * iblScale;
	specularTerm += iblSpecular * iblScale;

	// リムライト
	// specularTerm += pow( saturate(1- NdotV), 2 )  * float3(0,1,0) * 30;
	// AO適用
	diffuseTerm  *= AO;														// AmbientOcclusion
	specularTerm *= saturate( (NdotV + AO) * (NdotV + AO) - 1.0f + AO );	// SpecularOcclusion近似 (from Tri-ace)

	// 最終カラー
	color.rgb = lerp(diffuseTerm, specularTerm, metalness + 0.04);

	// sRGBへ変換
	//color.rgb = pow(saturate(color.rgb), 1.0/2.2);

	// アルファテスト
	if (color.a < 0.5) {
		discard;
	}

	return float4(color.rgb, 1);
}
