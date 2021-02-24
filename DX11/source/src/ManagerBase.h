//===========================================================================
//!	@file	ManagerBase.h
//!	@brief	�}�l�[�W���[���
//===========================================================================
#pragma once

//===========================================================================
//! �}�l�W���[���
//===========================================================================
class ManagerBase
{
public:
    ManagerBase()   = default;
    virtual ~ManagerBase() = default;

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

};
