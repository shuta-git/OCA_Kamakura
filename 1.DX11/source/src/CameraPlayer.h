//===========================================================================
//! @file   CameraPlayer.h
//!	@brief	プレイヤーカメラ
//===========================================================================
#pragma once

//===========================================================================
//! プレイヤーカメラ
//===========================================================================
class CameraPlayer : public Camera
{
public:
    //! コンストラクタ
    CameraPlayer();

    //! デストラクタ
    virtual ~CameraPlayer() = default;

    //! 更新
    virtual void update() override;

    //-------------------------------------------------------------
    //!	@name	設定
    //-------------------------------------------------------------
    //@{

    void setPlayerPos(Vector3 playerPos) { playerPos_ = playerPos; }

    //@}

private:
    
    //! カーソルの移動範囲
    void coursorMoveRange(POINT cursor);

private:

    float xdiff; 
    float ydiff;

    float prevYdiff;

    int countX;
    int countY;

    Vector3 playerPos_;     //!< プレイヤーの位置
    Vector3 pos_;           //!< カメラの座標
    POINT prevCursorPos_;   //!< 1フレーム前のカーソル座標

};
