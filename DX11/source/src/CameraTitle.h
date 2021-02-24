//===========================================================================
//! @file   CameraTitle.h
//!	@brief	タイトルカメラ
//===========================================================================
#pragma once

//===========================================================================
//! タイトルカメラ
//===========================================================================
class CameraTitle : public Camera
{
public:
    //! コンストラクタ
    CameraTitle();

    //! デストラクタ
    virtual ~CameraTitle() = default;

    //! 更新
    virtual void update() override;

    //-------------------------------------------------------------
    //!	@name	取得
    //-------------------------------------------------------------
    //@{

    //!< 座標
    Vector3 getPos() const { return pos_; }

    //@}

private:

    Vector3 pos_;   //!< カメラの座標

    float speed_;   //!< 回転速度
    float moveTo_;  //!< 移動量
};
