//===========================================================================
//!	@file   CameraDebug.cpp
//!	@brief	�f�o�b�O�J����
//===========================================================================
#include "CameraDebug.h"

//---------------------------------------------------------------------------
//! �R���X�g���N�^
//---------------------------------------------------------------------------
CameraDebug::CameraDebug()
{
    // �ʒu������
    GetCursorPos(&prevCursorPos_);
}

//---------------------------------------------------------------------------
//! �X�V
//---------------------------------------------------------------------------
void CameraDebug::update()
{
    auto& matWorld = getWorldMatrix();

    //-------------------------------------------------------------
    // ���s�ړ�
    //-------------------------------------------------------------
    Vector3 moveTo(0.0f, 0.0f, 0.0f);

    Vector3 forward = matWorld.axisZ() * -1.0f;
    Vector3 right   = matWorld.axisX();

    if(GetKeyState('W') & 0x8000) {
        moveTo += forward * 0.2f;
    }
    if(GetKeyState('S') & 0x8000) {
        moveTo -= forward * 0.2f;
    }

    if(GetKeyState('A') & 0x8000) {
        moveTo -= right * 0.2f;
    }
    if(GetKeyState('D') & 0x8000) {
        moveTo += right * 0.2f;
    }

    if(GetKeyState(VK_SHIFT) & 0x8000) {
        moveTo *= 4.0f;
    }

    auto position = getPosition();
    auto lookAt   = getLookAt();

    position += moveTo;
    lookAt += moveTo;

    //-------------------------------------------------------------
    // ���_��]
    //-------------------------------------------------------------
    POINT cursor;   // ���݈ʒu
    GetCursorPos(&cursor);

    // �}�E�X�J�[�\���̈ړ������l
    float xdiff = (cursor.x - prevCursorPos_.x) * -0.004f;
    float ydiff = (cursor.y - prevCursorPos_.y) * -0.004f;

    // �����x�N�g���̉�]
    if(GetKeyState(VK_RBUTTON) & 0x8000) {

        Vector3 viewDir = lookAt - position;
        viewDir = viewDir.transformNormal(Matrix::rotateY(xdiff));

        // �㉺��U��F�J�����̉E�����̃x�N�g�����S�ɉ�]
        Vector3 axisX = matWorld.axisX();
        viewDir       = viewDir.transformNormal(Matrix::rotateAxis(axisX, ydiff));

        lookAt = position + viewDir;
    }

    setPosition(position);
    setLookAt(lookAt);

    prevCursorPos_ = cursor;   // ���̃t���[���̂��߂ɕۑ����Ă���

    // �e�N���X���Ă�
    Camera::update();
}
