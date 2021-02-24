#include "shader.h"

float	D_BlinnPhong(float roughness, float NdotH)
{
	// ���K��blinn-Phong���ʔ��˃��f��
	float	specularPower = exp2(10 * (1 - roughness) + 1);

	float	normalizeFactor = (specularPower + 2) / (2 * PI);
	return pow(saturate(NdotH), specularPower) * normalizeFactor;
}

float	D_GGX(float roughness, float NdotH)
{
	float	alpha = roughness * roughness +0.000001;	// 0���Z�΍�

	float	denom = NdotH * (alpha *alpha - 1) + 1;

	float	result = alpha / denom;
	return result * result * (1.0/PI);
}

float G_Smith_Schlick_GGX(float roughness, float NdotV, float NdotL)
{
	float	alpha = roughness * roughness +0.000001;	// 0���Z�΍�

//  float k = alpha * 0.5f;							// ���Ƃ̎�
    float k = (roughness+1) * (roughness+1) / 8;	// UE4��
	
    float GV = NdotV / (NdotV * (1 - k) + k);
    float GL = NdotL / (NdotL * (1 - k) + k);

    return GV * GL;
}

// Schlick�̃t���l���ߎ��� (Fresnel approximation)
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
		shadow = 0.0;		// �e
	}

	return shadow;
}

float rand(float2 st)
{
	return frac(sin(dot(st.xy, float2(12.9898,78.233))) * 43758.5453123);
}


float2 vogelDiskSample(int sampleIndex, int samplesCount, float phi)
{
  float goldenAngle = 2.4f;	// �����p

  float r = sqrt(sampleIndex + 0.5f) / sqrt(samplesCount);
  float theta = sampleIndex * goldenAngle + phi;

  float sine, cosine;
  sincos(theta, sine, cosine);
  
  return float2(r * cosine, r * sine);
}




//---------------------------------------------------------------------------
// ���f���p
//---------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{
	//return float4(input.tangent_*10, 1);	// �ڃx�N�g���f�o�b�O
	//	return float4(input.normal_,1);	// �@���f�o�b�O

	float3	lightColor = float3(1.0,1.0,1.0) * 50*0;

	float4	color = AlbedoTexture.Sample(LinearSampler, input.uv_);
	color.rgb = pow(saturate(color.rgb), 2.2);	// ���j�A��Ԃɕϊ�

	color *= input.color_;	// ���_�J���[

	float3	N = normalize(input.normal_);	// �@���x�N�g�� N



	float1	AO = AoTexture.Sample(LinearSampler, input.uv_).r;
	//return float4(AO.rrr, 1);	// AO�f�o�b�O

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



	//-------------------------------------------------------------
	// �V���h�E�K�p
	//-------------------------------------------------------------
	float	shadow = 0.0;
#if 0
	// BOX�t�B���^�[
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
	// �����p�ŗ����`��ŃT���v�����O�B�~�`�ŋψ�ɃT���v�����O�����B
	static const float scale = 4.0;
	static const int SAMPLE_COUNT = 16;

	float	phi = rand(input.position_.xy) * 2.0 * PI ;

	for( int i=0; i<SAMPLE_COUNT; i++) {
		float2	uv = input.shadowmapUv_.xy;
		uv += vogelDiskSample(i, SAMPLE_COUNT, phi) * float2(1.0/4096.0, 1.0/4096) * scale;
		shadow += shadowSample( float3(uv, input.shadowmapUv_.z) );
	}
	shadow *= 1.0/SAMPLE_COUNT;

	// �ʏ�̉A�e�ƍ���
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

	// Environment BRDF (�e�����ɂ���ăt���l���l���قȂ邽�߂̕␳��)
	// SIGGRAPH 2015 "Optimizing PBR" ���
	float3	environmentBRDF = 1-max(roughness, NdotV);
	environmentBRDF = environmentBRDF * environmentBRDF * environmentBRDF + specularColor;

	iblSpecular *= environmentBRDF;
	iblDiffuse  *= color.rgb      ;
	
	// ���K��Lambert�g�U���˃��f��
	static const float	PI = 3.141592;
	float	diffuse = saturate(NdotL) * (1.0 / PI);

	//----------------------------------------------------------
	// Cook Torrance ���f��
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
	// �ŏI����	
	//-------------------------------------------------------------
	float3	diffuseTerm = lightColor * color.rgb * diffuse;		// �f�B�t���[�Y��
	float3	specularTerm = lightColor * specular;

//	diffuseTerm  *= shadow;
//	specularTerm *= shadow;

	float	iblScale = 0.5;
	diffuseTerm  += iblDiffuse  * iblScale;
	specularTerm += iblSpecular * iblScale;

	// �������C�g
	// specularTerm += pow( saturate(1- NdotV), 2 )  * float3(0,1,0) * 30;
	// AO�K�p
	diffuseTerm  *= AO;														// AmbientOcclusion
	specularTerm *= saturate( (NdotV + AO) * (NdotV + AO) - 1.0f + AO );	// SpecularOcclusion�ߎ� (from Tri-ace)

	// �ŏI�J���[
	color.rgb = lerp(diffuseTerm, specularTerm, metalness + 0.04);

	// sRGB�֕ϊ�
	//color.rgb = pow(saturate(color.rgb), 1.0/2.2);

	// �A���t�@�e�X�g
	if (color.a < 0.5) {
		discard;
	}

	return float4(color.rgb, 1);
}
