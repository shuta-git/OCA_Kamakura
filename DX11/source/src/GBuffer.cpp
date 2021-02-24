//===========================================================================
//!	@file   GBuffer.cpp
//!	@brief	GBuffer管理
//===========================================================================
#include "GBuffer.h"

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool GBuffer::initialize(u32 w, u32 h)
{
    //-------------------------------------------------------------
    // GBufferの作成
    //-------------------------------------------------------------
    gbuffer_[0].reset(gpu::createTarget(w, h, DXGI_FORMAT_R8G8B8A8_UNORM));
    gbuffer_[1].reset(gpu::createTarget(w, h, DXGI_FORMAT_R8G8B8A8_UNORM));
    gbuffer_[2].reset(gpu::createTarget(w, h, DXGI_FORMAT_R16G16_FLOAT));
    gbuffer_[3].reset(gpu::createTarget(w, h, DXGI_FORMAT_R32G32B32A32_FLOAT));

    return true;
}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void GBuffer::finalize()
{
    gbuffer_[0].reset();
    gbuffer_[1].reset();
    gbuffer_[2].reset();
    gbuffer_[3].reset();
}

//---------------------------------------------------------------------------
//! 開始
//---------------------------------------------------------------------------
void GBuffer::begin()
{
    std::array<ptr<gpu::Texture>, 4> mrts{
        gbuffer_[0],
        gbuffer_[1],
        gbuffer_[2],
        gbuffer_[3],
    };

    gpu::setRenderTarget(mrts.size(), mrts.data(), GmRender()->getDepthStencil());
}

//---------------------------------------------------------------------------
//! シャドウパスの終了
//---------------------------------------------------------------------------
void GBuffer::end()
{
    // 元に戻す
    gpu::setRenderTarget(GmRender()->getRenderBuffer(), GmRender()->getDepthStencil());

    // GBufferの内容をテクスチャに設定
    gpu::setTexture(5, gbuffer_[0]);
    gpu::setTexture(6, gbuffer_[1]);
    gpu::setTexture(7, gbuffer_[2]);
    gpu::setTexture(8, gbuffer_[3]);
}

//---------------------------------------------------------------------------
//! Imgui描画
//---------------------------------------------------------------------------
void GBuffer::imguiRender()
{
    // ウィンドウの縮小サイズ
    //※ 値を大きくすればするほど小さくなります
    int windowShrinkSize = 8;

    //-------------------------------------------------------------
    // GBufferのデバッグウィンドウ
    //-------------------------------------------------------------
    ImGui::Begin(u8"GBuffer");

    if(ImGui::BeginTabBar("GBuffer")) {
        // MRT枚数分
        for(u32 i = 0; i < std::size(gbuffer_); ++i) {
            auto& x = gbuffer_[i];

            std::string tabName{ "GBuffer" };

            // GBuffer番号
            char number[2];
            sprintf_s(number, "%1d", i);
            tabName += number;

            if(ImGui::BeginTabItem(tabName.c_str())) {
                ImGui::Image(x->getD3DSrv(), ImVec2(x->getWidth() / windowShrinkSize, x->getHeight() / windowShrinkSize));
                ImGui::EndTabItem();
            }
        }

        if(ImGui::BeginTabItem("Depth")) {
            // デプスバッファ
            auto x = GmRender()->getDepthStencil();
            ImGui::Image(x->getD3DSrv(), ImVec2(x->getWidth() / windowShrinkSize, x->getHeight() / windowShrinkSize));
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::Separator();

    if(ImGui::BeginTabBar("Light Accumulation Buffer")) {
        if(ImGui::BeginTabItem("Light Accumulation Buffer")) {
            {
                auto x = GmRender()->getDiffuseTexture();
                ImGui::Image(x->getD3DSrv(), ImVec2(x->getWidth() / windowShrinkSize, x->getHeight() / windowShrinkSize));
            }
            ImGui::SameLine();
            {
                auto x = GmRender()->getSpecularTexture();
                ImGui::Image(x->getD3DSrv(), ImVec2(x->getWidth() / windowShrinkSize, x->getHeight() / windowShrinkSize));
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}
