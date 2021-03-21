//===========================================================================
//!	@file	SceneController.h
//!	@brief	�V�[���R���g���[���[
//===========================================================================
#pragma once

//===========================================================================
//! �V�[���R���g���[���[
//===========================================================================
class SceneController
{
public:
    //! �R���X�g���N�^
    SceneController();

    //! �f�X�g���N�^
    virtual  ~SceneController();

    //! ������
    bool initialize();

    //! �X�V
    void update();

    //! �`��
    void render();

    //! ���
    void finalize();

private:

    //! �V�[���؂�ւ�
    void switchScene();

private:

    int mode_;      //!< �V�[�����[�h
    int prevMode_;  //!< 1�t���[���O�̃V�[�����[�h

};
