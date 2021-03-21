////===========================================================================
////!	@file   ColliderResource.h
////!	@brief	コライダーリソース管理
////===========================================================================
//#include "ColliderResource.h"
//
//
//namespace {
//
////! コライダーテーブル
//std::unordered_map < int, std::vector<std::shared_ptr<gpu::Collider>>> colliderTable;
//
//}
//
//namespace gpu {
//
////---------------------------------------------------------------------------
////! タグ付けされたスフィアコライダー作成
////! @parm [in] tag タグ
////---------------------------------------------------------------------------
//std::shared_ptr<gpu::SphereCollider> createSphere(int tag)
//{
//    std::shared_ptr<gpu::SphereCollider> collider = gpu::createSphereCollider();
//    collider->setType(gpu::COLLIDER_TYPE::SPHERE);
//    // 保存
//    colliderTable[tag].push_back(collider);
//
//    return collider;
//}
//
////---------------------------------------------------------------------------
////! タグ付けされたカプセルコライダー作成
////! @parm [in] tag タグ
////---------------------------------------------------------------------------
//std::shared_ptr<gpu::CapsuleCollider> createCapsule(int tag)
//{
//    std::shared_ptr<gpu::CapsuleCollider> collider = gpu::createCapsuleCollider();
//    collider->setType(gpu::COLLIDER_TYPE::CAPSULE);
//    // 保存
//    colliderTable[tag].push_back(collider);
//
//    return collider;
//}
//
////---------------------------------------------------------------------------
////! タグ付けされたボックスコライダー作成
////! @parm [in] tag タグ
////---------------------------------------------------------------------------
//std::shared_ptr<gpu::BoxCollider> createBox(int tag)
//{
//    std::shared_ptr<gpu::BoxCollider> collider = gpu::createBoxCollider();
//    collider->setType(gpu::COLLIDER_TYPE::BOX);
//    // 保存
//    colliderTable[tag].push_back(collider);
//
//    return collider;
//}
//
////---------------------------------------------------------------------------
////! タグにあったコライダーを取得
////! @parm [in] tag タグ
////---------------------------------------------------------------------------
//std::vector<std::shared_ptr<gpu::Collider>>getCollider(int tag)
//{
//    return colliderTable[tag];
//}
//
//}   // namespace collider