//===========================================================================
//!	@file	CollisionDetection.h
//!	@brief	�����蔻��
//===========================================================================
#pragma once

//! �� vs ��
//! @parm [in] s1 �X�t�B�A�R���C�_�[
//! @parm [in] s2 �X�t�B�A�R���C�_�[
bool isHit(const std::shared_ptr<gpu::SphereCollider> s1, const std::shared_ptr<gpu::SphereCollider> s2);

//! �J�v�Z�� vs ��
//! @parm [in] c1 �J�v�Z���R���C�_�[
//! @parm [in] s1 �X�t�B�A�R���C�_�[
bool isHit(const std::shared_ptr<gpu::CapsuleCollider> c1, const std::shared_ptr<gpu::SphereCollider> s1);

//! �J�v�Z�� vs �J�v�Z��
//! @parm [in] c1 �J�v�Z���R���C�_�[
//! @parm [in] c2 �J�v�Z���R���C�_�[
bool isHit(const std::shared_ptr<gpu::CapsuleCollider> c1, const std::shared_ptr<gpu::CapsuleCollider> c2);

//! �{�b�N�X vs�@�{�b�N�X 
//! @parm [in] b1 �{�b�N�X�R���C�_�[
//! @parm [in] b2 �{�b�N�X�R���C�_�[
bool isHit(const std::shared_ptr<gpu::BoxCollider>& b1, const std::shared_ptr<gpu::BoxCollider>& b2);

//! AABB vs AABB �̌v�Z
//! @parm [in] m   ������AABB
//! @parm [in] t   �^�[�Q�b�g��AABB
//! @parm [in] pos �����̍��W
Vector3 calculation(const AABB& m, const AABB& t, const Vector3& pos);
