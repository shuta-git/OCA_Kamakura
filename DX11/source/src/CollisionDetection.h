//===========================================================================
//!	@file	CollisionDetection.h
//!	@brief	当たり判定
//===========================================================================
#pragma once

//! 球 vs 球
//! @parm [in] s1 スフィアコライダー
//! @parm [in] s2 スフィアコライダー
bool isHit(const std::shared_ptr<gpu::SphereCollider> s1, const std::shared_ptr<gpu::SphereCollider> s2);

//! カプセル vs 球
//! @parm [in] c1 カプセルコライダー
//! @parm [in] s1 スフィアコライダー
bool isHit(const std::shared_ptr<gpu::CapsuleCollider> c1, const std::shared_ptr<gpu::SphereCollider> s1);

//! カプセル vs カプセル
//! @parm [in] c1 カプセルコライダー
//! @parm [in] c2 カプセルコライダー
bool isHit(const std::shared_ptr<gpu::CapsuleCollider> c1, const std::shared_ptr<gpu::CapsuleCollider> c2);

//! ボックス vs　ボックス 
//! @parm [in] b1 ボックスコライダー
//! @parm [in] b2 ボックスコライダー
bool isHit(const std::shared_ptr<gpu::BoxCollider>& b1, const std::shared_ptr<gpu::BoxCollider>& b2);

//! AABB vs AABB の計算
//! @parm [in] m   自分のAABB
//! @parm [in] t   ターゲットのAABB
//! @parm [in] pos 自分の座標
Vector3 calculation(const AABB& m, const AABB& t, const Vector3& pos);
