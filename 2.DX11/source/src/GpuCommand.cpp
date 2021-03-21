//===========================================================================
//! @file  GpuCommand.cpp
//!	@brief	GPU�R�}���h
//===========================================================================
#include <unordered_map>
#include "GpuCommand.h"

namespace gpu {

namespace {
const gpu::InputLayout* gInputLayout;
const gpu::ShaderVs*    gShaderVs;
}   // namespace

//---------------------------------------------------------------------------
//! ���̓��C�A�E�g�̐ݒ�
//---------------------------------------------------------------------------
void setInputLayout(ptr<const gpu::InputLayout> inputLayout)
{
    gInputLayout = inputLayout;
}

//---------------------------------------------------------------------------
// ���_�o�b�t�@�̐ݒ�
//---------------------------------------------------------------------------
void setVertexBuffer(ptr<const gpu::Buffer> buffer, u32 stride, u32 offset)
{
    ID3D11Buffer* buffers[1]{ buffer->getD3DBuffer() };
    u32           strides[1]{ stride };
    u32           offsets[1]{ offset };
    D3DContext()->IASetVertexBuffers(0, 1, buffers, strides, offsets);
}

//---------------------------------------------------------------------------
//! �C���f�b�N�X�o�b�t�@�̐ݒ�
//---------------------------------------------------------------------------
void setIndexBuffer(ptr<const gpu::Buffer> buffer, u32 offset)
{
    D3DContext()->IASetIndexBuffer(buffer->getD3DBuffer(), DXGI_FORMAT_R32_UINT, offset);
}

//---------------------------------------------------------------------------
//! �V�F�[�_�[�̐ݒ�
//---------------------------------------------------------------------------
void setShader(ptr<const gpu::ShaderVs> vs, ptr<const gpu::ShaderPs> ps)
{
    D3DContext()->VSSetShader(vs->get(), nullptr, 0);
    D3DContext()->PSSetShader(ps->get(), nullptr, 0);

    gShaderVs = vs;
}

void setShader(ptr<const gpu::ShaderCs> cs)
{
    D3DContext()->CSSetShader(cs->get(), nullptr, 0);
}

//---------------------------------------------------------------------------
// �萔�o�b�t�@�̐ݒ�
//---------------------------------------------------------------------------
void setConstantBuffer(u32 slot, ptr<gpu::Buffer> buffer)
{
    ID3D11Buffer* d3dBuffer = buffer ? buffer->getD3DBuffer() : nullptr;
    D3DContext()->VSSetConstantBuffers(slot, 1, &d3dBuffer);
    D3DContext()->PSSetConstantBuffers(slot, 1, &d3dBuffer);
    D3DContext()->CSSetConstantBuffers(slot, 1, &d3dBuffer);
}

//---------------------------------------------------------------------------
// �e�N�X�`���̐ݒ�
//---------------------------------------------------------------------------
void setTexture(u32 slot, ptr<gpu::Texture> texture)
{
    ID3D11ShaderResourceView* srv = texture ? texture->getD3DSrv() : nullptr;
    D3DContext()->PSSetShaderResources(slot, 1, &srv);   // �e�N�X�`��
    D3DContext()->CSSetShaderResources(slot, 1, &srv);   // �e�N�X�`��
}

//---------------------------------------------------------------------------
// RW�e�N�X�`���̐ݒ�
//---------------------------------------------------------------------------
void setRWTexture(u32 slot, ptr<gpu::Texture> texture)
{
    auto* uav = texture ? texture->getD3DUav() : nullptr;
    D3DContext()->CSSetUnorderedAccessViews(slot, 1, &uav, nullptr);   // RW�e�N�X�`��
}

//---------------------------------------------------------------------------
//! �J�����O�ݒ�
//---------------------------------------------------------------------------
void setCullMode(D3D11_RASTERIZER_DESC rd)
{
    //D3D11_RASTERIZER_DESC rd{};
    //rd.FillMode = D3D11_FILL_SOLID;
    //// �w�ʃJ�����O�����Ȃ��ꍇ
    //rd.CullMode        = D3D11_CULL_NONE;
    //rd.ScissorEnable   = false;
    //rd.DepthClipEnable = false;
    //// ���v���(clockwise)�̒��_�����ʂ�\�ƌ��Ȃ� (�f�t�H���g)
    //rd.FrontCounterClockwise = FALSE;

    ID3D11RasterizerState* pState;
    gpu::D3DDevice()->CreateRasterizerState(&rd, &pState);
    gpu::D3DContext()->RSSetState(pState);
    pState->Release();
}

//---------------------------------------------------------------------------
//! �`��
//---------------------------------------------------------------------------
void draw(PRIMITIVE_TYPE type, u32 vertexCount, u32 vertexOffset)
{
    if(gInputLayout == nullptr)
        return;

    auto inputLayout    = const_cast<gpu::InputLayout*>(gInputLayout);
    auto d3dInputLayout = inputLayout->getD3DInputLayout(gShaderVs);

    D3DContext()->IASetInputLayout(d3dInputLayout);

    D3DContext()->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)type);
    D3DContext()->Draw(vertexCount, vertexOffset);
}

