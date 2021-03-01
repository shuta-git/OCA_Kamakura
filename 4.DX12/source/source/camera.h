//---------------------------------------------------------------------------
//!	@file	camera.h
//!	@brief	カメラ
//---------------------------------------------------------------------------
#pragma once

//===========================================================================
//! カメラ
//===========================================================================
class Camera
{
public:
    Camera()          = default;
    virtual ~Camera() = default;

    //! 位置を設定
    void setPosition(const float3& position);

    //! 注視点を設定
    void setLookAt(const float3& lookAt);

    //! 世界の上方向ベクトルを設定
    void setWorldUp(const float3& worldUp);

    //! 更新
    virtual void update();

    //----------------------------------------------------------
    //! @name   参照
    //----------------------------------------------------------
    //@{

    //! ビュー行列を取得
    const matrix& view();

    //! 投影行列を取得
    const matrix& projection();

    //@}
protected:
    float3 position_       = { 0.0f, 3.0f, 5.0f };   //!< 位置座標
    float3 lookAt_         = { 0.0f, 0.0f, 0.0f };   //!< 注視点
    float3 worldUp_        = math::AXIS_Y.xyz;       //!< 世界の上方向のベクトル
    matrix matCameraWorld_ = math::identity();       //!< カメラ自身のワールド行列
    matrix matView_        = math::identity();       //!< ビュー行列
    matrix matProj_        = math::identity();       //!< 投影行列
};

//===========================================================================
//! デバッグ用カメラ
//! キーボードとマウスで操作
//===========================================================================
class CameraDebug : public Camera
{
public:
    //! 更新
    virtual void update() override;
};