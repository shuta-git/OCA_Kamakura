//===========================================================================
//!	@file   CameraPlayer.cpp
//!	@brief	プレイヤーカメラ
//===========================================================================
#include "CameraPlayer.h"

//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
CameraPlayer::CameraPlayer()
{
    // 位置初期化
    GetCursorPos(&prevCursorPos_);

    //SetCursorPos(630,650);

    countX = 0;
    countY = 0;
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void CameraPlayer::update()
{
    pos_ = playerPos_;
    prevYdiff  = ydiff;

    //-------------------------------------------------------------
    // 視点回転
    //-------------------------------------------------------------
    POINT cursor;   // 現在位置
    GetCursorPos(&cursor);

    xdiff = ((cursor.x + (FULL_WINDOW_W * countX)) * -0.005f);
    ydiff = (cursor.y * -0.005f);

    // カーソルの移動範囲
    coursorMoveRange(cursor);

    // 視線ベクトルの回転
    float   lenght;   // ２点間の距離
    lenght = 2.4f;

    pos_.x_ += lenght * sinf(xdiff);
    pos_.z_ += lenght * cosf(xdiff);
    pos_.y_ += lenght * sinf(ydiff);

    setPosition(pos_);

    prevCursorPos_ = cursor;     // 次のフレームのために保存しておく

    // 親クラスを呼ぶ
    Camera::update();
}

//---------------------------------------------------------------------------
//! カーソルの移動範囲
//---------------------------------------------------------------------------
void CameraPlayer::coursorMoveRange(POINT cursor)
{
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);

    // 左画面
    if(cursor.x <= 0) {
        countX--;
        SetCursorPos(FULL_WINDOW_W - 1, cursor.y);
    }
    // 右画面
    if(cursor.x >= FULL_WINDOW_W - 1) {
        countX++;
        SetCursorPos(0, cursor.y);
    }

    // 570 800 
    // 上下の可動域
    if(570 > cursor.y) {
        ydiff = prevYdiff;
    }

    if(cursor.y > 800) {
        ydiff = prevYdiff;
    }


}
