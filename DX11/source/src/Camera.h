//===========================================================================
//!	@file   Camera.h
//!	@brief	カメラ
//===========================================================================
#pragma once

//===========================================================================
//! カメラ
//===========================================================================
class Camera
{
public:
    //! コンストラクタ
    Camera();

    //! デストラクタ
    virtual ~Camera();

    //! 更新
    virtual void update();

    //-------------------------------------------------------------
    //!	@name	設定
    //-------------------------------------------------------------
    //@{

    //! 位置を設定
    void setPosition(const Vector3& position) { position_ = position; }

    //! 注視点を設定
    void setLookAt(const Vector3& lookAt) { lookAt_ = lookAt; }

    //! 上方向のベクトルを設定
    void setWorldUp(const Vector3& worldUp) { worldUp_ = worldUp; }

    //! 画角を設定
    void setFovY(f32 fovY) { fovY_ = fovY; }

    //! アスペクト比を設定
    void setAspectRatio(f32 aspectRatio) { aspectRatio_ = aspectRatio; }

    //! 近クリップ面を設定
    void setNearZ(f32 zNear) { zNear_ = zNear; }

    //! 遠クリップ面を設定
    void setFarZ(f32 zFar) { zFar_ = zFar; }

    //@}

    //-------------------------------------------------------------
    //!	@name	取得
    //-------------------------------------------------------------

    //@{

    //! カメラのワールド行列を取得
    Matrix getWorldMatrix() const { return matWorld_; }

    //! ビュー行列を取得
    Matrix getViewMatrix() const { return matView_; }

    //! 投影行列を取得
    Matrix getProjMatrix() const { return matProj_; }

	//! 位置を取得
    Vector3 getPosition() const { return position_; }

    //! 注視点を取得
    Vector3 getLookAt() const { return lookAt_; }

    //! 上方向のベクトルを取得
    Vector3 getWorldUp() const { return worldUp_; }

    //! 画角を取得
    f32 getFovY() const { return fovY_; }

    //! アスペクト比を取得
    f32 getAspectRatio() const { return aspectRatio_; }

    //! 近クリップ面を取得
    f32 getNearZ() const { return zNear_; }

    //! 遠クリップ面を取得
    f32 getFarZ() const { return zFar_; }

    //@}

private:
    Matrix  matWorld_;   //!< カメラのワールド行列
    Matrix  matView_;    //!< ビュー行列
    Vector3 position_;   //!< 位置
    Vector3 lookAt_;     //!< 注視点
    Vector3 worldUp_;    //!< 上方向のベクトル

    Matrix matProj_;       //!< 投影行列
    f32    fovY_;          //!< 画角
    f32    aspectRatio_;   //!< アスペクト比
    f32    zNear_;         //!< 近クリップ面
    f32    zFar_;          //!< 遠クリップ面
};
