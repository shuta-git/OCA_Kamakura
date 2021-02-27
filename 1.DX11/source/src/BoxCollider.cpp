//===========================================================================
//!	@file	BoxCollider.cpp
//!	@brief	�{�b�N�X�R���C�_�[
//===========================================================================
#include "BoxCollider.h"

namespace gpu {

//===========================================================================
//! �{�b�N�X�R���C�_�[������
//===========================================================================
class BoxColliderImpl final : public gpu::BoxCollider
{
public:
    //! �R���X�g���N�^
    BoxColliderImpl()
    {
        box_.min_ = Vector3(0,0,0);
        box_.max_ = Vector3(1.0f,1.0f,1.0f);
        isTrigger_ = false;
    }

    //! �f�X�g���N�^
    virtual ~BoxColliderImpl()
    {
        finalize();
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
    BoxColliderImpl(const BoxColliderImpl&) = delete;
    BoxColliderImpl(BoxColliderImpl&&)      = delete;
    BoxColliderImpl& operator=(const BoxColliderImpl&) = delete;
    BoxColliderImpl& operator=(BoxColliderImpl&&) = delete;

private:

    //! �R���C�_�[�쐬
    void createCollider();

private:

    Matrix matWorld;   //!< ���[���h�s��

    //-------------------------------------------------------------
    //!	@name	�f�[�^
    //-------------------------------------------------------------
    //@{

    int vertex_num = 24;
    int index_num  = 12;

    // @}
};

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool BoxColliderImpl::initialize()
{
    return false;
}

//---------------------------------------------------------------------------
//! �X�V
//---------------------------------------------------------------------------
void BoxColliderImpl::update()
{
}

//---------------------------------------------------------------------------
//! �`��
//---------------------------------------------------------------------------
void BoxColliderImpl::render()
{
    matWorld = Matrix::rotateX(rotate_.x_) *
               Matrix::rotateY(rotate_.y_) *
               Matrix::rotateZ(rotate_.z_);

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
void BoxColliderImpl::finalize()
{
    
}

//---------------------------------------------------------------------------
//! imgui�`��
//---------------------------------------------------------------------------
void BoxColliderImpl::imguiRender()
{
    ImGui::Checkbox("isTrigger", &isTrigger_);
    Vector3 min         = box_.min_;
    Vector3 max         = box_.max_;
    float*  pLocalMin[] = { &min.x_, &min.y_, &min.z_ };
    float*  pLocalMax[] = { &max.x_, &max.y_, &max.z_ };
    ImGui::DragFloat3("min", *pLocalMin, 0.03f, -FLT_MAX, FLT_MAX);
    ImGui::DragFloat3("max", *pLocalMax, 0.03f, -FLT_MAX, FLT_MAX);
    setAABB({ min, max });
    
    float* pos[]  = { &pos_.x_, &pos_.y_, &pos_.z_ };
    float  wight  = wigth_;
    float  height = height_;
    float  depth  = depth_;
    ImGui::SliderFloat3("pos", *pos, -12.0f, 12.0f);
    ImGui::SliderFloat("wigth", &wight, -16.0f, 16.0f);
    ImGui::SliderFloat("height", &height, -16.0f, 16.0f);
    ImGui::SliderFloat("depth", &depth, -16.0f, 16.0f);
    
    setPos(Vector3(*pos[0], *pos[1], *pos[2]));
    setWigth(wight);
    setHeight(height);
    setDepth(depth);
    
}

//============================================================================================================
// ��������
//============================================================================================================
//---------------------------------------------------------------------------
//! �R���C�_�[�쐬
//---------------------------------------------------------------------------
void BoxColliderImpl::createCollider()
{
    box_.min_.x_ = pos_.x_;
    box_.min_.y_ = pos_.y_;
    box_.min_.z_ = pos_.z_;
    
    box_.max_.x_ = pos_.x_ + wigth_;
    box_.max_.y_ = pos_.y_ + height_;
    box_.max_.z_ = pos_.z_ + depth_;

    // ����
    // ��
    dxVertex3f(box_.min_.x_, box_.max_.y_, box_.min_.z_);
    dxVertex3f(box_.max_.x_, box_.max_.y_, box_.min_.z_);
    // ��
    dxVertex3f(box_.min_.x_, box_.min_.y_, box_.min_.z_);
    dxVertex3f(box_.max_.x_, box_.min_.y_, box_.min_.z_);
    // �E
    dxVertex3f(box_.max_.x_, box_.min_.y_, box_.min_.z_);
    dxVertex3f(box_.max_.x_, box_.max_.y_, box_.min_.z_);
    // ��
    dxVertex3f(box_.min_.x_, box_.min_.y_, box_.min_.z_);
    dxVertex3f(box_.min_.x_, box_.max_.y_, box_.min_.z_);

    // �w��
    // ��
    dxVertex3f(box_.min_.x_, box_.max_.y_, box_.max_.z_);
    dxVertex3f(box_.max_.x_, box_.max_.y_, box_.max_.z_);
    // ��
    dxVertex3f(box_.min_.x_, box_.min_.y_, box_.max_.z_);
    dxVertex3f(box_.max_.x_, box_.min_.y_, box_.max_.z_);
    // �E
    dxVertex3f(box_.max_.x_, box_.min_.y_, box_.max_.z_);
    dxVertex3f(box_.max_.x_, box_.max_.y_, box_.max_.z_);
    // ��
    dxVertex3f(box_.min_.x_, box_.min_.y_, box_.max_.z_);
    dxVertex3f(box_.min_.x_, box_.max_.y_, box_.max_.z_);

    // ���ʁi���ʂƔw�ʂ��Ȃ���S�ӂ����j
    // ����
    dxVertex3f(box_.min_.x_, box_.max_.y_, box_.min_.z_);
    dxVertex3f(box_.min_.x_, box_.max_.y_, box_.max_.z_);
    // ����
    dxVertex3f(box_.min_.x_, box_.min_.y_, box_.min_.z_);
    dxVertex3f(box_.min_.x_, box_.min_.y_, box_.max_.z_);
    // �E��
    dxVertex3f(box_.max_.x_, box_.max_.y_, box_.min_.z_);
    dxVertex3f(box_.max_.x_, box_.max_.y_, box_.max_.z_);
    // �E��
    dxVertex3f(box_.max_.x_, box_.min_.y_, box_.min_.z_);
    dxVertex3f(box_.max_.x_, box_.min_.y_, box_.max_.z_);

    // �Ίp��
    // ����
    // �E�ォ�獶��
    dxVertex3f(box_.min_.x_, box_.max_.y_, box_.min_.z_);
    dxVertex3f(box_.max_.x_, box_.min_.y_, box_.min_.z_);
    // ���ォ��E��
    dxVertex3f(box_.min_.x_, box_.min_.y_, box_.min_.z_);
    dxVertex3f(box_.max_.x_, box_.max_.y_, box_.min_.z_);

    // �w��
    // �E�ォ�獶��
    dxVertex3f(box_.min_.x_, box_.max_.y_, box_.max_.z_);
    dxVertex3f(box_.max_.x_, box_.min_.y_, box_.max_.z_);
    // ���ォ��E��
    dxVertex3f(box_.min_.x_, box_.min_.y_, box_.max_.z_);
    dxVertex3f(box_.max_.x_, box_.max_.y_, box_.max_.z_);
    
    // ���
    // �E�ォ�獶��
    dxVertex3f(box_.max_.x_, box_.max_.y_, box_.max_.z_);
    dxVertex3f(box_.min_.x_, box_.max_.y_, box_.min_.z_);
    // ���ォ��E��
    dxVertex3f(box_.min_.x_, box_.max_.y_, box_.max_.z_);
    dxVertex3f(box_.max_.x_, box_.max_.y_, box_.min_.z_);
   
    // ���
    // �E�ォ�獶��
    dxVertex3f(box_.max_.x_, box_.min_.y_, box_.min_.z_);
    dxVertex3f(box_.max_.x_, box_.min_.y_, box_.max_.z_);
    // ���ォ��E��
    dxVertex3f(box_.max_.x_, box_.min_.y_, box_.min_.z_);
    dxVertex3f(box_.min_.x_, box_.min_.y_, box_.max_.z_);

    // ������
    // �E�ォ�獶��
    dxVertex3f(box_.min_.x_, box_.max_.y_, box_.min_.z_);
    dxVertex3f(box_.min_.x_, box_.min_.y_, box_.max_.z_);
    // ���ォ��E��
    dxVertex3f(box_.min_.x_, box_.max_.y_, box_.max_.z_);
    dxVertex3f(box_.min_.x_, box_.min_.y_, box_.min_.z_);

    // �E����
    // �E�ォ�獶��
    dxVertex3f(box_.max_.x_, box_.max_.y_, box_.max_.z_);
    dxVertex3f(box_.max_.x_, box_.min_.y_, box_.min_.z_);
    // ���ォ��E��
    dxVertex3f(box_.max_.x_, box_.max_.y_, box_.min_.z_);
    dxVertex3f(box_.max_.x_, box_.min_.y_, box_.max_.z_);

}

//---------------------------------------------------------------------------
//! �{�b�N�X�R���C�_�[���쐬
//---------------------------------------------------------------------------
std::shared_ptr<gpu::BoxCollider> createBoxCollider()
{
    //---------------------------------------------------------------------
    // SphereCollider�̏�����
    //---------------------------------------------------------------------
    auto p = std::make_shared<gpu::BoxColliderImpl>();

    if(!p) {
        p.reset();
    }

    return p;
}

}   // namespace gpu