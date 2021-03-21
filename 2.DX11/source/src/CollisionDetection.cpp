//===========================================================================
//!	@file	CollisionDetection.cpp
//!	@brief	�����蔻��
//===========================================================================
#pragma once
#include"CollisionDetection.h"

//---------------------------------------------------------------------------
//! �� vs ��
//! @parm [in] s1 �X�t�B�A�R���C�_�[
//! @parm [in] s2 �X�t�B�A�R���C�_�[
//---------------------------------------------------------------------------
bool isHit(const std::shared_ptr<gpu::SphereCollider> s1, const std::shared_ptr<gpu::SphereCollider> s2)
{
    Vector3 v1 = s1->getPos();
    Vector3 v2 = s2->getPos();

    float L = v1.sqrtf(v1, v2);
    float R = (s1->getRadius() + s2->getRadius()) * 2;

    if(L <= R) {
        return true;
    }

    return false;
}

//---------------------------------------------------------------------------
//! �J�v�Z�� vs ��
//! @parm [in] c1 �J�v�Z���R���C�_�[
//! @parm [in] s1 �X�t�B�A�R���C�_�[
//---------------------------------------------------------------------------
bool isHit(const std::shared_ptr<gpu::CapsuleCollider> c1, const std::shared_ptr<gpu::SphereCollider> s1)
{
    Vector3 v1 = c1->getPos();
    Vector3 v2 = s1->getPos();

    float L = v1.sqrtf(v1, v2);
    float R = (c1->getRadius() + s1->getRadius()) * 2;

    if(L <= R) {
        return true;
    }

    return false;
}

//---------------------------------------------------------------------------
//! �J�v�Z�� vs �J�v�Z��
//! @parm [in] c1 �J�v�Z���R���C�_�[
//! @parm [in] c2 �J�v�Z���R���C�_�[
//---------------------------------------------------------------------------
bool isHit(const std::shared_ptr<gpu::CapsuleCollider> c1, const std::shared_ptr<gpu::CapsuleCollider> c2)
{
    Vector3 v1 = c1->getPos();
    Vector3 v2 = c2->getPos();

    float L = v1.sqrtf(v1, v2);
    float R = (c1->getRadius() + c2->getRadius()) * 2;

    if(L <= R) {
        return true;
    }

    return false;
}

//---------------------------------------------------------------------------
//! �{�b�N�X vs�@�{�b�N�X
//! @parm [in] b1 �{�b�N�X�R���C�_�[
//! @parm [in] b2 �{�b�N�X�R���C�_�[
//---------------------------------------------------------------------------
bool isHit(const std::shared_ptr<gpu::BoxCollider>& b1, const std::shared_ptr<gpu::BoxCollider>& b2)
{
    AABB aa1 = b1->getAABB();
    AABB aa2 = b2->getAABB();

    if(aa1.min_.x_ > aa2.max_.x_)return false;
    if(aa1.max_.x_ < aa2.min_.x_)return false;
    if(aa1.min_.y_ > aa2.max_.y_)return false;
    if(aa1.max_.y_ < aa2.min_.y_)return false;
    if(aa1.min_.z_ > aa2.max_.z_)return false;
    if(aa1.max_.z_ < aa2.min_.z_)return false;

    return true;
}

//---------------------------------------------------------------------------
//! AABB vs AABB �̌v�Z
//! @parm [in] m   ������AABB
//! @parm [in] t   �^�[�Q�b�g��AABB
//! @parm [in] pos �����̍��W
//---------------------------------------------------------------------------
Vector3 calculation(const AABB& m, const AABB& t,const Vector3& p)
{
    Vector3 pos = p;
    
    //-------------------------------------------------------------
    // 2�̃{�b�N�X�̍����v�Z
    //-------------------------------------------------------------
    f32 dx1 = t.max_.x_ - m.min_.x_;
    f32 dx2 = t.min_.x_ - m.max_.x_;
    f32 dy1 = t.max_.y_ - m.min_.y_;
    f32 dy2 = t.min_.y_ - m.max_.y_;
    f32 dz1 = t.max_.z_ - m.min_.z_;
    f32 dz2 = t.min_.z_ - m.max_.z_;
    
    //-------------------------------------------------------------
    // �v�Z�����l�̒Ⴂ�ق���ۑ�
    //-------------------------------------------------------------
    f32 dx = abs(dx1) < abs(dx2) ? dx1 : dx2;
    f32 dy = abs(dy1) < abs(dy2) ? dy1 : dy2;
    f32 dz = abs(dz1) < abs(dz2) ? dz1 : dz2;
    
    //-------------------------------------------------------------
    // ��ԒႢ�Ƃ���Ɏ����̍��W�𒲐�
    //-------------------------------------------------------------
    if(abs(dx) <= abs(dy) && abs(dx) <= abs(dz)) {
        pos.x_ += dx;
    }
    else if(abs(dy) <= abs(dx) && abs(dy) <= abs(dz)) {
        pos.y_ += dy;
    }
    else {
        pos.z_ += dz;
    }
    
    return pos;
}
