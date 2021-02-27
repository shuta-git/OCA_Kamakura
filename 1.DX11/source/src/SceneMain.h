//===========================================================================
//!	@file	SceneMain.h
//!	@brief	���C���V�[��
//===========================================================================
#pragma once

//===========================================================================
//! ���C���V�[��
//===========================================================================
class SceneMain : public Scene
{
public:
    //! �R���X�g���N�^
    SceneMain();

    //! �f�X�g���N�^
    virtual ~SceneMain();

    //! ������
    virtual bool initialize() override;

    //! �X�V
    virtual void update() override;

    //! �`��
    virtual void render(RenderMode mode) override;

    //! ���
    virtual void finalize() override;

    //! imgui�`��
    virtual void imguiRender() override;

private:
    Matrix                         matWorld_;   //!< ���[���h�s��
    std::unique_ptr<gpu::ShaderVs> shaderVs_;   //!< ���_�V�F�[�_�[
    std::unique_ptr<gpu::ShaderPs> shaderPs_;   //!< �s�N�Z���V�F�[�_�[
    std::unique_ptr<gpu::ShaderCs> shaderCs_;   //!< ���Z�V�F�[�_�[

    gpu::Buffer bufferBoxVb_;   //!< �{�b�N�X�`��p���_
    gpu::Buffer bufferBoxIb_;   //!< �{�b�N�X�`��p�C���f�b�N�X

    ModelMQO model_;
    std::shared_ptr<gpu::ModelFBX> modelFbx_;
    std::shared_ptr<gpu::ModelFBX> modelFbx2_;

    //---- �e�N�X�`��(Shader Resource)
    std::unique_ptr<gpu::Texture> texture_;   //!< �e�N�X�`��

    std::unique_ptr<gpu::Texture> cubeEnvironment_;   //!< ���}�b�v�e�N�X�`��

    std::unique_ptr<CameraDebug> camera_;   //!< �J����

};

