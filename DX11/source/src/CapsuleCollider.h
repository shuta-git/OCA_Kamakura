//===========================================================================
//!	@file	CapsuleCollider.h
//!	@brief	�J�v�Z���R���C�_�[
//===========================================================================
#pragma once

namespace gpu {

//===========================================================================
//!�@�J�v�Z���R���C�_�[
//===========================================================================
class CapsuleCollider : public Collider
{
public:
    virtual ~CapsuleCollider() = default;

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

    //! ���a��ݒ�
    void setRadius(float radius) { radius_ = radius; }

    //! ������ݒ�
    void setHeight(float height) { height_ = height; }

    //! �����̍�����ݒ�
    void setSphereHeight(float sphereHeight) { sphereHeight_ = sphereHeight; }

    //@}

    //-------------------------------------------------------------
    //!	@name	�擾
    //-------------------------------------------------------------
    //@{

    //! ���a���擾
    float getRadius() const { return radius_; }

    //! �J�v�Z���̒��S����̕����擾
    float getHeight() const { return height_; }

    //! �����̍������擾
    float getSphereHeight() const { return sphereHeight_; }

    //! ��̔����̍��W
    Vector3 getTopPos() const { 
        Vector3 result = pos_;
        result.y_      += height_;
        return result; 
    }

    //! ��̔����̍��W
    Vector3 getUnderPos() const
    {
        Vector3 result = pos_;
        result.y_      -= height_;
        return result;
    }


    //@}

protected:

    float radius_       = 1.0f;   //!< ���a
    float height_       = 1.0f;   //!< �J�v�Z���̍���
    float sphereHeight_ = 0.5f;   //!< �����̍���

};

//! CapsuleCollider���쐬
std::shared_ptr<gpu::CapsuleCollider> createCapsuleCollider();

}   // namespace gpu