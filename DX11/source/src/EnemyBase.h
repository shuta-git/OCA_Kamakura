//===========================================================================
//!	@file	EnemyBase.h
//!	@brief	エネミー基底
//===========================================================================
#pragma once

//===========================================================================
//!	エネミー基底
//===========================================================================
class EnemyBase : public CollisionBase
{
public:
    EnemyBase()           = default;
    virtual ~EnemyBase() = default;

    //! 初期化
    virtual bool initialize() = 0;

    //! 更新
    virtual void update() = 0;

    //! 更新
    virtual void update(Vector3 playerPos)  = 0;

    //! 描画
    virtual void render() = 0;

    //! 解放
    virtual void finalize() = 0;

    //! imgui描画
    virtual void imguiRender() = 0;

    //-------------------------------------------------------------
    //!	@name	取得
    //-------------------------------------------------------------
    //@{

    std::shared_ptr<gpu::BoxCollider>     getBoxCollider() { return boxCollider_; }
    
    //@}

protected:

    //! 移動
    void move();

    //! 接近
    //! @parm [in] target    ターゲット
    void Approach(Vector3 target);

protected:

    std::shared_ptr<gpu::ModelFBX> modelFbx_;   //!< モデルFBX
    std::shared_ptr<gpu::BoxCollider>     boxCollider_;       //!< ボックスコライダー

    float moveSpeed_;   //!< 移動速度

};
