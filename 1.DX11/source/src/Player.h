//===========================================================================
//!	@file	Player.h
//!	@brief	�v���C���[
//===========================================================================
#pragma once

//===========================================================================
//!�@�v���C���[
//===========================================================================
class Player : public PlayerBase
{
public:
    //! �R���X�g���N�^
    Player();

    //! �f�X�g���N�^
    virtual ~Player();

    //! ������
    virtual bool initialize() override;

    //! �X�V
    virtual void update() override;

    //! �`��
    virtual void render() override;

    //! ���
    virtual void finalize() override;

    //! imgui�`��
    virtual void imguiRender() override;

private:

    std::unique_ptr<CameraDebug> cameraDebug_;     //!< �J����
    


};
