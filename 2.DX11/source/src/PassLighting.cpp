//===========================================================================
//!	@file   PassLighting.cpp
//!	@brief	�Ɩ��p�X
//===========================================================================
#include "PassLighting.h"

//---------------------------------------------------------------------------
//!	������
//---------------------------------------------------------------------------
bool PassLighting::initialize()
{
    shaderVs_ = gpu::ShaderVs::create("shader/vsPrim2D.fx");
    if(!shaderVs_) {
        return false;
    }

    shaderPs_ = gpu::ShaderPs::create("shader/psLight_Directional.fx");
    if(!shaderPs_) {
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------
//!	���
//---------------------------------------------------------------------------
void PassLighting::finalize()
{
    shaderVs_.reset();
    shaderPs_.reset();
}

//---------------------------------------------------------------------------
//!	�J�n
//---------------------------------------------------------------------------
void PassLighting::begin()
{
    std::array<ptr<gpu::Texture>, 2> mrts{
        GmRender()->getDiffuseTexture(),
        GmRender()->getSpecularTexture(),
    };
    gpu::setRenderTarget(mrts.size(), mrts.data(), nullptr);

    gpu::setTexture(9, GmRender()->getDepthStencil());   // �f�v�X�e�N�X�`��

    // ���̉��Z�����̂��߂Ƀu�����h�L��
    f32 blendFactor[4]{};
    gpu::D3DContext()->OMSetBlendState(GmRender()->getBlendState(BLEND_MODE_ADD), blendFactor, 0xffffffffUL);
}

//---------------------------------------------------------------------------
//!	���s
//---------------------------------------------------------------------------
void PassLighting::execute()
{
    gpu::setShader(shaderVs_, shaderPs_);
    GmRender()->put();
}

//---------------------------------------------------------------------------
//!	�I��
//---------------------------------------------------------------------------
void PassLighting::end()
{
    // ���Ƃɖ߂�
    f32 blendFactor[4]{};
    gpu::D3DContext()->OMSetBlendState(GmRender()->getBlendState(BLEND_MODE_OFF), blendFactor, 0xffffffffUL);
    gpu::setRenderTarget(GmRender()->getRenderBuffer(), GmRender()->getDepthStencil());
}
