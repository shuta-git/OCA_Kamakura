//===========================================================================
//!	@file   Camera.h
//!	@brief	�J����
//===========================================================================
#pragma once

//===========================================================================
//! �J����
//===========================================================================
class Camera
{
public:
    //! �R���X�g���N�^
    Camera();

    //! �f�X�g���N�^
    virtual ~Camera();

    //! �X�V
    virtual void update();

    //-------------------------------------------------------------
    //!	@name	�ݒ�
    //-------------------------------------------------------------
    //@{

    //! �ʒu��ݒ�
    void setPosition(const Vector3& position) { position_ = position; }

    //! �����_��ݒ�
    void setLookAt(const Vector3& lookAt) { lookAt_ = lookAt; }

    //! ������̃x�N�g����ݒ�
    void setWorldUp(const Vector3& worldUp) { worldUp_ = worldUp; }

    //! ��p��ݒ�
    void setFovY(f32 fovY) { fovY_ = fovY; }

    //! �A�X�y�N�g���ݒ�
    void setAspectRatio(f32 aspectRatio) { aspectRatio_ = aspectRatio; }

    //! �߃N���b�v�ʂ�ݒ�
    void setNearZ(f32 zNear) { zNear_ = zNear; }

    //! ���N���b�v�ʂ�ݒ�
    void setFarZ(f32 zFar) { zFar_ = zFar; }

    //@}

    //-------------------------------------------------------------
    //!	@name	�擾
    //-------------------------------------------------------------

    //@{

    //! �J�����̃��[���h�s����擾
    Matrix getWorldMatrix() const { return matWorld_; }

    //! �r���[�s����擾
    Matrix getViewMatrix() const { return matView_; }

    //! ���e�s����擾
    Matrix getProjMatrix() const { return matProj_; }

	//! �ʒu���擾
    Vector3 getPosition() const { return position_; }

    //! �����_���擾
    Vector3 getLookAt() const { return lookAt_; }

    //! ������̃x�N�g�����擾
    Vector3 getWorldUp() const { return worldUp_; }

    //! ��p���擾
    f32 getFovY() const { return fovY_; }

    //! �A�X�y�N�g����擾
    f32 getAspectRatio() const { return aspectRatio_; }

    //! �߃N���b�v�ʂ��擾
    f32 getNearZ() const { return zNear_; }

    //! ���N���b�v�ʂ��擾
    f32 getFarZ() const { return zFar_; }

    //@}

private:
    Matrix  matWorld_;   //!< �J�����̃��[���h�s��
    Matrix  matView_;    //!< �r���[�s��
    Vector3 position_;   //!< �ʒu
    Vector3 lookAt_;     //!< �����_
    Vector3 worldUp_;    //!< ������̃x�N�g��

    Matrix matProj_;       //!< ���e�s��
    f32    fovY_;          //!< ��p
    f32    aspectRatio_;   //!< �A�X�y�N�g��
    f32    zNear_;         //!< �߃N���b�v��
    f32    zFar_;          //!< ���N���b�v��
};
