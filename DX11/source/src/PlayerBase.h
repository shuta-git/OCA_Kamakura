//===========================================================================
//!	@file	PlayerBase.h
//!	@brief	プレイヤー基底
//===========================================================================
#pragma once

//===========================================================================
//!	プレイヤー基底
//===========================================================================
class PlayerBase : public CollisionBase
{
public:
    PlayerBase() = default;
    virtual ~PlayerBase() = default;

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
    //!	@name	取得
    //-------------------------------------------------------------
    //@{

    std::shared_ptr<gpu::BoxCollider> getBoxCollider() const { return boxCollider_; }
    std::shared_ptr<gpu::BoxCollider> getAttackCollider() const { return attackCollider_; }

    //@}

    //-------------------------------------------------------------
    //!	@name	参照
    //-------------------------------------------------------------
    //@{

    bool getIsAttack() { return isAttack_; }

    //@}

protected:

     //! 移動
    void move();

    //! 回転
    void rotate();

    //! 攻撃
    void attack();

    //! 回転方向
    void rotateDir(Vector3 cameraDir);

protected:

    std::shared_ptr<gpu::ModelFBX> modelFbx_;   //!< モデルFBX
    std::unique_ptr<CameraPlayer>  cameraPlayer_;   //!< カメラ

    std::shared_ptr<gpu::BoxCollider> boxCollider_;       //!< ボックスコライダー
    std::shared_ptr<gpu::BoxCollider> attackCollider_;    //!< 攻撃判定用のコライダー

    float moveSpeed_;   //!< 移動速度

    bool isAttack_;   //!< 攻撃をしてるかどうか

};
