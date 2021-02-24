//===========================================================================
//! @file   GpuCommand.h
//!	@brief	GPUコマンド
//===========================================================================
#pragma once

//--------------------------------------------------------------
// プリミティブの種類
//--------------------------------------------------------------
enum PRIMITIVE_TYPE
{
    PT_POINTS         = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
    PT_LINES          = D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
    PT_LINE_STRIP     = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
    PT_TRIANGLES      = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
    PT_TRIANGLE_STRIP = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
};

namespace gpu {

//! 入力レイアウトの設定
void setInputLayout(ptr<const gpu::InputLayout> inputLayout);

// 頂点バッファの設定
void setVertexBuffer(ptr<const gpu::Buffer> buffer, u32 stride, u32 offset = 0);

//! インデックスバッファの設定
void setIndexBuffer(ptr<const gpu::Buffer> buffer, u32 offset = 0);

//! シェーダーの設定(VS,PS)
void setShader(ptr<const gpu::ShaderVs> vs, ptr<const gpu::ShaderPs> ps);

//! シェーダーの設定(CS)
void setShader(ptr<const gpu::ShaderCs> cs);

//! 定数バッファの設定
void setConstantBuffer(u32 slot, ptr<gpu::Buffer> buffer);

//! テクスチャの設定
void setTexture(u32 slot, ptr<gpu::Texture> texture);

//! RWテクスチャの設定
void setRWTexture(u32 slot, ptr<gpu::Texture> texture);

//! カリング設定
//!	@param [in]	cullMode	カリング
void setCullMode(D3D11_RASTERIZER_DESC rd);

//! 描画
//!	@param [in]	type	プリミティブの種類
//!	@param [in]	vertexCount	頂点数
//!	@param [in]	vertexOffet	頂点番号のオフセット
void draw(PRIMITIVE_TYPE type, u32 vertexCount, u32 vertexOffset = 0);

//! インデックス描画
//!	@param [in]	type	プリミティブの種類
//!	@param [in]	indexCount	インデックス数
//!	@param [in]	indexOffset	インデックス番号のオフセット
//!	@param [in]	vertexOffet	頂点番号のオフセット
void drawIndexed(PRIMITIVE_TYPE type, u32 indexCount, u32 indexOffset = 0, u32 vertexOffset = 0);

//! 演算実行
//!	@param	[in]	groupCountX	スレッドグループ数X
//!	@param	[in]	groupCountY	スレッドグループ数Y
//!	@param	[in]	groupCountZ	スレッドグループ数Z
void dispatch(u32 groupCountX, u32 groupCountY, u32 groupCountZ);

// 描画ターゲットの設定
void setRenderTarget(ptr<gpu::Texture> renderTarget, ptr<gpu::Texture> depthStencil = nullptr);

// 描画ターゲットの設定(MRT用)
void setRenderTarget(u32 renderTargetCount, ptr<gpu::Texture>* renderTargets, ptr<gpu::Texture> depthStencil = nullptr);

// 描画ターゲットのクリア
void clearTarget(ptr<gpu::Texture> target, const Vector4& color = Vector4(0.0f, 0.0f, 0.0f, 0.0f), f32 depth = 1.0f, u8 stencil = 0);

}   // namespace gpu
