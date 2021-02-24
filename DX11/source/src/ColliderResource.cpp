////===========================================================================
////!	@file   ColliderResource.h
////!	@brief	�R���C�_�[���\�[�X�Ǘ�
////===========================================================================
//#include "ColliderResource.h"
//
//
//namespace {
//
////! �R���C�_�[�e�[�u��
//std::unordered_map < int, std::vector<std::shared_ptr<gpu::Collider>>> colliderTable;
//
//}
//
//namespace gpu {
//
////---------------------------------------------------------------------------
////! �^�O�t�����ꂽ�X�t�B�A�R���C�_�[�쐬
////! @parm [in] tag �^�O
////---------------------------------------------------------------------------
//std::shared_ptr<gpu::SphereCollider> createSphere(int tag)
//{
//    std::shared_ptr<gpu::SphereCollider> collider = gpu::createSphereCollider();
//    collider->setType(gpu::COLLIDER_TYPE::SPHERE);
//    // �ۑ�
//    colliderTable[tag].push_back(collider);
//
//    return collider;
//}
//
////---------------------------------------------------------------------------
////! �^�O�t�����ꂽ�J�v�Z���R���C�_�[�쐬
////! @parm [in] tag �^�O
////---------------------------------------------------------------------------
//std::shared_ptr<gpu::CapsuleCollider> createCapsule(int tag)
//{
//    std::shared_ptr<gpu::CapsuleCollider> collider = gpu::createCapsuleCollider();
//    collider->setType(gpu::COLLIDER_TYPE::CAPSULE);
//    // �ۑ�
//    colliderTable[tag].push_back(collider);
//
//    return collider;
//}
//
////---------------------------------------------------------------------------
////! �^�O�t�����ꂽ�{�b�N�X�R���C�_�[�쐬
////! @parm [in] tag �^�O
////---------------------------------------------------------------------------
//std::shared_ptr<gpu::BoxCollider> createBox(int tag)
//{
//    std::shared_ptr<gpu::BoxCollider> collider = gpu::createBoxCollider();
//    collider->setType(gpu::COLLIDER_TYPE::BOX);
//    // �ۑ�
//    colliderTable[tag].push_back(collider);
//
//    return collider;
//}
//
////---------------------------------------------------------------------------
////! �^�O�ɂ������R���C�_�[���擾
////! @parm [in] tag �^�O
////---------------------------------------------------------------------------
//std::vector<std::shared_ptr<gpu::Collider>>getCollider(int tag)
//{
//    return colliderTable[tag];
//}
//
//}   // namespace collider