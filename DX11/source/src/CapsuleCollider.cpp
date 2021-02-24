//===========================================================================
//!	@file	CapsuleCollider.cpp
//!	@brief	�J�v�Z���R���C�_�[
//===========================================================================
#include "CapsuleCollider.h"

namespace gpu {

//===========================================================================
//! ���R���C�_�[������
//===========================================================================
class CapsuleColliderImpl final : public gpu::CapsuleCollider
{
public:
    //! �R���X�g���N�^
    CapsuleColliderImpl()
    {
    }

    //! �f�X�g���N�^
    virtual ~CapsuleColliderImpl()
    {
    }

    //! ������
    virtual bool initialize() override;

    //! �X�V
    virtual void update() override;

    //! �`��
    virtual void render() override;

    //! ���
    virtual void finalize() override;

    //! imgui�`��
    virtual void imguiRender() override;

private:
    // �R�s�[�֎~/move�֎~
    CapsuleColliderImpl(const CapsuleColliderImpl&) = delete;
    CapsuleColliderImpl(CapsuleColliderImpl&&)      = delete;
    CapsuleColliderImpl& operator=(const CapsuleColliderImpl&) = delete;
    CapsuleColliderImpl& operator=(CapsuleColliderImpl&&) = delete;

private:
    
    //! �R���C�_�[�쐬
    void createCollider();

private:

    Matrix matWorld;   //!< ���[���h�s��

    //-------------------------------------------------------------
    //!	@name	�f�[�^
    //-------------------------------------------------------------
    //@{

    const int u_max = 15;   //!< �c���̐�
    const int v_max = 15;   //!< �����̐�

