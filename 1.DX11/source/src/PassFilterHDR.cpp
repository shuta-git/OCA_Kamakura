//===========================================================================
//!	@file   PassFilterHDR.cpp
//!	@brief	HDR�t�B���^�[�p�X
//===========================================================================
#include "PassFilterHDR.h"

//---------------------------------------------------------------------------
//!	������
//---------------------------------------------------------------------------
bool PassFilterHDR::initialize()
{
    // �V�F�[�_�[�̓ǂݍ���
    shaderTonemappingVs_ = gpu::ShaderVs::create("shader/vsPrim2D.fx");
    shaderTonemappingPs_ = gpu::ShaderPs::create("shader/psTonemapping.fx");

    shaderVs_integrate_ = gpu::ShaderVs::create("shader/vsPrim2D.fx");
    if(!shaderVs_integrate_) {
        return false;
    }

    shaderPs_integrate_ = gpu::ShaderPs::create("shader/psIntegrate.fx");
    if(!shaderPs_integrate_) {
        return false;
    }

    //-------------------------------------------------------------
    // �t�B���^�[������
    //-------------------------------------------------------------
    if(filterGlare_.initialize() == false) {
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------
//!	���
//---------------------------------------------------------------------------
void PassFilterHDR::finalize()
{
    shaderVs_integrate_.reset();
    shaderPs_integrate_.reset();
}

//---------------------------------------------------------------------------
//!	�J�n
//---------------------------------------------------------------------------
void PassFilterHDR::begin()
{
	gpu::setRenderTarget(GmRender()->getRenderBuffer(), nullptr);

    //=============================================================
    // Diffuse/Specular�o�b�t�@��HDR�o�b�t�@�֍���
    //=============================================================
	gpu::setTexture(0, GmRender()->getDiffuseTexture());
	gpu::setTexture(1, GmRender()->getSpecularTexture());
	
	gpu::setShader(shaderVs_integrate_, shaderPs_integrate_);
    GmRender()->put();
}

//---------------------------------------------------------------------------
//!	���s
//---------------------------------------------------------------------------
void PassFilterHDR::execute()
{
    filterGlare_.render();
}

//---------------------------------------------------------------------------
//!	�I��
//---------------------------------------------------------------------------
void PassFilterHDR::end()
{
    //-------------------------------------------------------------
    // �g�[���}�b�s���O��K�p
    //-------------------------------------------------------------
    auto hdrBuffer = GmRender()->getRenderBuffer();

    GmRender()->setHDR(false);   // ��������SDR

    // SDR�o�b�N�o�b�t�@��ݒ�
    gpu::setRenderTarget(GmRender()->getRenderBuffer());

    dxTexture(hdrBuffer);

    // �g�[���}�b�s���O�t�B���^�[�V�F�[�_�[�ݒ�
    gpu::setShader(shaderTonemappingVs_, shaderTonemappingPs_);

    GmRender()->put();
}
