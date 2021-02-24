//===========================================================================
//!	@file	PlayerBase.h
//!	@brief	�v���C���[���
//===========================================================================
#pragma once

//===========================================================================
//!	�v���C���[���
//===========================================================================
class PlayerBase : public CollisionBase
{
public:
    PlayerBase() = default;
    virtual ~PlayerBase() = default;

    //! ������
    virtual bool initialize() = 0;

    //! �X�V
    virtual void update() = 0;

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

    std::shared_ptr<gpu::BoxCollider> getBoxCollider() const { return boxCollider_; }
    std::shared_ptr<gpu::BoxCollider> getAttackCollider() const { return attackCollider_; }

    //@}

    //-------------------------------------------------------------
    //!	@name	�Q��
    //-------------------------------------------------------------
    //@{

    bool getIsAttack() { return isAttack_; }

    //@}

protected:

     //! �ړ�
    void move();

    //! ��]
    void rotate();

    //! �U��
    void attack();

    //! ��]����
    void rotateDir(Vector3 cameraDir);

protected:

    std::shared_ptr<gpu::ModelFBX> modelFbx_;   //!< ���f��FBX
    std::unique_ptr<CameraPlayer>  cameraPlayer_;   //!< �J����

    std::shared_ptr<gpu::BoxCollider> boxCollider_;       //!< �{�b�N�X�R���C�_�[
    std::shared_ptr<gpu::BoxCollider> attackCollider_;    //!< �U������p�̃R���C�_�[

    float moveSpeed_;   //!< �ړ����x

    bool isAttack_;   //!< �U�������Ă邩�ǂ���

};
