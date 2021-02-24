//===========================================================================
//!	@file	CollisionBase.h
//!	@brief	�R���W�������
//===========================================================================

#pragma once

 enum class STATE
{
    ACTIVE,      //!< �A�N�e�B�u
    PAUSED,      //!< ��~
    COLLISION,   //!< �Փ�
    DEAD         //!< ���S
};


class CollisionBase : public Base
{
public:
    virtual ~CollisionBase() = default;

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

public:
    //-------------------------------------------------------------
    //!	@name	�Q��
    //-------------------------------------------------------------
    //@{

    //! ��Ԃ�ݒ�
    void setState(STATE state) { state_ = state; }

    //@)

    //-------------------------------------------------------------
    //!	@name	�擾
    //-------------------------------------------------------------
    //@{

    //! ��Ԃ��擾
    STATE getState() const { return state_; }

    //@)

private:

    STATE state_;   //!< ��ԊǗ�

public:

    bool colliderRender = false; //!< �{�b�N�X�R���C�_�[��`�悷�邩�ǂ���



};
