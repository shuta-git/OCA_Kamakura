//---------------------------------------------------------------------------
//!	@file	Shader.fx
//!	@brief	シェーダー
//---------------------------------------------------------------------------

Texture2D<float4> tex:register(t0);//0番スロットに設定されたテクスチャ
Texture2D<float4> sph:register(t1);//1番スロットに設定されたテクスチャ(乗算)
Texture2D<float4> spa:register(t2);//2番スロットに設定されたテクスチャ(加算)
Texture2D<float4> toon:register(t3);//3番スロットに設定されたテクスチャ(トゥーン)

SamplerState smp:register(s0);//0番スロットに設定されたサンプラ
SamplerState smpToon:register(s1);//1番スロットに設定されたサンプラ

//定数バッファ0
struct SceneConstantBuffer{
	matrix world_;//ワールド変換行列
	matrix view_;
	matrix proj_;//ビュープロジェクション行列
	float3 eye_;
};

ConstantBuffer<SceneConstantBuffer> scene : register(b0);

struct TransformConstantBuffer {
	matrix world_;//ワールド変換行列
	matrix bones_[256];
};

ConstantBuffer<TransformConstantBuffer> transform : register(b1);

//定数バッファ1
//マテリアル用
struct  MaterialConstantBuffer {
	float4 diffuse_;//ディフューズ色
	float4 specular_;//スペキュラ
	float3 ambient_;//アンビエント
};

ConstantBuffer<MaterialConstantBuffer> material : register(b2);

// 頂点シェーダー入力
struct VS_INPUT
{
	float4 pos_		    : POSITION;	// ポジション
	float4 normal_		: NORMAL;	// 法線
	float2 uv_		    : TEXCOORD;	// UV
	min16uint2 boneno_  : BONENO;	// ボーン
	min16uint weight_	: WEIGHT;
};

//ピクセルシェーダー入力
struct PS_INPUT {
	float4 svpos_   : SV_POSITION; //システム用頂点座標
	float4 pos_		: POSITION;	   //システム用頂点座標
	float4 normal_  : NORMAL0;	   //法線ベクトル
	float4 vnormal_ : NORMAL1;	   //法線ベクトル
	float2 uv_	    : TEXCOORD;	   //UV値
	float3 ray_	    : VECTOR;	   //ベクトル
};

// 頂点シェーダー
PS_INPUT VS(VS_INPUT input) {
	
	// ピクセルシェーダーへ渡す値
	PS_INPUT output;

	float w = (float)input.weight_ / 100.0f;
	// ボーン行列
	matrix bm = transform.bones_[input.boneno_[0]] * w + transform.bones_[input.boneno_[1]] * (1.0f - w);

	input.pos_ = mul(bm,input.pos_);
	input.pos_ = mul(transform.world_,input.pos_);
	// ※シェーダでは列優先なので注意
	output.svpos_ = mul(mul(scene.proj_, scene.view_),input.pos_);
	output.pos_ = mul(scene.view_, input.pos_);
	input.normal_.w = 0;//ここ重要(平行移動成分を無効にする)
	output.normal_ = mul(transform.world_,input.normal_);//法線にもワールド変換を行う
	//output.normal_ = mul(transform.world_, mul(bm, input.normal_));//法線にもワールド変換を行う
	output.vnormal_ = mul(scene.view_, output.normal_);
	output.uv_ = input.uv_;//UV
	output.ray_ = normalize(input.pos_.xyz - scene.eye_);	// 視線ベクトル
	//output.ray_ = normalize(output.pos_.xyz - mul(scene.view_, scene.eye_));	// 視線ベクトル

	return output;
}

// ピクセルシェーダー
float4 PS(PS_INPUT input) : SV_TARGET{

	// 光の向かうベクトル（平行光線）
	float3 light = normalize(float3(1,-1,1));
	float3 lightColor = float3(1, 1, 1);//ライトのカラー(1,1,1で真っ白)

	// ディフューズ計算
	float diffuseB = saturate(dot(-light, input.normal_.xyz));
	float4 toonDif = toon.Sample(smpToon, float2(0, 1.0 - diffuseB));
	
	// 光の反射ベクトル
	float3 refLight = normalize(reflect(light, input.normal_.xyz));
	float specularB = pow(saturate(dot(refLight, - input.ray_)), material.specular_.a);

	// スフィアマップ用 UV
	float2 sphereMapUV = input.vnormal_.xy;
	sphereMapUV = (sphereMapUV + float2(1, -1)) * float2(0.5, -0.5);


	float4 ambCol = float4(material.ambient_ * 0.6, 1);
	// テクスチャカラー
	float4 texColor = tex.Sample(smp, input.uv_);

	return saturate((
		toonDif // 輝度（トゥーン）
		* material.diffuse_ + ambCol * 0.5) // ディフューザー色
		* texColor// テクスチャカラー
		* sph.Sample(smp, sphereMapUV)// スフィアマップ(乗算)
		+ spa.Sample(smp, sphereMapUV)// スフィアマップ(加算)
		+ float4(specularB * material.specular_.rgb, 1));// スペキュラー
}


