//===========================================================================
//!	@file	CapsuleCollider.h
//!	@brief	カプセルコライダー
//===========================================================================
#pragma once

namespace gpu {

//===========================================================================
//!　カプセルコライダー
//===========================================================================
class CapsuleCollider : public Collider
{
public:
    virtual ~CapsuleCollider() = default;

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

    //! 高さを設定
    void setHeight(float height) { height_ = height; }

    //! 半球の高さを設定
    void setSphereHeight(float sphereHeight) { sphereHeight_ = sphereHeight; }

    //@}

    //-------------------------------------------------------------
    //!	@name	取得
    //-------------------------------------------------------------
    //@{

    //! 半径を取得
    float getRadius() const { return radius_; }

    //! カプセルの中心からの幅を取得
    float getHeight() const { return height_; }

    //! 半球の高さを取得
    float getSphereHeight() const { return sphereHeight_; }

    //! 上の半球の座標
    Vector3 getTopPos() const { 
        Vector3 result = pos_;
        result.y_      += height_;
        return result; 
    }

    //! 上の半球の座標
    Vector3 getUnderPos() const
    {
        Vector3 result = pos_;
        result.y_      -= height_;
        return result;
    }


    //@}

protected:

    float radius_       = 1.0f;   //!< 半径
    float height_       = 1.0f;   //!< カプセルの高さ
    float sphereHeight_ = 0.5f;   //!< 半球の高さ

};

//! CapsuleColliderを作成
std::shared_ptr<gpu::CapsuleCollider> createCapsuleCollider();

}   // namespace gpu