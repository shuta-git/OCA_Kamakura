//===========================================================================
//!	@file	Collider.h
//!	@brief	�R���C�_�[���
//===========================================================================
#pragma once

namespace gpu {

//===========================================================================
//!�@�R���C�_�[���
//===========================================================================
class Collider : public Base
{
public:

    virtual ~Collider() = default;

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
    //!	@name	�Q��
    //-------------------------------------------------------------
    //@{

    void setTrigger(bool isTrigger) { isTrigger_ = isTrigger; }

    //@}

    bool getTrigger() { return isTrigger_; }

protected:

    bool isTrigger_;

};

}   // namespace gpu