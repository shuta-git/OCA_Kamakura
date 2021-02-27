//===========================================================================
//!	@file	SphereCollider.h
//!	@brief	スフィアコライダー
//===========================================================================
#pragma once

namespace gpu {

//===========================================================================
//!　スフィアコライダー
//===========================================================================
class SphereCollider : public Collider
{
public:
    virtual ~SphereCollider() = default;

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

    //-------------------------------------------------------------
    //!	@name	参照
    //-------------------------------------------------------------
    //@{

    //! 半径を設定
    void setRadius(float radius) { radius_ = radius; }

    //@}

    //-------------------------------------------------------------
    //!	@name	取得
    //-------------------------------------------------------------
    //@{

    //! 半径の取得
    float getRadius() const { return radius_; }

    //@}

protected:

    float  radius_ = 1.0f;   //!< 半径

};

//! SphereColliderを作成
std::shared_ptr<gpu::SphereCollider> createSphereCollider();

}



