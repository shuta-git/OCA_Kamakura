//===========================================================================
//!	@file	Base.h
//!	@brief	îê
//===========================================================================
#pragma once

//===========================================================================
//!	îê
//===========================================================================
class Base
{
public:
    Base()               = default;
    virtual ~Base() = default;

    //! ú»
    virtual bool initialize() = 0;

    //! XV
    virtual void update() = 0;

    //! `æ
    virtual void render() = 0;

    //! ðú
    virtual void finalize() = 0;

    //! imgui`æ
    virtual void imguiRender() = 0;

    //-------------------------------------------------------------
    //!	@name	QÆ
    //-------------------------------------------------------------
    //@{

    //! ÊuðÝè
    void setPos(Vector3 pos) { pos_ = pos; }

    //! ñ]ðÝè
    void setRotate(Vector3 rotate) { rotate_ = rotate; }

    //! X²ñ]ðÝè
    void setRotateX(float x) { rotate_.x_ = x; }

    //! Y²ñ]ðÝè
    void setRotateY(float y) { rotate_.y_ = y; }

    //! Z²ñ]ðÝè
    void setRotateZ(float z) { rotate_.z_ = z; }

    //@}

    //-------------------------------------------------------------
    //!	@name	æ¾
    //-------------------------------------------------------------
    //@{

    //! Êuðæ¾
    Vector3 getPos() const { return pos_; }

    //! ñ]ðæ¾
    Vector3 getRotate() const { return rotate_; }

    //! X²ñ]ðæ¾
    float getRotateX() const { return rotate_.x_; }

    //! Y²ñ]ðæ¾
    float getRotateY() const { return rotate_.y_; }

    //! Z²ñ]ðæ¾
    float getRotateZ() const { return rotate_.z_; }

    //@}


protected:

    Vector3 pos_ = {0,0,0};   //!< Êu
    Vector3 rotate_ = { 0,0,0};            //!< ñ]px(WAp)

};
