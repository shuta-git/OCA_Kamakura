//===========================================================================
//!	@file	BoxCollider.h
//!	@brief	ボックスコライダー
//===========================================================================
#pragma once

struct AABB
{
    Vector3 max_;
    Vector3 min_;
};

namespace gpu {

//===========================================================================
//!　ボックスコライダー
//===========================================================================
class BoxCollider : public Collider
{
public:
    
    virtual ~BoxCollider() = default;

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

    //! 高さを設定
    void setHeight(float height) { height_ = height; }

    //! 幅を設定
    void setWigth(float wigth) { wigth_ = wigth; }

    //! 奥行きを設定
    void setDepth(float depth) { depth_ = depth; }

    //! AABBを設定
    void setAABB(AABB box) { box_ = box; }

    //@}

    //-------------------------------------------------------------
    //!	@name	取得
    //-------------------------------------------------------------
    //@{

    //! 高さを取得
    float getHeight() const { return height_; }

    //! 幅を取得
    float getWigth() const { return wigth_; }

    //! 奥行きを取得
    float getDepth() const { return depth_; }

    //! AABBを取得
    AABB getAABB() const { 
        AABB result = box_;
        result.min_ = pos_;
        result.max_ = pos_;
        result.max_.x_ += wigth_;
        result.max_.y_ += height_;
        result.max_.z_ += depth_;
        return result; 
    }

    //@}

protected:

    AABB box_;

    float height_ = 1.0f;
    float wigth_  = 1.0f;
    float depth_  = 1.0f;

};

//! SphereColliderを作成
std::shared_ptr<gpu::BoxCollider> createBoxCollider();



}   // namespace gpu