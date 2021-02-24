//===========================================================================
//!	@file	Player.h
//!	@brief	プレイヤー
//===========================================================================
#pragma once

//===========================================================================
//!　プレイヤー
//===========================================================================
class Player : public PlayerBase
{
public:
    //! コンストラクタ
    Player();

    //! デストラクタ
    virtual ~Player();

    //! 初期化
    virtual bool initialize() override;

    //! 更新
    virtual void update() override;

    //! 描画
    virtual void render() override;

    //! 解放
    virtual void finalize() override;

    //! imgui描画
    virtual void imguiRender() override;

private:

    std::unique_ptr<CameraDebug> cameraDebug_;     //!< カメラ
    


};
