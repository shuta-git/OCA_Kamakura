//===========================================================================
//! @file  GpuCommand.cpp
//!	@brief	GPUコマンド
//===========================================================================
#include <unordered_map>
#include "GpuCommand.h"

namespace gpu {

namespace {
const gpu::InputLayout* gInputLayout;
const gpu::ShaderVs*    gShaderVs;
}   // namespace

//---------------------------------------------------------------------------
//! 入力レイアウトの設定
//---------------------------------------------------------------------------
void setInputLayout(ptr<const gpu::InputLayout> inputLayout)
{
    gInputLayout = inputLayout;
}

//---------------------------------------------------------------------------
// 頂点バッファの設定
//---------------------------------------------------------------------------
void setVertexBuffer(ptr<const gpu::Buffer> buffer, u32 stride, u32 offset)
{
    ID3D11Buffer* buffers[1]{ buffer->getD3DBuffer() };
    u32           strides[1]{ stride };
    u32           offsets[1]{ offset };
    D3DContext()->IASetVertexBuffers(0, 1, buffers, strides, offsets);
}

//---------------------------------------------------------------------------
//! インデックスバッファの設定
//---------------------------------------------------------------------------
void setIndexBuffer(ptr<const gpu::Buffer> buffer, u32 offset)
{
    D3DContext()->IASetIndexBuffer(buffer->getD3DBuffer(), DXGI_FORMAT_R32_UINT, offset);
}

//---------------------------------------------------------------------------
//! シェーダーの設定
//---------------------------------------------------------------------------
void setShader(ptr<const gpu::ShaderVs> vs, ptr<const gpu::ShaderPs> ps)
{
    D3DContext()->VSSetShader(vs->get(), nullptr, 0);
    D3DContext()->PSSetShader(ps->get(), nullptr, 0);

    gShaderVs = vs;
}

void setShader(ptr<const gpu::ShaderCs> cs)
{
    D3DContext()->CSSetShader(cs->get(), nullptr, 0);
}

//---------------------------------------------------------------------------
// 定数バッファの設定
//---------------------------------------------------------------------------
void setConstantBuffer(u32 slot, ptr<gpu::Buffer> buffer)
{
    ID3D11Buffer* d3dBuffer = buffer ? buffer->getD3DBuffer() : nullptr;
    D3DContext()->VSSetConstantBuffers(slot, 1, &d3dBuffer);
    D3DContext()->PSSetConstantBuffers(slot, 1, &d3dBuffer);
    D3DContext()->CSSetConstantBuffers(slot, 1, &d3dBuffer);
}

//---------------------------------------------------------------------------
// テクスチャの設定
//---------------------------------------------------------------------------
void setTexture(u32 slot, ptr<gpu::Texture> texture)
{
    ID3D11ShaderResourceView* srv = texture ? texture->getD3DSrv() : nullptr;
    D3DContext()->PSSetShaderResources(slot, 1, &srv);   // テクスチャ
    D3DContext()->CSSetShaderResources(slot, 1, &srv);   // テクスチャ
}

//---------------------------------------------------------------------------
// RWテクスチャの設定
//---------------------------------------------------------------------------
void setRWTexture(u32 slot, ptr<gpu::Texture> texture)
{
    auto* uav = texture ? texture->getD3DUav() : nullptr;
    D3DContext()->CSSetUnorderedAccessViews(slot, 1, &uav, nullptr);   // RWテクスチャ
}

//---------------------------------------------------------------------------
//! カリング設定
//---------------------------------------------------------------------------
void setCullMode(D3D11_RASTERIZER_DESC rd)
{
    //D3D11_RASTERIZER_DESC rd{};
    //rd.FillMode = D3D11_FILL_SOLID;
    //// 背面カリングをしない場合
    //rd.CullMode        = D3D11_CULL_NONE;
    //rd.ScissorEnable   = false;
    //rd.DepthClipEnable = false;
    //// 時計回り(clockwise)の頂点を持つ面を表と見なす (デフォルト)
    //rd.FrontCounterClockwise = FALSE;

    ID3D11RasterizerState* pState;
    gpu::D3DDevice()->CreateRasterizerState(&rd, &pState);
    gpu::D3DContext()->RSSetState(pState);
    pState->Release();
}

//---------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------
void draw(PRIMITIVE_TYPE type, u32 vertexCount, u32 vertexOffset)
{
    if(gInputLayout == nullptr)
        return;

    auto inputLayout    = const_cast<gpu::InputLayout*>(gInputLayout);
    auto d3dInputLayout = inputLayout->getD3DInputLayout(gShaderVs);

    D3DContext()->IASetInputLayout(d3dInputLayout);

    D3DContext()->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)type);
    D3DContext()->Draw(vertexCount, vertexOffset);
}

