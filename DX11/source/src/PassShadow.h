//===========================================================================
//!	@file   PassShadow.h
//!	@brief	�V���h�E�����p�X
//===========================================================================
#pragma once

class PassShadow : public Pass
{
public:
    PassShadow() = default;

    //! ������
    virtual bool initialize() override;

    //! ���
    virtual void finalize() override;

    //! �J�n
    virtual void begin() override;

    //! ���s
    virtual void execute() override;

    //! �I��
    virtual void end() override;

private:
    // �R�s�[�֎~
    PassShadow(const PassShadow&)  = delete;
    PassShadow(const PassShadow&&) = delete;
    void operator=(const PassShadow&) = delete;
    void operator=(const PassShadow&&) = delete;

private:
};
