//===========================================================================
//!	@file	EnemyBase.cpp
//!	@brief	�G�l�~�[���
//===========================================================================
#include "EnemyBase.h"

//---------------------------------------------------------------------------
//! �ړ�
//---------------------------------------------------------------------------
void EnemyBase::move()
{
    rotate_ = modelFbx_->getRotate();
    // �G�l�~�[�̃��W�A���p���x�N�g���̌����ɕύX
    Vector3 rotateDir(0.0f, 0.0f, 1.0f);
    rotateDir = rotateDir.transformNormal(Matrix::rotateY(rotate_.y_));

    Vector3 moveTo(0.0f, 0.0f, 0.0f);

    moveTo += rotateDir;

    moveTo     = moveTo.normalize() * moveSpeed_ ;
    moveTo.y_ = 0;

    pos_ += moveTo;
    modelFbx_->setPos(pos_);
}

//---------------------------------------------------------------------------
//! �ڋ�
//! @parm [in] target    �^�[�Q�b�g
//---------------------------------------------------------------------------
void EnemyBase::Approach(Vector3 target)
{
    rotate_ = modelFbx_->getRotate();

    Vector3 sub;
    sub.x_ = target.x_ - pos_.x_;
    sub.z_ = target.z_ - pos_.z_;

    float rot_y;
    rot_y = atan2f(sub.z_, sub.x_);
    rotate_.y_ = atan2f(sub.z_, sub.x_);

    pos_.x_ += moveSpeed_ * cosf(rot_y) * 0.01f;
    pos_.z_ += moveSpeed_ * sinf(rot_y) * 0.01f;

    modelFbx_->setRotateY(-rotate_.y_ - radian(90.0f));
    modelFbx_->setPos(pos_);
}
