//===========================================================================
//!	@file   PassFilterHDR.h
//!	@brief	HDR�t�B���^�[�p�X
//===========================================================================
#pragma once

class PassFilterHDR : public Pass
{
public:
    PassFilterHDR() = default;

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
    PassFilterHDR(const PassFilterHDR&)  = delete;
    PassFilterHDR(const PassFilterHDR&&) = delete;
    void operator=(const PassFilterHDR&) = delete;
    void operator=(const PassFilterHDR&&) = delete;

private:
    std::unique_ptr<gpu::ShaderVs> shaderVs_integrate_;
    std::unique_ptr<gpu::ShaderPs> shaderPs_integrate_;

    // �O���A�t�B���^�[
    FilterGlare filterGlare_;

    // �g�[���}�b�s���O�V�F�[�_�[
    std::unique_ptr<gpu::ShaderVs> shaderTonemappingVs_;   //!< ���_�V�F�[�_�[
    std::unique_ptr<gpu::ShaderPs> shaderTonemappingPs_;   //!< �s�N�Z���V�F�[�_�[




};
