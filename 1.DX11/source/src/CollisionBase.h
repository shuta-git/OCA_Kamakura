//===========================================================================
//!	@file	CollisionBase.h
//!	@brief	コリジョン基底
//===========================================================================

#pragma once

 enum class STATE
{
    ACTIVE,      //!< アクティブ
    PAUSED,      //!< 停止
    COLLISION,   //!< 衝突
    DEAD         //!< 死亡
};


class CollisionBase : public Base
{
public:
    virtual ~CollisionBase() = default;

    //! 初期化
    virtual bool initialize() = 0;

    //! 更新
    virtual void update() = 0;

    //! 描画
    virtual void render() = 0;

    //! 解放
    virtual void finalize() = 0;

    //! imgui描画
    virtual void imguiRender() = 0;

public:
    //-------------------------------------------------------------
    //!	@name	参照
    //-------------------------------------------------------------
    //@{

    //! 状態を設定
    void setState(STATE state) { state_ = state; }

    //@)

    //-------------------------------------------------------------
    //!	@name	取得
    //-------------------------------------------------------------
    //@{

    //! 状態を取得
    STATE getState() const { return state_; }

    //@)

private:

    STATE state_;   //!< 状態管理

public:

    bool colliderRender = false; //!< ボックスコライダーを描画するかどうか



};
