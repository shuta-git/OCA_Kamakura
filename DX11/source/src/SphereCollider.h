//===========================================================================
//!	@file	SphereCollider.h
//!	@brief	�X�t�B�A�R���C�_�[
//===========================================================================
#pragma once

namespace gpu {

//===========================================================================
//!�@�X�t�B�A�R���C�_�[
//===========================================================================
class SphereCollider : public Collider
{
public:
    virtual ~SphereCollider() = default;

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

    //@}

    //-------------------------------------------------------------
    //!	@name	�擾
    //-------------------------------------------------------------
    //@{

    //! ���a�̎擾
    float getRadius() const { return radius_; }

    //@}

protected:

    float  radius_ = 1.0f;   //!< ���a

};

//! SphereCollider���쐬
std::shared_ptr<gpu::SphereCollider> createSphereCollider();

}



