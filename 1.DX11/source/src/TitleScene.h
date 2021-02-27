//===========================================================================
//!	@file	TitleScene.h
//!	@brief	タイトルシーン
//===========================================================================
#pragma once

//===========================================================================
//! タイトルシーン
//===========================================================================
class TitleScene : public Scene
{
public:
    //! コンストラクタ
    TitleScene();

    //! デストラクタ
    virtual ~TitleScene();

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

    //! テキストアニメーション
    void textAnimation();

private:

    std::unique_ptr<Map>          map_;      //!< マップ

    std::unique_ptr<CameraTitle> cameraTitle_;   //!< カメラ

    f32 textScale_;
    std::string text_;
    bool        isRender_;

    f32 time_;   //!< 経過時間(s)

};
