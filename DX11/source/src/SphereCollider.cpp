//===========================================================================
//!	@file	SphereCollider.cpp
//!	@brief	�X�t�B�A�R���C�_�[
//===========================================================================
#include "SphereCollider.h"

namespace gpu {

//===========================================================================
//! �X�t�B�A�R���C�_�[������
//===========================================================================
class SphereColliderImpl final : public gpu::SphereCollider
{
public:
    //! �R���X�g���N�^
    SphereColliderImpl()
    {
    }

    //! �f�X�g���N�^
    virtual ~SphereColliderImpl()
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
    SphereColliderImpl(const SphereColliderImpl&) = delete;
    SphereColliderImpl(SphereColliderImpl&&)      = delete;
    SphereColliderImpl& operator=(const SphereColliderImpl&) = delete;
    SphereColliderImpl& operator=(SphereColliderImpl&&) = delete;

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
bool SphereColliderImpl::initialize()
{
    return false;
}

//---------------------------------------------------------------------------
//! �X�V
//---------------------------------------------------------------------------
void SphereColliderImpl::update()
{
}

//---------------------------------------------------------------------------
//! �`��
//---------------------------------------------------------------------------
void SphereColliderImpl::render()
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
void SphereColliderImpl::finalize()
{
}

//---------------------------------------------------------------------------
//! imgui�`��
//---------------------------------------------------------------------------
void SphereColliderImpl::imguiRender()
{
}

//============================================================================================================
// ��������
//============================================================================================================
//---------------------------------------------------------------------------
//! �R���C�_�[�쐬
//---------------------------------------------------------------------------
void SphereColliderImpl::createCollider()
{
    // �c��
    for(int u = 0; u < u_max; u++) {
        for(int v = 0; v <= v_max; v++) {
            double theta = XMConvertToRadians(180.0f * v / v_max);
            double phi   = XMConvertToRadians(360.0f * u / u_max);

            double x = sin(theta) * cos(phi) * radius_;
            double y = cos(theta) * radius_;
            double z = sin(theta) * sin(phi) * radius_;

            dxVertex3f(x, y, z);

            theta = XMConvertToRadians(180.0f * (v + 1) / v_max);

            x = sin(theta) * cos(phi) * radius_;
            y = cos(theta) * radius_;
            z = sin(theta) * sin(phi) * radius_;

            dxVertex3f(x, y, z);
        }
    }

    // ����
    for(int v = 0; v <= v_max; v++) {
        for(int u = 0; u < u_max; u++) {
            double theta = XMConvertToRadians(180.0f * v / v_max);
            double phi   = XMConvertToRadians(360.0f * u / u_max);

            double x = sin(theta) * cos(phi) * radius_;
            double y = cos(theta) * radius_;
            double z = sin(theta) * sin(phi) * radius_;

            dxVertex3f(x, y, z);

            phi = XMConvertToRadians(360.0f * (u + 1) / u_max);

            x = sin(theta) * cos(phi) * radius_;
            y = cos(theta) * radius_;
            z = sin(theta) * sin(phi) * radius_;

            dxVertex3f(x, y, z);
        }
    }
}

//---------------------------------------------------------------------------
//! �X�t�B�A�R���C�_�[���쐬
//---------------------------------------------------------------------------
std::shared_ptr<gpu::SphereCollider> createSphereCollider()
{
    //---------------------------------------------------------------------
    // SphereCollider�̏�����
    //---------------------------------------------------------------------
    auto p = std::make_shared<gpu::SphereColliderImpl>();

    if(!p) {
        p.reset();
    }

    return p;
}

}   // namespace gpu