//===========================================================================
//!	@file	GameScene.h
//!	@brief	ゲームシーン
//===========================================================================
#pragma once

//===========================================================================
//! ゲームシーン
//===========================================================================
class GameScene : public Scene
{
public:
    //! コンストラクタ
    GameScene();

    //! デストラクタ
    virtual ~GameScene();

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

    //! 当たり判定
    void collisionDetection();

private:

    std::unique_ptr<gpu::ShaderCs> shaderCs_;   //!< 演算シェーダー

    std::unique_ptr<Player> player_;        //!< プレイヤー
    std::unique_ptr<EnemyManager> enemy_;   //!< エネミー
    std::unique_ptr<Map> map_;              //!< マップ

    f32         textScale_;
    std::string text_;

    f32 time_;

};
