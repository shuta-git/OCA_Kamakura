//===========================================================================
//!	@file   GBuffer.cpp
//!	@brief	GBuffer�Ǘ�
//===========================================================================
#include "GBuffer.h"

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool GBuffer::initialize(u32 w, u32 h)
{
    //-------------------------------------------------------------
    // GBuffer�̍쐬
    //-------------------------------------------------------------
    gbuffer_[0].reset(gpu::createTarget(w, h, DXGI_FORMAT_R8G8B8A8_UNORM));
    gbuffer_[1].reset(gpu::createTarget(w, h, DXGI_FORMAT_R8G8B8A8_UNORM));
    gbuffer_[2].reset(gpu::createTarget(w, h, DXGI_FORMAT_R16G16_FLOAT));
    gbuffer_[3].reset(gpu::createTarget(w, h, DXGI_FORMAT_R32G32B32A32_FLOAT));

    return true;
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
void GBuffer::finalize()
{
    gbuffer_[0].reset();
    gbuffer_[1].reset();
    gbuffer_[2].reset();
    gbuffer_[3].reset();
}

//---------------------------------------------------------------------------
//! �J�n
//---------------------------------------------------------------------------
void GBuffer::begin()
{
    std::array<ptr<gpu::Texture>, 4> mrts{
        gbuffer_[0],
        gbuffer_[1],
        gbuffer_[2],
        gbuffer_[3],
    };

    gpu::setRenderTarget(mrts.size(), mrts.data(), GmRender()->getDepthStencil());
}

//---------------------------------------------------------------------------
//! �V���h�E�p�X�̏I��
//---------------------------------------------------------------------------
void GBuffer::end()
{
    // ���ɖ߂�
    gpu::setRenderTarget(GmRender()->getRenderBuffer(), GmRender()->getDepthStencil());

    // GBuffer�̓��e���e�N�X�`���ɐݒ�
    gpu::setTexture(5, gbuffer_[0]);
    gpu::setTexture(6, gbuffer_[1]);
    gpu::setTexture(7, gbuffer_[2]);
    gpu::setTexture(8, gbuffer_[3]);
}

//---------------------------------------------------------------------------
//! Imgui�`��
//---------------------------------------------------------------------------
void GBuffer::imguiRender()
{
    // �E�B���h�E�̏k���T�C�Y
    //�� �l��傫������΂���قǏ������Ȃ�܂�
    int windowShrinkSize = 8;

    //-------------------------------------------------------------
    // GBuffer�̃f�o�b�O�E�B���h�E
    //-------------------------------------------------------------
    ImGui::Begin(u8"GBuffer");

    if(ImGui::BeginTabBar("GBuffer")) {
        // MRT������
        for(u32 i = 0; i < std::size(gbuffer_); ++i) {
            auto& x = gbuffer_[i];

            std::string tabName{ "GBuffer" };

            // GBuffer�ԍ�
            char number[2];
            sprintf_s(number, "%1d", i);
            tabName += number;

            if(ImGui::BeginTabItem(tabName.c_str())) {
                ImGui::Image(x->getD3DSrv(), ImVec2(x->getWidth() / windowShrinkSize, x->getHeight() / windowShrinkSize));
                ImGui::EndTabItem();
            }
        }

        if(ImGui::BeginTabItem("Depth")) {
            // �f�v�X�o�b�t�@
            auto x = GmRender()->getDepthStencil();
            ImGui::Image(x->getD3DSrv(), ImVec2(x->getWidth() / windowShrinkSize, x->getHeight() / windowShrinkSize));
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::Separator();

    if(ImGui::BeginTabBar("Light Accumulation Buffer")) {
        if(ImGui::BeginTabItem("Light Accumulation Buffer")) {
            {
                auto x = GmRender()->getDiffuseTexture();
                ImGui::Image(x->getD3DSrv(), ImVec2(x->getWidth() / windowShrinkSize, x->getHeight() / windowShrinkSize));
            }
            ImGui::SameLine();
            {
                auto x = GmRender()->getSpecularTexture();
                ImGui::Image(x->getD3DSrv(), ImVec2(x->getWidth() / windowShrinkSize, x->getHeight() / windowShrinkSize));
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}
