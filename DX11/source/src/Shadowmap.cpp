//===========================================================================
//!	@file   Shadowmap.cpp
//!	@brief	シャドウマップ管理
//===========================================================================
#include "Shadowmap.h"

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool Shadowmap::initialize(s32 resolution)
{
    resolution_ = resolution;

    //-------------------------------------------------------------
    // デプステクスチャの作成
    //-------------------------------------------------------------
    shadowDepthTexture_.reset(gpu::createTarget(resolution, resolution, DXGI_FORMAT_D32_FLOAT));
    if(!shadowDepthTexture_) {
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void Shadowmap::finalize()
{
    shadowDepthTexture_.reset();
}

//---------------------------------------------------------------------------
//! シャドウパスの開始
//---------------------------------------------------------------------------
void Shadowmap::begin()
{
    //------------------------------------------------------------
    // シャドウデプスバッファのクリア
    //------------------------------------------------------------

    // 最遠景の1.0fで初期化。Ｚ比較はLESSEQUALを使用
    gpu::D3DContext()->ClearDepthStencilView(shadowDepthTexture_->getD3DDsv(),   // デプステクスチャ
                                             D3D11_CLEAR_DEPTH,                  // クリアする対象
                                             1.0f,                               // クリアするデプスの値
                                             0);                                 // クリアするステンシルの値

    //------------------------------------------------------------
    // デプスバッファの設定 (倍速Zモード)
    //------------------------------------------------------------
    gpu::setRenderTarget(nullptr, shadowDepthTexture_);

    //------------------------------------------------------------
    // 光源カメラの設定
    //------------------------------------------------------------
    lightCamera_.setPosition(Vector3(20, 20, 20));
    lightCamera_.setLookAt(Vector3(0, 0, 0));
    lightCamera_.update();   // 行列生成

    const f32 range         = 5.0f;     // 撮影の幅(±の範囲)
    const f32 limitDistance = 100.0f;   // 影の遠クリップ面

    matLightView_ = lightCamera_.getViewMatrix();
    matLightProj_ = Matrix::scale(Vector3(0.5f / range, 0.5f / range, 1.0f / limitDistance * -1.0f));

    GmRender()->setCamera(matLightView_, matLightProj_);
}

//---------------------------------------------------------------------------
//! シャドウパスの終了
//---------------------------------------------------------------------------
void Shadowmap::end()
{
}

//---------------------------------------------------------------------------
//! Imgui描画
//---------------------------------------------------------------------------
void Shadowmap::imguiRender()
{
    //-------------------------------------------------------------
    // シャドウマップのデバッグウィンドウ
    //-------------------------------------------------------------
    ImGui::Begin(u8"シャドウマップ");
    {
        ImGui::InputInt(u8"解像度", &resolution_, 1, 100, ImGuiInputTextFlags_ReadOnly);
        ImGui::Image(shadowDepthTexture_->getD3DSrv(), ImVec2(512, 512));
    }
    ImGui::End();
}
