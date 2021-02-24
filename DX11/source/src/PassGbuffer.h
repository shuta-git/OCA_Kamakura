//===========================================================================
//!	@file   PassGbuffer.h
//!	@brief	Gbuffer�����p�X
//===========================================================================
#pragma once

class PassGbuffer : public Pass
{
public:
    PassGbuffer() = default;

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
    PassGbuffer(const PassGbuffer&)  = delete;
    PassGbuffer(PassGbuffer&&) = delete;
    void operator=(const PassGbuffer&) = delete;
    void operator=(PassGbuffer&&) = delete;

private:
};