//---------------------------------------------------------------------------
//! �C���f�b�N�X�`��
//---------------------------------------------------------------------------
void drawIndexed(PRIMITIVE_TYPE type, u32 indexCount, u32 indexOffset, u32 vertexOffset)
{
    auto inputLayout    = const_cast<gpu::InputLayout*>(gInputLayout);
    auto d3dInputLayout = inputLayout->getD3DInputLayout(gShaderVs);

    D3DContext()->IASetInputLayout(d3dInputLayout);

    D3DContext()->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)type);
    D3DContext()->DrawIndexed(indexCount, indexOffset, vertexOffset);
}

//---------------------------------------------------------------------------
//! ���Z���s
//---------------------------------------------------------------------------
void dispatch(u32 groupCountX, u32 groupCountY, u32 groupCountZ)
{
    D3DContext()->Dispatch(groupCountX, groupCountY, groupCountZ);
}

//---------------------------------------------------------------------------
// �`��^�[�Q�b�g�̐ݒ�
//---------------------------------------------------------------------------
void setRenderTarget(ptr<gpu::Texture> renderTarget, ptr<gpu::Texture> depthStencil)
{
    gpu::setRenderTarget(1, &renderTarget, depthStencil);
}

// �`��^�[�Q�b�g�̐ݒ�(MRT�p)
void setRenderTarget(u32 renderTargetCount, ptr<gpu::Texture>* renderTargets, ptr<gpu::Texture> depthStencil)
{
    ID3D11RenderTargetView* d3dRtvs[8];

    for(u32 i = 0; i < renderTargetCount; ++i) {
        d3dRtvs[i] = (renderTargets[i]) ? renderTargets[i]->getD3DRtv() : nullptr;
    }

    auto d3dDsv = (depthStencil) ? depthStencil->getD3DDsv() : nullptr;

    gpu::D3DContext()->OMSetRenderTargets(renderTargetCount, d3dRtvs, d3dDsv);

    //---- �r���[�|�[�g�ݒ�i�`��^�[�Q�b�g�̕ύX���̓r���[�|�[�g�̍X�V���s���j
    D3D11_VIEWPORT viewport;

    auto texture = (renderTargets[0]) ? renderTargets[0] : depthStencil;

    viewport.TopLeftX = 0.0f;                                 // ����X
    viewport.TopLeftY = 0.0f;                                 // ����Y
    viewport.Width    = texture ? texture->getWidth() : 0;    // ��
    viewport.Height   = texture ? texture->getHeight() : 0;   // ����
    viewport.MinDepth = 0.0f;                                 // Z�ŏ��l
    viewport.MaxDepth = 1.0f;                                 // Z�ő�l

    gpu::D3DContext()->RSSetViewports(1, &viewport);
}

void clearTarget(ptr<gpu::Texture> target, const Vector4& color, f32 depth, u8 stencil)
{
    // �`��^�[�Q�b�g�̃N���A
    if(target->getD3DRtv()) {
        auto clearColor = reinterpret_cast<const f32*>(&color);
        gpu::D3DContext()->ClearRenderTargetView(target->getD3DRtv(), clearColor);
    }

    // �f�v�X�o�b�t�@�̃N���A
    if(target->getD3DDsv()) {
        gpu::D3DContext()->ClearDepthStencilView(target->getD3DDsv(),
                                                 D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
                                                 depth,
                                                 stencil);
    }
}

}   // namespace gpu
