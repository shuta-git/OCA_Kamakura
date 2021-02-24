#include "shader.h"

//---------------------------------------------------------------------------
//	プリミティブ用頂点シェーダー
//---------------------------------------------------------------------------
PS_INPUT main(VS_INPUT input)
{
	PS_INPUT	output = (PS_INPUT)0;	// 頂点シェーダー出力結果はピクセルシェーダーに使われる

	//-------------------------------------------------------------
	// 頂点の座標変換
	//-------------------------------------------------------------
	float4	position = input.position_;

	position = mul(matWorld_, position);
	position = mul(matView_, position);
	position = mul(matProj_, position);

	output.position_ = position;		// 座標は投影後のxyzw座標値 (GPUによって頂点シェーダー実行後に自動でxyz÷wが実行される)
	output.color_    = input.color_;	// パススルー
	output.uv_       = input.uv_;		// UVを加工しない場合はそのまま代入でスルーさせる

	return output;
}
