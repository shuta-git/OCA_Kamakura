//===========================================================================
//!	@file	BoxCollider.h
//!	@brief	�{�b�N�X�R���C�_�[
//===========================================================================
#pragma once

struct AABB
{
    Vector3 max_;
    Vector3 min_;
};

namespace gpu {

//===========================================================================
//!�@�{�b�N�X�R���C�_�[
//===========================================================================
class BoxCollider : public Collider
{
public:
    
    virtual ~BoxCollider() = default;

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

    //! ������ݒ�
    void setHeight(float height) { height_ = height; }

    //! ����ݒ�
    void setWigth(float wigth) { wigth_ = wigth; }

    //! ���s����ݒ�
    void setDepth(float depth) { depth_ = depth; }

    //! AABB��ݒ�
    void setAABB(AABB box) { box_ = box; }

    //@}

    //-------------------------------------------------------------
    //!	@name	�擾
    //-------------------------------------------------------------
    //@{

    //! �������擾
    float getHeight() const { return height_; }

    //! �����擾
    float getWigth() const { return wigth_; }

    //! ���s�����擾
    float getDepth() const { return depth_; }

    //! AABB���擾
    AABB getAABB() const { 
        AABB result = box_;
        result.min_ = pos_;
        result.max_ = pos_;
        result.max_.x_ += wigth_;
        result.max_.y_ += height_;
        result.max_.z_ += depth_;
        return result; 
    }

    //@}

protected:

    AABB box_;

    float height_ = 1.0f;
    float wigth_  = 1.0f;
    float depth_  = 1.0f;

};

//! SphereCollider���쐬
std::shared_ptr<gpu::BoxCollider> createBoxCollider();



}   // namespace gpu