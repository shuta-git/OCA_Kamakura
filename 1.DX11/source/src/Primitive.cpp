//===========================================================================
//!	@file   Primitive.cpp
//!	@brief	�v���~�e�B�u�`��
//===========================================================================
#include "Primitive.h"

namespace {

// ���f���P�ʂōX�V����萔�o�b�t�@
struct CBWorld
{
    Matrix matWorld_;   //!< ���[���h�s��
};

std::vector<Matrix> matrixStack_;                     //!< �s��X�^�b�N
Matrix              matWorld_ = Matrix::identity();   //!< ���݂̃��[���h�s��

gpu::Buffer*                   dynamicBuffer_;
std::unique_ptr<gpu::ShaderVs> shaderVs3D_;
std::unique_ptr<gpu::ShaderPs> shaderPsFlat_;
std::unique_ptr<gpu::ShaderPs> shaderPsTexture_;
gpu::ConstantBuffer<CBWorld>   cbWorld_;   //!< �萔�o�b�t�@(���[���h�s��)

const u32 MAX_VERTEX_COUNT = 65536 * 1024;   // �ő�o�^���_��
const u32 BUFFER_SIZE      = sizeof(Vertex) * MAX_VERTEX_COUNT;

u32            usedCount_ = 0;      // �g�p���̒��_��
u32            drawCount_;          // ���_��
Vertex         vertex_;             // ���_�̈ꎞ�̈�
PRIMITIVE_TYPE type_ = PT_POINTS;   // ���݂̃v���~�e�B�u�̎��
Vertex*        pvertex_;            // ���݃}�b�v����Ă��钸�_�̐擪

}   // namespace

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool PRIM_Initialize()
{
    // ���I���_�o�b�t�@�̏�����
    dynamicBuffer_ = new gpu::Buffer();
    if(dynamicBuffer_->initialize(BUFFER_SIZE, D3D11_USAGE_DYNAMIC, D3D11_BIND_VERTEX_BUFFER) == false) {
        return false;
    }

    // �V�F�[�_�[�̓ǂݍ���
    shaderVs3D_      = gpu::ShaderVs::create("shader/vsPrim3D.fx");
    shaderPsFlat_    = gpu::ShaderPs::create("shader/psFlat.fx");
    shaderPsTexture_ = gpu::ShaderPs::create("shader/psTexture.fx");

    return true;
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
void PRIM_Finalize()
{
    shaderVs3D_.reset();
    shaderPsFlat_.reset();
    shaderPsTexture_.reset();

    GM_DELETE(dynamicBuffer_);
}

//==============================================================
// OpenGL�݊��֐�
//==============================================================

//---------------------------------------------------------------------------
//! �s��̐ݒ�
//---------------------------------------------------------------------------
void dxLoadMatrixf(const Matrix& matWorld)
{
    auto p       = cbWorld_.begin();
    p->matWorld_ = matWorld;
    cbWorld_.end();

    // �����ɑޔ�
    matWorld_ = matWorld;

    gpu::setConstantBuffer(1, cbWorld_);
}

//---------------------------------------------------------------------------
//! �s��X�^�b�N�֑ޔ�
//---------------------------------------------------------------------------
void dxPushMatrix()
{
    matrixStack_.push_back(matWorld_);
}

//---------------------------------------------------------------------------
//! �s��X�^�b�N���畜�A
//---------------------------------------------------------------------------
void dxPopMatrix()
{
    matWorld_ = matrixStack_.back();
    matrixStack_.pop_back();

    dxLoadMatrixf(matWorld_);   // �Đݒ�
}

//---------------------------------------------------------------------------
//! �`��J�n
//!	@param	[in]	type	�v���~�e�B�u�̎��
//---------------------------------------------------------------------------
void dxBegin(PRIMITIVE_TYPE type)
{
    // �������}�b�v�J�n (�o�b�t�@�̐擪�A�h���X���擾�����)
    D3D11_MAP map = D3D11_MAP_WRITE_NO_OVERWRITE;

    // ���t�߂��Ȃ�Ċm��
    if(MAX_VERTEX_COUNT - 65536 * 10 < usedCount_) {
        map        = D3D11_MAP_WRITE_DISCARD;
        usedCount_ = 0;   // �ŏ����痘�p
    }

    D3D11_MAPPED_SUBRESOURCE mapped;
    gpu::D3DContext()->Map(dynamicBuffer_->getD3DBuffer(), 0, map, 0, &mapped);

    // ���p���̐擪�A�h���X
    pvertex_ = &static_cast<Vertex*>(mapped.pData)[usedCount_];
    type_    = type;
}

//---------------------------------------------------------------------------
//! ���_�L�b�N (Kick)
//---------------------------------------------------------------------------
void dxVertex3f(f32 x, f32 y, f32 z)
{
    dxVertex3fv(Vector3(x, y, z));
}

void dxVertex3fv(const Vector3& v)
{
    // �l�̐ݒ�
    vertex_.position_.x = v.x_;
    vertex_.position_.y = v.y_;
    vertex_.position_.z = v.z_;

    // ���_�L�b�N
    *pvertex_++ = vertex_;   // �ꎞ���[�N���珑���o��
    drawCount_++;
}

//---------------------------------------------------------------------------
//! �J���[u8
//---------------------------------------------------------------------------
void dxColor4ub(u8 r, u8 g, u8 b, u8 a)
{
    vertex_.color_ = Color(r, g, b, a);
}

//---------------------------------------------------------------------------
//! �J���[f32
//---------------------------------------------------------------------------
void dxColor4f(f32 fr, f32 fg, f32 fb, f32 fa)
{
    u8 r = (u8)std::max(0.0f, std::min(fr * 255.5f, 255.5f));
    u8 g = (u8)std::max(0.0f, std::min(fg * 255.5f, 255.5f));
    u8 b = (u8)std::max(0.0f, std::min(fb * 255.5f, 255.5f));
    u8 a = (u8)std::max(0.0f, std::min(fa * 255.5f, 255.5f));

    vertex_.color_ = Color(r, g, b, a);
}

//---------------------------------------------------------------------------
//! �e�N�X�`�����W
//---------------------------------------------------------------------------
void dxTexCoord2f(f32 u, f32 v)
{
    vertex_.uv_.x = u;
    vertex_.uv_.y = v;
}

//---------------------------------------------------------------------------
//! �@��
//---------------------------------------------------------------------------
void dxNormal3fv(const Vector3& v)
{
    // �l�̐ݒ�
    vertex_.normal_.x = v.x_;
    vertex_.normal_.y = v.y_;
    vertex_.normal_.z = v.z_;
}

//---------------------------------------------------------------------------
//! �ڃx�N�g��
//---------------------------------------------------------------------------
void dxTangent3fv(const Vector3& v)
{
    // �l�̐ݒ�
    vertex_.tangent_.x = v.x_;
    vertex_.tangent_.y = v.y_;
    vertex_.tangent_.z = v.z_;
}

//---------------------------------------------------------------------------
//! �]�@���x�N�g��
//---------------------------------------------------------------------------
void dxBinormal3fv(const Vector3& v)
{
    // �l�̐ݒ�
    vertex_.binormal_.x = v.x_;
    vertex_.binormal_.y = v.y_;
    vertex_.binormal_.z = v.z_;
}

//---------------------------------------------------------------------------
//! �`��I��
//---------------------------------------------------------------------------
void dxEnd()
{
    // �������}�b�s���O�I��
    gpu::D3DContext()->Unmap(dynamicBuffer_->getD3DBuffer(), 0);

    // �`��
    gpu::setVertexBuffer(dynamicBuffer_, sizeof(Vertex));   // ���_�o�b�t�@
    gpu::draw(type_, drawCount_, usedCount_);
    usedCount_ += drawCount_;
    drawCount_ = 0;
}

//---------------------------------------------------------------------------
//! �e�N�X�`���̐ݒ�
//---------------------------------------------------------------------------
void dxTexture(ptr<gpu::Texture> texture)
{
    gpu::setShader(shaderVs3D_, (texture) ? shaderPsTexture_ : shaderPsFlat_);
    gpu::setTexture(0, texture);
}
