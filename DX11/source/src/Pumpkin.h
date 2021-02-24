//===========================================================================
//!	@file	Pumpkin.h
//!	@brief	プレイヤー
//===========================================================================
#pragma once

//===========================================================================
//!　エネミー
//===========================================================================
class Pumpkin : public EnemyBase
{
public:
    //! コンストラクタ
    Pumpkin(Vector3 pos);

    //! デストラクタ
    virtual ~Pumpkin();

    //! 初期化
    virtual bool initialize() override;

    //! 更新
    virtual void update() override;

    //! 更新
    virtual void update(Vector3 playerPos) override;

    //! 描画
    virtual void render() override;

    //! 解放
    virtual void finalize() override;

    //! imgui描画
    virtual void imguiRender() override;

private:

};
