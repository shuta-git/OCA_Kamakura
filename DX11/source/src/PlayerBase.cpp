//===========================================================================
//!	@file	PlayerBase.cpp
//!	@brief	�v���C���[���
//===========================================================================
#include"PlayerBase.h"

//---------------------------------------------------------------------------
//! �ړ�
//---------------------------------------------------------------------------
void PlayerBase::move()
{
    // �J�����̍s��
    Matrix matCameraWorld = cameraPlayer_->getWorldMatrix();

    Vector3 moveTo(0.0f, 0.0f, 0.0f);

    Vector3 forward = matCameraWorld.axisZ() * -1.0f;
    Vector3 right   = matCameraWorld.axisX();

    if(GetKeyState('W') & 0x8000) {
        moveTo += forward;
    }
    if(GetKeyState('S') & 0x8000) {
        moveTo -= forward;
    }
    if(GetKeyState('A') & 0x8000) {
        moveTo -= right;
    }
    if(GetKeyState('D') & 0x8000) {
        moveTo += right;
    }
    
    float fpsN = (float)getFPS() * 2;
    float fps  = (float)getFPS() / fpsN / 10;

    moveTo = moveTo.normalize() * moveSpeed_ * fps;

    // Y���W��0�ɌŒ�
    moveTo.y_ = 0;

    pos_ += moveTo;
    modelFbx_->setPos(pos_);
}

//---------------------------------------------------------------------------
//! ��]
//---------------------------------------------------------------------------
void PlayerBase::rotate()
{
    // �J�����̍s��
    Matrix matCameraWorld = cameraPlayer_->getWorldMatrix();

    Vector3 forward = matCameraWorld.axisZ() * -1.0f;
    Vector3 right   = matCameraWorld.axisX();

    forward.y_ = 0.0f;
    right.y_   = 0.0f;
    rotate_    = modelFbx_->getRotate();

    if(GetKeyState('W') & 0x8000) {
        rotateDir(-forward);
    }

    if(GetKeyState('S') & 0x8000) {
        rotateDir(forward);
    }

    if(GetKeyState('D') & 0x8000) {
        rotateDir(-right);
    }

    if(GetKeyState('A') & 0x8000) {
        rotateDir(right);
    }

    modelFbx_->setRotate(rotate_);
}

//---------------------------------------------------------------------------
//! �U��
//---------------------------------------------------------------------------
void PlayerBase::attack()
{
    isAttack_ = false;
    if (GetKeyState(VK_LBUTTON) & 0x8000) {
        isAttack_ = true;
    }
}

//---------------------------------------------------------------------------
//! ��]����
//! @parm [in] cameraDir �J�����̌���
//---------------------------------------------------------------------------
void PlayerBase::rotateDir(Vector3 cameraDir)
{
    // �v���C���[�̃��W�A���p���x�N�g���̌����ɕύX
    Vector3 rotateDir(0.0f, 0.0f, 1.0f);
    rotateDir = rotateDir.transformNormal(Matrix::rotateY(rotate_.y_));

    // �x�N�g���̒���
    f32 lengthA = rotateDir.length();
    f32 lengthB = cameraDir.length();

    // ���ςƃx�N�g���̒������g����cos�Ƃ����߂�
    f32 cosSita = Vector3::dot(rotateDir, cameraDir) / lengthA * lengthB;

    // cos�Ƃ���Ƃ����߂�
    f32 theta = acosf(cosSita);

    theta = std::min(theta, radian(10.0f));

    // �O�ς����߂ĖړI�̊p�x�ɋ߂��������]
    Vector3 a = Vector3::cross(rotateDir, cameraDir);
    theta *= a.y_;

    rotate_.y_ += theta;
}
