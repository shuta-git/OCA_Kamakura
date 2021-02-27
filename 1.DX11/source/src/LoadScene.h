
//===========================================================================
//!	@file	LoadScene.h
//!	@brief	ロードシーン
//===========================================================================
#pragma once

//===========================================================================
//! タイトルシーン
//===========================================================================
class LoadScene : public Scene
{
public:
    //! コンストラクタ
    LoadScene();

    //! デストラクタ
    virtual ~LoadScene();

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
    //! ファイル読み込み
    void loadFile();

private:

    std::unique_ptr<gpu::Texture> texture_;

    f32 time_;//!< ロード時間
};
