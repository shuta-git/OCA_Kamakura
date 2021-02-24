//===========================================================================
//!	@file   PassLighting.h
//!	@brief	�Ɩ��p�X
//===========================================================================
#pragma once

class PassLighting : public Pass
{
public:
    PassLighting() = default;

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
    PassLighting(const PassLighting&)  = delete;
    PassLighting(const PassLighting&&) = delete;
    void operator=(const PassLighting&) = delete;
    void operator=(const PassLighting&&) = delete;

private:
    std::unique_ptr<gpu::ShaderVs> shaderVs_;
    std::unique_ptr<gpu::ShaderPs> shaderPs_;
};
