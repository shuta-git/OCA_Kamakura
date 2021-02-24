//===========================================================================
//!	@file   Primitive.h
//!	@brief	プリミティブ描画
//===========================================================================
#pragma once

//! 初期化
bool PRIM_Initialize();

//! 解放
void PRIM_Finalize();

//===========================================================================
// OpenGL互換関数
//===========================================================================


//! 行列の設定
void dxLoadMatrixf(const Matrix& matWorld);

//! 行列スタックへ退避
void dxPushMatrix();

//! 行列スタックから復帰
void dxPopMatrix();


//! 描画開始
//!	@param	[in]	type	プリミティブの種類
void dxBegin(PRIMITIVE_TYPE type);

//! 頂点キック (Kick)
void dxVertex3f(f32 x, f32 y, f32 z);
void dxVertex3fv(const Vector3& v);

//! カラーu8
void dxColor4ub(u8 r, u8 g, u8 b, u8 a);

//! カラーf32
void dxColor4f(f32 r, f32 g, f32 b, f32 a);

//! テクスチャ座標
void dxTexCoord2f(f32 u, f32 v);

//! 法線
void dxNormal3fv(const Vector3& v);

//! 接ベクトル
void dxTangent3fv(const Vector3& v);

//! 従法線ベクトル
void dxBinormal3fv(const Vector3& v);

//! 描画終了
void dxEnd();

//! テクスチャの設定
void dxTexture(ptr<gpu::Texture> texture);
