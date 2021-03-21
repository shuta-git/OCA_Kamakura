//===========================================================================
//! @file   CameraPlayer.h
//!	@brief	�v���C���[�J����
//===========================================================================
#pragma once

//===========================================================================
//! �v���C���[�J����
//===========================================================================
class CameraPlayer : public Camera
{
public:
    //! �R���X�g���N�^
    CameraPlayer();

    //! �f�X�g���N�^
    virtual ~CameraPlayer() = default;

    //! �X�V
    virtual void update() override;

    //-------------------------------------------------------------
    //!	@name	�ݒ�
    //-------------------------------------------------------------
    //@{

    void setPlayerPos(Vector3 playerPos) { playerPos_ = playerPos; }

    //@}

private:
    
    //! �J�[�\���̈ړ��͈�
    void coursorMoveRange(POINT cursor);

private:

    float xdiff; 
    float ydiff;

    float prevYdiff;

    int countX;
    int countY;

    Vector3 playerPos_;     //!< �v���C���[�̈ʒu
    Vector3 pos_;           //!< �J�����̍��W
    POINT prevCursorPos_;   //!< 1�t���[���O�̃J�[�\�����W

};
