//===========================================================================
//!	@file   CameraPlayer.cpp
//!	@brief	�v���C���[�J����
//===========================================================================
#include "CameraPlayer.h"

//---------------------------------------------------------------------------
//! �R���X�g���N�^
//---------------------------------------------------------------------------
CameraPlayer::CameraPlayer()
{
    // �ʒu������
    GetCursorPos(&prevCursorPos_);

    //SetCursorPos(630,650);

    countX = 0;
    countY = 0;
}

//---------------------------------------------------------------------------
//! �X�V
//---------------------------------------------------------------------------
void CameraPlayer::update()
{
    pos_ = playerPos_;
    prevYdiff  = ydiff;

    //-------------------------------------------------------------
    // ���_��]
    //-------------------------------------------------------------
    POINT cursor;   // ���݈ʒu
    GetCursorPos(&cursor);

    xdiff = ((cursor.x + (FULL_WINDOW_W * countX)) * -0.005f);
    ydiff = (cursor.y * -0.005f);

    // �J�[�\���̈ړ��͈�
    coursorMoveRange(cursor);

    // �����x�N�g���̉�]
    float   lenght;   // �Q�_�Ԃ̋���
    lenght = 2.4f;

    pos_.x_ += lenght * sinf(xdiff);
    pos_.z_ += lenght * cosf(xdiff);
    pos_.y_ += lenght * sinf(ydiff);

    setPosition(pos_);

    prevCursorPos_ = cursor;     // ���̃t���[���̂��߂ɕۑ����Ă���

    // �e�N���X���Ă�
    Camera::update();
}

//---------------------------------------------------------------------------
//! �J�[�\���̈ړ��͈�
//---------------------------------------------------------------------------
void CameraPlayer::coursorMoveRange(POINT cursor)
{
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);

    // �����
    if(cursor.x <= 0) {
        countX--;
        SetCursorPos(FULL_WINDOW_W - 1, cursor.y);
    }
    // �E���
    if(cursor.x >= FULL_WINDOW_W - 1) {
        countX++;
        SetCursorPos(0, cursor.y);
    }

    // 570 800 
    // �㉺�̉���
    if(570 > cursor.y) {
        ydiff = prevYdiff;
    }

    if(cursor.y > 800) {
        ydiff = prevYdiff;
    }


}
