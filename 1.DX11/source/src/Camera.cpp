//===========================================================================
//!	@file   Camera.cpp
//!	@brief	カメラ
//===========================================================================
#include "Camera.h"

//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
Camera::Camera()
: matWorld_{ Matrix::identity() }   // カメラのワールド行列
, matView_{ Matrix::identity() }    // ビュー行列
, position_{ 0.0f, 0.0f, 0.0f }     // 位置
, lookAt_{ 0.0f, 0.0f, 0.0f }       // 注視点
, worldUp_{ 0.0f, 0.0f, 0.0f }      // 上方向のベクトル
, matProj_{ Matrix::identity() }    // 投影行列
, fovY_{ XM_PIDIV4 }                // 画角
, aspectRatio_{ 16.0f / 9.0f }      // アスペクト比
, zNear_{ 0.01f }                   // 近クリップ面
, zFar_{ 1000.0f }                  // 遠クリップ面
{
}

//---------------------------------------------------------------------------
//! デストラクタ
//---------------------------------------------------------------------------
Camera::~Camera() = default;

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void Camera::update()
{
    //-------------------------------------------------------------
    // カメラのワールド行列を作成
    //-------------------------------------------------------------
    // (1) Z軸
    Vector3 axisZ = (position_ - lookAt_).normalize();

    // (2) X軸
    Vector3 axisX = Vector3::cross(Vector3(0.0f, 1.0f, 0.0f), axisZ).normalize();

    // (3) Y軸
    Vector3 axisY = Vector3::cross(axisZ, axisX);

    // 行列に設定
    matWorld_.m_[0][0] = axisX.x_;
    matWorld_.m_[0][1] = axisX.y_;
    matWorld_.m_[0][2] = axisX.z_;
    matWorld_.m_[0][3] = 0.0f;

    matWorld_.m_[1][0] = axisY.x_;
    matWorld_.m_[1][1] = axisY.y_;
    matWorld_.m_[1][2] = axisY.z_;
    matWorld_.m_[1][3] = 0.0f;

    matWorld_.m_[2][0] = axisZ.x_;
    matWorld_.m_[2][1] = axisZ.y_;
    matWorld_.m_[2][2] = axisZ.z_;
    matWorld_.m_[2][3] = 0.0f;

    matWorld_.m_[3][0] = position_.x_;
    matWorld_.m_[3][1] = position_.y_;
    matWorld_.m_[3][2] = position_.z_;
    matWorld_.m_[3][3] = 1.0f;

    //-------------------------------------------------------------
    // ビュー行列を作成
    //-------------------------------------------------------------
    // ※ ビュー行列 = カメラのワールド行列の逆行列
    matView_.m_[0][0] = axisX.x_;
    matView_.m_[0][1] = axisY.x_;
    matView_.m_[0][2] = axisZ.x_;
    matView_.m_[0][3] = 0.0f;

    matView_.m_[1][0] = axisX.y_;
    matView_.m_[1][1] = axisY.y_;
    matView_.m_[1][2] = axisZ.y_;
    matView_.m_[1][3] = 0.0f;

    matView_.m_[2][0] = axisX.z_;
    matView_.m_[2][1] = axisY.z_;
    matView_.m_[2][2] = axisZ.z_;
    matView_.m_[2][3] = 0.0f;

    matView_.m_[3][0] = -Vector3::dot(axisX, position_);
    matView_.m_[3][1] = -Vector3::dot(axisY, position_);
    matView_.m_[3][2] = -Vector3::dot(axisZ, position_);
    matView_.m_[3][3] = 1.0f;

    //-------------------------------------------------------------
    // 投影行列の作成
    //-------------------------------------------------------------
    f32 sinFov = sinf(0.5f * fovY_);
    f32 cosFov = cosf(0.5f * fovY_);

    float height = cosFov / sinFov;
    float width  = height / aspectRatio_;
    float range  = zFar_ / (zNear_ - zFar_);

    matProj_.m_[0][0] = width;
    matProj_.m_[0][1] = 0.0f;
    matProj_.m_[0][2] = 0.0f;
    matProj_.m_[0][3] = 0.0f;

    matProj_.m_[1][0] = 0.0f;
    matProj_.m_[1][1] = height;
    matProj_.m_[1][2] = 0.0f;
    matProj_.m_[1][3] = 0.0f;

    matProj_.m_[2][0] = 0.0f;
    matProj_.m_[2][1] = 0.0f;
    matProj_.m_[2][2] = range;
    matProj_.m_[2][3] = -1.0f;

    matProj_.m_[3][0] = 0.0f;
    matProj_.m_[3][1] = 0.0f;
    matProj_.m_[3][2] = range * zNear_;
    matProj_.m_[3][3] = 0.0f;
}
