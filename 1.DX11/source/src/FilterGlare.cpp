//===========================================================================
//!	@file   FilterGlare.cpp
//!	@brief	�O���A�t�B���^�[
//===========================================================================
#include "FilterGlare.h"

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool FilterGlare::initialize()
{
    shaderVs_ = gpu::ShaderVs::create("shader/vsPrim2D.fx");
    if(!shaderVs_) {
        return false;
    }

    shaderPs_Luminance_ = gpu::ShaderPs::create("shader/psFilterGlare_Luminance.fx");
    if(!shaderPs_Luminance_) {
        return false;
    }

    shaderPs_GaussianBlurH_ = gpu::ShaderPs::create("shader/psGaussianBlur_H.fx");
    if(!shaderPs_GaussianBlurH_) {
        return false;
    }
    shaderPs_GaussianBlurV_ = gpu::ShaderPs::create("shader/psGaussianBlur_V.fx");
    if(!shaderPs_GaussianBlurV_) {
        return false;
    }

    shaderPs_Texture_ = gpu::ShaderPs::create("shader/psTexture_Clamp.fx");
    if(!shaderPs_Texture_) {
        return false;
    }

    shaderPs_ = gpu::ShaderPs::create("shader/psFilterGlare.fx");
    if(!shaderPs_) {
        return false;
    }

    shaderPs_Glare_ = gpu::ShaderPs::create("shader/psFilterGlare_Glare.fx");
    if(!shaderPs_Glare_) {
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------
//! �`��
//---------------------------------------------------------------------------
void FilterGlare::render()
{
	return;



    gpu::Texture* work[2];
    work[0] = GmRender()->getHDRWorkBuffer(0);
    work[1] = GmRender()->getHDRWorkBuffer(1);

    gpu::Texture* backBuffer = GmRender()->getRenderBuffer();

    //----------------------------------------------------------
    // ���P�x�����𒊏o
    //----------------------------------------------------------
    gpu::setRenderTarget(work[0]);
    gpu::setTexture(0, backBuffer);
    gpu::setShader(shaderVs_, shaderPs_Luminance_);

    GmRender()->put();

    //----------------------------------------------------------
    // �K�E�V�A���ڂ���
    //----------------------------------------------------------
    for(u32 mip = 1; mip < 7; ++mip) {
        // �~�b�v��i�k���R�s�[����
        gpu::setRenderTarget(GmRender()->getHDRWorkBuffer(0, mip));
        gpu::setTexture(0, GmRender()->getHDRWorkBuffer(0, mip - 1));
        gpu::setShader(shaderVs_, shaderPs_Texture_);

        GmRender()->put();

        // 1pass�� (����)
        gpu::setRenderTarget(GmRender()->getHDRWorkBuffer(1, mip));
        gpu::setTexture(0, GmRender()->getHDRWorkBuffer(0, mip));
        gpu::setShader(shaderVs_, shaderPs_GaussianBlurH_);

        GmRender()->put();

        // 2pass�� (����)
        gpu::setRenderTarget(GmRender()->getHDRWorkBuffer(0, mip));
        gpu::setTexture(0, GmRender()->getHDRWorkBuffer(1, mip));
        gpu::setShader(shaderVs_, shaderPs_GaussianBlurV_);

        GmRender()->put();
    }
    //----------------------------------------------------------
    // ���ʂ����Z����
    //----------------------------------------------------------
    f32 blendFactor[4]{};
    gpu::D3DContext()->OMSetBlendState(GmRender()->getBlendState(BLEND_MODE_ADD), blendFactor, 0xffffffffUL);

    gpu::setRenderTarget(backBuffer);

    gpu::setTexture(0, GmRender()->getHDRWorkBuffer(0, 1));
    gpu::setTexture(1, GmRender()->getHDRWorkBuffer(0, 2));
    gpu::setTexture(2, GmRender()->getHDRWorkBuffer(0, 3));
    gpu::setTexture(3, GmRender()->getHDRWorkBuffer(0, 4));
    gpu::setTexture(4, GmRender()->getHDRWorkBuffer(0, 5));
    gpu::setTexture(5, GmRender()->getHDRWorkBuffer(0, 6));

    gpu::setShader(shaderVs_, shaderPs_);

    GmRender()->put();

    // ������������OFF
    gpu::D3DContext()->OMSetBlendState(GmRender()->getBlendState(BLEND_MODE_OFF), blendFactor, 0xffffffffUL);

    //----------------------------------------------------------
    // �O���A��䊂��v�Z
    //----------------------------------------------------------

    u32 lineNum = 6;

    for(u32 n = 0; n < lineNum; ++n) {
        //-----------------------------------------------------------
        // ����
        //-----------------------------------------------------------
        Vector4 dir{};

        f32 PI = 3.14159f;

        f32 angle       = f32(n) / f32(lineNum) * (2.0f * PI);
        f32 angleOffset = 0.25f / f32(lineNum) * (2.0f * PI);
        dir.x_          = sinf(angle + angleOffset);
        dir.y_          = cosf(angle + angleOffset);

        // �~�b�v��i�k���R�s�[����
        // 1280x720 �� 640x360
        gpu::setRenderTarget(GmRender()->getHDRWorkBuffer(0, 1));
        gpu::setTexture(0, GmRender()->getHDRWorkBuffer(0, 0));
        gpu::setShader(shaderVs_, shaderPs_Texture_);
        GmRender()->put();

        // 640x360 �� 320x180
        //       gpu::setRenderTarget(GmRender()->getHDRWorkBuffer(0, 2));
        //       gpu::setTexture(0, GmRender()->getHDRWorkBuffer(0, 1));
        //       gpu::setShader(shaderVs_, shaderPs_Texture_);
        //       GmRender()->put();

        //-----------------------------------------------------------
        // �u���[
        //-----------------------------------------------------------
        u32 srcIndex = 0;   // �]�����C���f�b�N�X�ԍ�

        const u32 PASS_COUNT = 3;
        for(s32 i = 0; i < PASS_COUNT; ++i) {
            {
                // �萔�o�b�t�@��]��
                auto p          = cbFilter_.begin();
                p->stride_      = powf(8.0f, i);
                p->attenuation_ = (i == (PASS_COUNT - 1)) ? 0.3f : 0.9f;
                p->dir_         = dir;

                cbFilter_.end();
            }
            gpu::setConstantBuffer(4, cbFilter_);   // �萔�o�b�t�@

            gpu::setRenderTarget(GmRender()->getHDRWorkBuffer(srcIndex ^ 1, 1));
            gpu::setTexture(0, GmRender()->getHDRWorkBuffer(srcIndex, 1));
            gpu::setShader(shaderVs_, shaderPs_Glare_);
            GmRender()->put();

            srcIndex ^= 1;
        }

        //-----------------------------------------------------------
        // ��ʂɓK�p(���Z����)
        //-----------------------------------------------------------

        f32 blendFactor[4]{};
        gpu::D3DContext()->OMSetBlendState(GmRender()->getBlendState(BLEND_MODE_ADD), blendFactor, 0xffffffffUL);

        gpu::setRenderTarget(backBuffer);
        gpu::setTexture(0, GmRender()->getHDRWorkBuffer(srcIndex, 1));

        gpu::setShader(shaderVs_, shaderPs_Texture_);

        GmRender()->put();

        // ������������OFF
        gpu::D3DContext()->OMSetBlendState(GmRender()->getBlendState(BLEND_MODE_OFF), blendFactor, 0xffffffffUL);
    }

    // ���Ƃɖ߂�
    gpu::setTexture(0, nullptr);
    gpu::setTexture(1, nullptr);
    gpu::setTexture(2, nullptr);
    gpu::setTexture(3, nullptr);
    gpu::setTexture(4, nullptr);
    gpu::setTexture(5, nullptr);
    gpu::setRenderTarget(backBuffer, GmRender()->getDepthStencil());

//-------------------------------------------------------------
// �V���h�E�}�b�v�̃f�o�b�O�E�B���h�E
//-------------------------------------------------------------
#define USE_DEBUG_GUI 1

    if constexpr(USE_DEBUG_GUI) {
        ImGui::Begin(u8"HDR���[�N�o�b�t�@");
        {
            ImGui::Text("work[0]");
            ImGui::Image(GmRender()->getHDRWorkBuffer(0, 1)->getD3DSrv(), ImVec2(1280 / 2, 720 / 2));

            ImGui::Text("work[1]");
            ImGui::Image(GmRender()->getHDRWorkBuffer(1, 1)->getD3DSrv(), ImVec2(1280 / 2, 720 / 2));
        }
        ImGui::End();
    }
}
