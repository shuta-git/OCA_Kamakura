//===========================================================================
//!	@file	PlayerBase.cpp
//!	@brief	プレイヤー基底
//===========================================================================
#include"PlayerBase.h"

//---------------------------------------------------------------------------
//! 移動
//---------------------------------------------------------------------------
void PlayerBase::move()
{
    // カメラの行列
    Matrix matCameraWorld = cameraPlayer_->getWorldMatrix();

    Vector3 moveTo(0.0f, 0.0f, 0.0f);

    Vector3 forward = matCameraWorld.axisZ() * -1.0f;
    Vector3 right   = matCameraWorld.axisX();

    if(GetKeyState('W') & 0x8000) {
        moveTo += forward;
    }
    if(GetKeyState('S') & 0x8000) {
        moveTo -= forward;
    }
    if(GetKeyState('A') & 0x8000) {
        moveTo -= right;
    }
    if(GetKeyState('D') & 0x8000) {
        moveTo += right;
    }
    
    float fpsN = (float)getFPS() * 2;
    float fps  = (float)getFPS() / fpsN / 10;

    moveTo = moveTo.normalize() * moveSpeed_ * fps;

    // Y座標は0に固定
    moveTo.y_ = 0;

    pos_ += moveTo;
    modelFbx_->setPos(pos_);
}

//---------------------------------------------------------------------------
//! 回転
//---------------------------------------------------------------------------
void PlayerBase::rotate()
{
    // カメラの行列
    Matrix matCameraWorld = cameraPlayer_->getWorldMatrix();

    Vector3 forward = matCameraWorld.axisZ() * -1.0f;
    Vector3 right   = matCameraWorld.axisX();

    forward.y_ = 0.0f;
    right.y_   = 0.0f;
    rotate_    = modelFbx_->getRotate();

    if(GetKeyState('W') & 0x8000) {
        rotateDir(-forward);
    }

    if(GetKeyState('S') & 0x8000) {
        rotateDir(forward);
    }

    if(GetKeyState('D') & 0x8000) {
        rotateDir(-right);
    }

    if(GetKeyState('A') & 0x8000) {
        rotateDir(right);
    }

    modelFbx_->setRotate(rotate_);
}

//---------------------------------------------------------------------------
//! 攻撃
//---------------------------------------------------------------------------
void PlayerBase::attack()
{
    isAttack_ = false;
    if (GetKeyState(VK_LBUTTON) & 0x8000) {
        isAttack_ = true;
    }
}

//---------------------------------------------------------------------------
//! 回転方向
//! @parm [in] cameraDir カメラの向き
//---------------------------------------------------------------------------
void PlayerBase::rotateDir(Vector3 cameraDir)
{
    // プレイヤーのラジアン角をベクトルの向きに変更
    Vector3 rotateDir(0.0f, 0.0f, 1.0f);
    rotateDir = rotateDir.transformNormal(Matrix::rotateY(rotate_.y_));

    // ベクトルの長さ
    f32 lengthA = rotateDir.length();
    f32 lengthB = cameraDir.length();

    // 内積とベクトルの長さを使ってcosθを求める
    f32 cosSita = Vector3::dot(rotateDir, cameraDir) / lengthA * lengthB;

    // cosθからθを求める
    f32 theta = acosf(cosSita);

    theta = std::min(theta, radian(10.0f));

    // 外積を求めて目的の角度に近い方から回転
    Vector3 a = Vector3::cross(rotateDir, cameraDir);
    theta *= a.y_;

    rotate_.y_ += theta;
}
