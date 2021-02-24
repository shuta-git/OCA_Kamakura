//===========================================================================
//!	@file   Shadowmap.h
//!	@brief	シャドウマップ管理
//===========================================================================
#pragma once

//===========================================================================
//!	シャドウマップ管理
//===========================================================================
class Shadowmap
{
public:
    Shadowmap()  = default;
    ~Shadowmap() = default;

    //! 初期化
    bool initialize(s32 resolution = 2048);

    //! 解放
    void finalize();

    //! シャドウパスの開始
    void begin();

    //! シャドウパスの終了
    void end();

    //! Imgui描画
    void imguiRender();

    //! シャドウテクスチャの取得
    gpu::Texture* getShadowTexture() const { return shadowDepthTexture_.get(); }

    //! 光源用ビュー行列
    Matrix getViewMatrix() const { return matLightView_; }

    //! 光源用投影行列
    Matrix getProjMatrix() const { return matLightProj_; }

private:
    s32                           resolution_ = 2048;    //!< シャドウマップの解像度
    std::unique_ptr<gpu::Texture> shadowDepthTexture_;   //!< 影用のデプステクスチャ
    Camera                        lightCamera_;          //!< 光源用カメラ
    Matrix                        matLightView_;         //!< 光源用ビュー行列
    Matrix                        matLightProj_;         //!< 光源用投影行列
};
