//===========================================================================
//!	@file	TitleScene.h
//!	@brief	�^�C�g���V�[��
//===========================================================================
#pragma once

//===========================================================================
//! �^�C�g���V�[��
//===========================================================================
class TitleScene : public Scene
{
public:
    //! �R���X�g���N�^
    TitleScene();

    //! �f�X�g���N�^
    virtual ~TitleScene();

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

    //! �e�L�X�g�A�j���[�V����
    void textAnimation();

private:

    std::unique_ptr<Map>          map_;      //!< �}�b�v

    std::unique_ptr<CameraTitle> cameraTitle_;   //!< �J����

    f32 textScale_;
    std::string text_;
    bool        isRender_;

    f32 time_;   //!< �o�ߎ���(s)

};
