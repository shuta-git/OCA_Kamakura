//===========================================================================
//!	@file	SceneMain.h
//!	@brief	メインシーン
//===========================================================================
#pragma once

//===========================================================================
//! メインシーン
//===========================================================================
class SceneMain : public Scene
{
public:
    //! コンストラクタ
    SceneMain();

    //! デストラクタ
    virtual ~SceneMain();

    //! 初期化
    virtual bool initialize() override;

    //! 更新
    virtual void update() override;

    //! 描画
    virtual void render(RenderMode mode) override;

    //! 解放
    virtual void finalize() override;

    //! imgui描画
    virtual void imguiRender() override;

private:
    Matrix                         matWorld_;   //!< ワールド行列
    std::unique_ptr<gpu::ShaderVs> shaderVs_;   //!< 頂点シェーダー
    std::unique_ptr<gpu::ShaderPs> shaderPs_;   //!< ピクセルシェーダー
    std::unique_ptr<gpu::ShaderCs> shaderCs_;   //!< 演算シェーダー

    gpu::Buffer bufferBoxVb_;   //!< ボックス描画用頂点
    gpu::Buffer bufferBoxIb_;   //!< ボックス描画用インデックス

    ModelMQO model_;
    std::shared_ptr<gpu::ModelFBX> modelFbx_;
    std::shared_ptr<gpu::ModelFBX> modelFbx2_;

    //---- テクスチャ(Shader Resource)
    std::unique_ptr<gpu::Texture> texture_;   //!< テクスチャ

    std::unique_ptr<gpu::Texture> cubeEnvironment_;   //!< 環境マップテクスチャ

    std::unique_ptr<CameraDebug> camera_;   //!< カメラ

};

