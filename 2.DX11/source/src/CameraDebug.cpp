//===========================================================================
//!	@file   CameraDebug.cpp
//!	@brief	デバッグカメラ
//===========================================================================
#include "CameraDebug.h"

//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
CameraDebug::CameraDebug()
{
    // 位置初期化
    GetCursorPos(&prevCursorPos_);
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void CameraDebug::update()
{
    auto& matWorld = getWorldMatrix();

    //-------------------------------------------------------------
    // 平行移動
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
    // 視点回転
    //-------------------------------------------------------------
    POINT cursor;   // 現在位置
    GetCursorPos(&cursor);

    // マウスカーソルの移動差分値
    float xdiff = (cursor.x - prevCursorPos_.x) * -0.004f;
    float ydiff = (cursor.y - prevCursorPos_.y) * -0.004f;

    // 視線ベクトルの回転
    if(GetKeyState(VK_RBUTTON) & 0x8000) {

        Vector3 viewDir = lookAt - position;
        viewDir = viewDir.transformNormal(Matrix::rotateY(xdiff));

        // 上下首振り：カメラの右方向のベクトル中心に回転
        Vector3 axisX = matWorld.axisX();
        viewDir       = viewDir.transformNormal(Matrix::rotateAxis(axisX, ydiff));

        lookAt = position + viewDir;
    }

    setPosition(position);
    setLookAt(lookAt);

    prevCursorPos_ = cursor;   // 次のフレームのために保存しておく

    // 親クラスを呼ぶ
    Camera::update();
}
