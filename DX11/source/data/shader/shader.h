//===========================================================================
//!	@file	shader.h
//!	@brief	HLSLヘッダー
//===========================================================================

static const float PI =3.14159265359f;

//---------------------------------------------------------------------------
//	テクスチャ
//---------------------------------------------------------------------------
Texture2D		Texture0           : register(t0);
Texture2D		Texture1           : register(t1);
Texture2D		Texture2           : register(t2);
Texture2D		Texture3           : register(t3);

SamplerState	LinearSampler : register(s0);
SamplerState	ClampSampler : register(s1);


Texture2D		AlbedoTexture      : register(t4);
Texture2D		NormalTexture      : register(t5);
Texture2D		RoughnessTexture   : register(t6);
Texture2D		MetalnessTexture   : register(t7);
Texture2D		AoTexture          : register(t8);

Texture2D		Gbuffer0           : register(t5);
Texture2D		Gbuffer1           : register(t6);
Texture2D		Gbuffer2           : register(t7);
Texture2D		Gbuffer3           : register(t8);
Texture2D		DepthTexture       : register(t9);



Texture2D		ShadowTexture      : register(t10);

TextureCube		cubeEnvironment    : register(t16);
TextureCube		iblDiffuseTexture  : register(t17);
TextureCube		iblSpecularTexture : register(t18);

//---------------------------------------------------------------------------
//	定数バッファ
//---------------------------------------------------------------------------

//---- シーン単位で更新される定数バッファ
cbuffer cbCamera : register(b0)
{
	matrix	matView_;			// ビュー行列
	matrix	matProj_;			// 投影行列

	matrix	matLightView_;		// 光源用ビュー行列
	matrix	matLightProj_;		// 光源用投影行列

	float3	cameraPosition_;	// カメラのワールド座標
};

//---- モデル単位で更新される定数バッファ
cbuffer cbWorld : register(b1)
{
	matrix	matWorld_;	// ワールド行列
};

//--------------------------------------------------------------
//	シェーダーへの入力構造体
//--------------------------------------------------------------

//---- 頂点シェーダー
struct VS_INPUT {
	float4	position_ : POSITION;		// 座標変換前の位置座標
										// （入力レイアウト側でfloat3を設定した状態でシェーダー内で
										//   float4として使うとW成分に1.0fが自動で入力される）
	float3	normal_   : NORMAL;			// 法線ベクトル
	float3	tangent_  : TANGENT;		// 接ベクトル
	float3	binormal_ : BINORMAL;		// 従法線ベクトル
	float4	color_    : COLOR;
	float2	uv_       : TEXCOORD0;		// テクスチャUV座標
};

//---- ピクセルシェーダー
struct PS_INPUT {
	float4	position_      : SV_Position;	// 頂点シェーダーからの出力はSV_Positionに出力すること
	float4	color_         : COLOR;
	float3	normal_        : NORMAL;		// 法線
	float3	tangent_       : TANGENT;		// 接ベクトル
	float3	binormal_      : BINORMAL;      // 従法線ベクトル
	float2	uv_            : TEXCOORD0;		// テクスチャUV座標
	float3	worldPosition_ : WorldPos;		// ワールド座標 
	float4	shadowmapUv_   : ShadowmapUv;	// シャドウマップのUV値
};




//===========================================================================
// Octahedron Normal Vector Encoding 
// https://knarkowicz.wordpress.com/2014/04/16/octahedron-normal-vector-encoding/
//===========================================================================
float2 OctWrap( float2 v )
{
    return ( 1.0 - abs( v.yx ) ) * ( v.xy >= 0.0 ? 1.0 : -1.0 );
}

float2 Octahedron_Encode( float3 n )
{
    n /= ( abs( n.x ) + abs( n.y ) + abs( n.z ) );
    n.xy = n.z >= 0.0 ? n.xy : OctWrap( n.xy );
    n.xy = n.xy * 0.5 + 0.5;
    return n.xy;
}

float3 Octahedron_Decode( float2 encN )
{
    encN = encN * 2.0 - 1.0;

    float3 n;
    n.z = 1.0 - abs( encN.x ) - abs( encN.y );
    n.xy = n.z >= 0.0 ? encN.xy : OctWrap( encN.xy );
    n = normalize( n );
    return n;
}




