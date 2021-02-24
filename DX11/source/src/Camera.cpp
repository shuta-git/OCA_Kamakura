//===========================================================================
//!	@file   Camera.cpp
//!	@brief	�J����
//===========================================================================
#include "Camera.h"

//---------------------------------------------------------------------------
//! �R���X�g���N�^
//---------------------------------------------------------------------------
Camera::Camera()
: matWorld_{ Matrix::identity() }   // �J�����̃��[���h�s��
, matView_{ Matrix::identity() }    // �r���[�s��
, position_{ 0.0f, 0.0f, 0.0f }     // �ʒu
, lookAt_{ 0.0f, 0.0f, 0.0f }       // �����_
, worldUp_{ 0.0f, 0.0f, 0.0f }      // ������̃x�N�g��
, matProj_{ Matrix::identity() }    // ���e�s��
, fovY_{ XM_PIDIV4 }                // ��p
, aspectRatio_{ 16.0f / 9.0f }      // �A�X�y�N�g��
, zNear_{ 0.01f }                   // �߃N���b�v��
, zFar_{ 1000.0f }                  // ���N���b�v��
{
}

//---------------------------------------------------------------------------
//! �f�X�g���N�^
//---------------------------------------------------------------------------
Camera::~Camera() = default;

//---------------------------------------------------------------------------
//! �X�V
//---------------------------------------------------------------------------
void Camera::update()
{
    //-------------------------------------------------------------
    // �J�����̃��[���h�s����쐬
    //-------------------------------------------------------------
    // (1) Z��
    Vector3 axisZ = (position_ - lookAt_).normalize();

    // (2) X��
    Vector3 axisX = Vector3::cross(Vector3(0.0f, 1.0f, 0.0f), axisZ).normalize();

    // (3) Y��
    Vector3 axisY = Vector3::cross(axisZ, axisX);

    // �s��ɐݒ�
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
    // �r���[�s����쐬
    //-------------------------------------------------------------
    // �� �r���[�s�� = �J�����̃��[���h�s��̋t�s��
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
    // ���e�s��̍쐬
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
