//===========================================================================
//!	@file	EnemyBase.h
//!	@brief	�G�l�~�[���
//===========================================================================
#pragma once

//===========================================================================
//!	�G�l�~�[���
//===========================================================================
class EnemyBase : public CollisionBase
{
public:
    EnemyBase()           = default;
    virtual ~EnemyBase() = default;

    //! ������
    virtual bool initialize() = 0;

    //! �X�V
    virtual void update() = 0;

    //! �X�V
    virtual void update(Vector3 playerPos)  = 0;

    //! �`��
    virtual void render() = 0;

    //! ���
    virtual void finalize() = 0;

    //! imgui�`��
    virtual void imguiRender() = 0;

    //-------------------------------------------------------------
    //!	@name	�擾
    //-------------------------------------------------------------
    //@{

    std::shared_ptr<gpu::BoxCollider>     getBoxCollider() { return boxCollider_; }
    
    //@}

protected:

    //! �ړ�
    void move();

    //! �ڋ�
    //! @parm [in] target    �^�[�Q�b�g
    void Approach(Vector3 target);

protected:

    std::shared_ptr<gpu::ModelFBX> modelFbx_;   //!< ���f��FBX
    std::shared_ptr<gpu::BoxCollider>     boxCollider_;       //!< �{�b�N�X�R���C�_�[

    float moveSpeed_;   //!< �ړ����x

};
