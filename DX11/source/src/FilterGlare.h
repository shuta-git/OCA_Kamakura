//===========================================================================
//!	@file   FilterGlare.h
//!	@brief	�O���A�t�B���^�[
//===========================================================================
#pragma once

//===========================================================================
//! �O���A�t�B���^�[
//===========================================================================
class FilterGlare
{
public:
    //-------------------------------------------------------------
    //!	@name	������
    //-------------------------------------------------------------
    //@{

    //! �R���X�g���N�^
    FilterGlare() = default;

    //! �f�X�g���N�^
    virtual ~FilterGlare() = default;

    //@}
    //-------------------------------------------------------------
    //!	@name	�X�V
    //-------------------------------------------------------------
    //@{

    //! ������
    bool initialize();

    // �`��
    void render();

    //@}
private:
    struct CBFilterGlare
    {
        f32 stride_;
		f32	attenuation_;
		Vector4	dir_;
    };

    std::unique_ptr<gpu::ShaderVs> shaderVs_;

    std::unique_ptr<gpu::ShaderPs> shaderPs_Luminance_;       //!< �P�x���o
    std::unique_ptr<gpu::ShaderPs> shaderPs_GaussianBlurH_;   //!< �K�E�X�ڂ���(����)
    std::unique_ptr<gpu::ShaderPs> shaderPs_GaussianBlurV_;   //!< �K�E�X�ڂ���(����)
    std::unique_ptr<gpu::ShaderPs> shaderPs_Texture_;         //!< �e�N�X�`���\��
    std::unique_ptr<gpu::ShaderPs> shaderPs_;                 //!< �ŏI����

    std::unique_ptr<gpu::ShaderPs> shaderPs_Glare_;   //!< �O���A���

    gpu::ConstantBuffer<CBFilterGlare> cbFilter_;   //!< �萔�o�b�t�@
};
