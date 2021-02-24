//===========================================================================
//!	@file	Base.h
//!	@brief	���
//===========================================================================
#pragma once

//===========================================================================
//!	���
//===========================================================================
class Base
{
public:
    Base()               = default;
    virtual ~Base() = default;

    //! ������
    virtual bool initialize() = 0;

    //! �X�V
    virtual void update() = 0;

    //! �`��
    virtual void render() = 0;

    //! ���
    virtual void finalize() = 0;

    //! imgui�`��
    virtual void imguiRender() = 0;

    //-------------------------------------------------------------
    //!	@name	�Q��
    //-------------------------------------------------------------
    //@{

    //! �ʒu��ݒ�
    void setPos(Vector3 pos) { pos_ = pos; }

    //! ��]��ݒ�
    void setRotate(Vector3 rotate) { rotate_ = rotate; }

    //! X����]��ݒ�
    void setRotateX(float x) { rotate_.x_ = x; }

    //! Y����]��ݒ�
    void setRotateY(float y) { rotate_.y_ = y; }

    //! Z����]��ݒ�
    void setRotateZ(float z) { rotate_.z_ = z; }

    //@}

    //-------------------------------------------------------------
    //!	@name	�擾
    //-------------------------------------------------------------
    //@{

    //! �ʒu���擾
    Vector3 getPos() const { return pos_; }

    //! ��]���擾
    Vector3 getRotate() const { return rotate_; }

    //! X����]���擾
    float getRotateX() const { return rotate_.x_; }

    //! Y����]���擾
    float getRotateY() const { return rotate_.y_; }

    //! Z����]���擾
    float getRotateZ() const { return rotate_.z_; }

    //@}


protected:

    Vector3 pos_ = {0,0,0};   //!< �ʒu
    Vector3 rotate_ = { 0,0,0};            //!< ��]�p�x(���W�A���p)

};
