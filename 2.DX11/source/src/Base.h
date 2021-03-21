//===========================================================================
//!	@file	Base.h
//!	@brief	Šî’ê
//===========================================================================
#pragma once

//===========================================================================
//!	Šî’ê
//===========================================================================
class Base
{
public:
    Base()               = default;
    virtual ~Base() = default;

    //! ‰Šú‰»
    virtual bool initialize() = 0;

    //! XV
    virtual void update() = 0;

    //! •`‰æ
    virtual void render() = 0;

    //! ‰ğ•ú
    virtual void finalize() = 0;

    //! imgui•`‰æ
    virtual void imguiRender() = 0;

    //-------------------------------------------------------------
    //!	@name	QÆ
    //-------------------------------------------------------------
    //@{

    //! ˆÊ’u‚ğİ’è
    void setPos(Vector3 pos) { pos_ = pos; }

    //! ‰ñ“]‚ğİ’è
    void setRotate(Vector3 rotate) { rotate_ = rotate; }

    //! X²‰ñ“]‚ğİ’è
    void setRotateX(float x) { rotate_.x_ = x; }

    //! Y²‰ñ“]‚ğİ’è
    void setRotateY(float y) { rotate_.y_ = y; }

    //! Z²‰ñ“]‚ğİ’è
    void setRotateZ(float z) { rotate_.z_ = z; }

    //@}

    //-------------------------------------------------------------
    //!	@name	æ“¾
    //-------------------------------------------------------------
    //@{

    //! ˆÊ’u‚ğæ“¾
    Vector3 getPos() const { return pos_; }

    //! ‰ñ“]‚ğæ“¾
    Vector3 getRotate() const { return rotate_; }

    //! X²‰ñ“]‚ğæ“¾
    float getRotateX() const { return rotate_.x_; }

    //! Y²‰ñ“]‚ğæ“¾
    float getRotateY() const { return rotate_.y_; }

    //! Z²‰ñ“]‚ğæ“¾
    float getRotateZ() const { return rotate_.z_; }

    //@}


protected:

    Vector3 pos_ = {0,0,0};   //!< ˆÊ’u
    Vector3 rotate_ = { 0,0,0};            //!< ‰ñ“]Šp“x(ƒ‰ƒWƒAƒ“Šp)

};