    //@}

};

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool CapsuleColliderImpl::initialize()
{
    return false;
}

//---------------------------------------------------------------------------
//! �X�V
//---------------------------------------------------------------------------
void CapsuleColliderImpl::update()
{
}

//---------------------------------------------------------------------------
//! �`��
//---------------------------------------------------------------------------
void CapsuleColliderImpl::render()
{
    matWorld = Matrix::rotateX(rotate_.x_) *
               Matrix::rotateY(rotate_.y_) *
               Matrix::rotateZ(rotate_.z_);

    matWorld = matWorld * Matrix::translate(pos_);
    // �s��ݒ�
    dxLoadMatrixf(matWorld);

    if(!isTrigger_) {
        dxColor4ub(60, 255, 60, 255);
    }
    else {
        dxColor4ub(255, 60, 60, 255);
    }
    dxBegin(PT_LINES);
    {
        createCollider();
    }
    dxEnd();

    // ���ɖ߂�
    dxLoadMatrixf(Matrix::identity());
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
void CapsuleColliderImpl::finalize()
{
}

//---------------------------------------------------------------------------
//! imgui�`��
//---------------------------------------------------------------------------
void CapsuleColliderImpl::imguiRender()
{
}

//---------------------------------------------------------------------------
//! �R���C�_�[�쐬
//---------------------------------------------------------------------------
void CapsuleColliderImpl::createCollider()
{
    //-------------------------------------------------------------
    // �~���̒��_�f�[�^�쐬
    //-------------------------------------------------------------
    // �c��
    for(int u = 0; u < u_max; u++) {
        for(int v = 0; v <= v_max; v++) {
            double theta  = XMConvertToRadians(180.0f * v / v_max);
            double dTheta = 2.0f * PI / u_max;

            float x = cos(u * dTheta) * radius_;
            float y = cos(theta) * height_;
            float z = sin(u * dTheta) * radius_;

            dxVertex3f(x, y, z);

            theta = XMConvertToRadians(180.0f * (v + 1) / v_max);

            x = cos(u * dTheta) * radius_;
            y = cos(theta) * height_;
            z = sin(u * dTheta) * radius_;

            dxVertex3f(x, y, z);
        }
    }
    // ����
    for(int v = 0; v <= v_max; v++) {
        for(int u = 0; u < u_max; u++) {
            double theta  = XMConvertToRadians(180.0f * v / v_max);
            double dTheta = 2.0f * PI / u_max;

            float x = cos(u * dTheta) * radius_;
            float y = cos(theta) * height_;
            float z = sin(u * dTheta) * radius_;

            dxVertex3f(x, y, z);

            x = cos((u + 1) * dTheta) * radius_;
            y = cos(theta) * height_;
            z = sin((u + 1) * dTheta) * radius_;

            dxVertex3f(x, y, z);
        }
    }

    //-------------------------------------------------------------
    // ��̔����̒��_�f�[�^�[�쐬
    //-------------------------------------------------------------
    // �c��
    for(int u = 0; u < u_max; u++) {
        for(int v = 0; v <= v_max; v++) {
            double theta = XMConvertToRadians(90.0f * v / v_max);
            double phi   = XMConvertToRadians(360.0f * u / u_max);

            double x = sin(theta) * cos(phi) * radius_;
            double y = cos(theta) * sphereHeight_ + height_;
            double z = sin(theta) * sin(phi) * radius_;

            dxVertex3f(x, y, z);

            theta = XMConvertToRadians(90.0f * (v + 1) / v_max);

            x = sin(theta) * cos(phi) * radius_;
            y = cos(theta) * sphereHeight_ + height_;
            z = sin(theta) * sin(phi) * radius_;

            dxVertex3f(x, y, z);
        }
    }
    // ����
    for(int v = 0; v <= v_max; v++) {
        for(int u = 0; u < u_max; u++) {
            double theta = XMConvertToRadians(90.0f * v / v_max);
            double phi   = XMConvertToRadians(360.0f * u / u_max);

            double x = sin(theta) * cos(phi) * radius_;
            double y = cos(theta) * sphereHeight_ + height_;
            double z = sin(theta) * sin(phi) * radius_;

            dxVertex3f(x, y, z);

            phi = XMConvertToRadians(360.0f * (u + 1) / u_max);

            x = sin(theta) * cos(phi) * radius_;
            y = cos(theta) * sphereHeight_ + height_;
            z = sin(theta) * sin(phi) * radius_;

            dxVertex3f(x, y, z);
        }
    }

    //-------------------------------------------------------------
    // ���̔����̒��_�f�[�^�[�쐬
    //-------------------------------------------------------------
    // �c��
    for(int u = 0; u < u_max; u++) {
        for(int v = 0; v <= v_max; v++) {
            double theta = XMConvertToRadians(90.0f * v / v_max);
            double phi   = XMConvertToRadians(360.0f * u / u_max);
            double x     = sin(theta) * cos(phi) * radius_;
            double y     = cos(theta) * -sphereHeight_ - height_;
            double z     = sin(theta) * sin(phi) * radius_;

            dxVertex3f(x, y, z);

            theta = XMConvertToRadians(90.0f * (v + 1) / v_max);

            x = sin(theta) * cos(phi) * radius_;
            y = cos(theta) * -sphereHeight_ - height_;
            z = sin(theta) * sin(phi) * radius_;

            dxVertex3f(x, y, z);
        }
    }
    // ����
    for(int v = 0; v <= v_max; v++) {
        for(int u = 0; u < u_max; u++) {
            double theta = XMConvertToRadians(90.0f * v / v_max);
            double phi   = XMConvertToRadians(360.0f * u / u_max);
            double x     = sin(theta) * cos(phi) * radius_;
            double y     = cos(theta) * -sphereHeight_ - height_;
            double z     = sin(theta) * sin(phi) * radius_;

            dxVertex3f(x, y, z);

            phi = XMConvertToRadians(360.0f * (u + 1) / u_max);

            x = sin(theta) * cos(phi) * radius_;
            y = cos(theta) * -sphereHeight_ - height_;
            z = sin(theta) * sin(phi) * radius_;

            dxVertex3f(x, y, z);
        }
    }
}

//---------------------------------------------------------------------------
//! CapsuleCollider���쐬
//---------------------------------------------------------------------------
std::shared_ptr<gpu::CapsuleCollider> createCapsuleCollider()
{
    //---------------------------------------------------------------------
    // CapsuleCollider�̏�����
    //---------------------------------------------------------------------
    auto p = std::make_shared<gpu::CapsuleColliderImpl>();

    if(!p) {
        p.reset();
    }

    return p;
}

}   // namespace gpu