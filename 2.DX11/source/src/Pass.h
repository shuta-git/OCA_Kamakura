//===========================================================================
//!	@file   Pass.h
//!	@brief	�`��p�X
//===========================================================================
#pragma once

class Pass
{
public:
    Pass() = default;

    //! ������
    //!	@retval	true	���� (����I��)
    //!	@retval	false	���s (�G���[�I��)
    virtual bool initialize() = 0;

    //! ���
    virtual void finalize() = 0;

    //! �J�n
    virtual void begin() = 0;

    //! ���s
    virtual void execute() = 0;

    //! �I��
    virtual void end() = 0;

private:
    // �R�s�[�֎~
    Pass(const Pass&)  = delete;
    Pass(const Pass&&) = delete;
    void operator=(const Pass&) = delete;
    void operator=(const Pass&&) = delete;

private:
};
