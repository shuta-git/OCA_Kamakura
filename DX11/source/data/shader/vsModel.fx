#include "shader.h"

//---------------------------------------------------------------------------
//	モデル用頂点シェーダー
//---------------------------------------------------------------------------
PS_INPUT main(VS_INPUT input)
{
	PS_INPUT	output = (PS_INPUT)0;	// 頂点シェーダー出力結果はピクセルシェーダーに使われる

	//-------------------------------------------------------------
	// 頂点の座標変換
	//-------------------------------------------------------------
	float4	position = input.position_;

	position = mul(matWorld_, position);

	float3	worldPosition = position.xyz;	// ワールド座標

	position = mul(matView_, position);
	position = mul(matProj_, position);

	// シャドウ用座標変換
	float4	shadowPosition = float4(worldPosition, 1.0);
	shadowPosition = mul(matLightView_, shadowPosition);
	shadowPosition = mul(matLightProj_, shadowPosition);

	// スクリーン座標をUV座標に変換
	shadowPosition.xy = shadowPosition.xy * float2(0.5, -0.5) - 0.5;
#if 0
	// (1)座標系の上下反転
	shadowPosition.y *= -1.0;
	// (2)原点を中央から左上に移動
	shadowPosition.x -= 1.0;
	shadowPosition.y -= 1.0;
	// (3)スケールをUV座標系に合わせる (0.5倍)
	shadowPosition.x *= 0.5;
	shadowPosition.y *= 0.5;
#endif
	//-------------------------------------------------------------
	// 出力
	//-------------------------------------------------------------
	output.position_      = position;		// 座標は投影後のxyzw座標値 (GPUによって頂点シェーダー実行後に自動でxyz÷wが実行される)
	output.color_         = input.color_;	// カラー
	output.normal_        = mul(matWorld_, float4(input.normal_  , 0)).xyz;	// 法線
	output.tangent_       = mul(matWorld_, float4(input.tangent_ , 0)).xyz;	// 接ベクトル
	output.binormal_      = mul(matWorld_, float4(input.binormal_, 0)).xyz;	// 従法線ベクトル
	output.uv_            = input.uv_;		// UVを加工しない場合はそのまま代入でスルーさせる
	output.worldPosition_ = worldPosition;
	output.shadowmapUv_   = shadowPosition;

	return output;
}