//---------------------------------------------------------------------------
//! インデックス描画
//---------------------------------------------------------------------------
void drawIndexed(PRIMITIVE_TYPE type, u32 indexCount, u32 indexOffset, u32 vertexOffset)
{
    auto inputLayout    = const_cast<gpu::InputLayout*>(gInputLayout);
    auto d3dInputLayout = inputLayout->getD3DInputLayout(gShaderVs);

    D3DContext()->IASetInputLayout(d3dInputLayout);

    D3DContext()->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)type);
    D3DContext()->DrawIndexed(indexCount, indexOffset, vertexOffset);
}

//---------------------------------------------------------------------------
//! 演算実行
//---------------------------------------------------------------------------
void dispatch(u32 groupCountX, u32 groupCountY, u32 groupCountZ)
{
    D3DContext()->Dispatch(groupCountX, groupCountY, groupCountZ);
}

//---------------------------------------------------------------------------
// 描画ターゲットの設定
//---------------------------------------------------------------------------
void setRenderTarget(ptr<gpu::Texture> renderTarget, ptr<gpu::Texture> depthStencil)
{
    gpu::setRenderTarget(1, &renderTarget, depthStencil);
}

// 描画ターゲットの設定(MRT用)
void setRenderTarget(u32 renderTargetCount, ptr<gpu::Texture>* renderTargets, ptr<gpu::Texture> depthStencil)
{
    ID3D11RenderTargetView* d3dRtvs[8];

    for(u32 i = 0; i < renderTargetCount; ++i) {
        d3dRtvs[i] = (renderTargets[i]) ? renderTargets[i]->getD3DRtv() : nullptr;
    }

    auto d3dDsv = (depthStencil) ? depthStencil->getD3DDsv() : nullptr;

    gpu::D3DContext()->OMSetRenderTargets(renderTargetCount, d3dRtvs, d3dDsv);

    //---- ビューポート設定（描画ターゲットの変更時はビューポートの更新も行う）
    D3D11_VIEWPORT viewport;

    auto texture = (renderTargets[0]) ? renderTargets[0] : depthStencil;

    viewport.TopLeftX = 0.0f;                                 // 左上X
    viewport.TopLeftY = 0.0f;                                 // 左上Y
    viewport.Width    = texture ? texture->getWidth() : 0;    // 幅
    viewport.Height   = texture ? texture->getHeight() : 0;   // 高さ
    viewport.MinDepth = 0.0f;                                 // Z最小値
    viewport.MaxDepth = 1.0f;                                 // Z最大値

    gpu::D3DContext()->RSSetViewports(1, &viewport);
}

void clearTarget(ptr<gpu::Texture> target, const Vector4& color, f32 depth, u8 stencil)
{
    // 描画ターゲットのクリア
    if(target->getD3DRtv()) {
        auto clearColor = reinterpret_cast<const f32*>(&color);
        gpu::D3DContext()->ClearRenderTargetView(target->getD3DRtv(), clearColor);
    }

    // デプスバッファのクリア
    if(target->getD3DDsv()) {
        gpu::D3DContext()->ClearDepthStencilView(target->getD3DDsv(),
                                                 D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
                                                 depth,
                                                 stencil);
    }
}

}   // namespace gpu
