
//===========================================================================
//!	@file	LoadScene.h
//!	@brief	���[�h�V�[��
//===========================================================================
#pragma once

//===========================================================================
//! �^�C�g���V�[��
//===========================================================================
class LoadScene : public Scene
{
public:
    //! �R���X�g���N�^
    LoadScene();

    //! �f�X�g���N�^
    virtual ~LoadScene();

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
    //! �t�@�C���ǂݍ���
    void loadFile();

private:

    std::unique_ptr<gpu::Texture> texture_;

    f32 time_;//!< ���[�h����
};
