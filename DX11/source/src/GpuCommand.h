//===========================================================================
//! @file   GpuCommand.h
//!	@brief	GPU�R�}���h
//===========================================================================
#pragma once

//--------------------------------------------------------------
// �v���~�e�B�u�̎��
//--------------------------------------------------------------
enum PRIMITIVE_TYPE
{
    PT_POINTS         = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
    PT_LINES          = D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
    PT_LINE_STRIP     = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
    PT_TRIANGLES      = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
    PT_TRIANGLE_STRIP = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
};

namespace gpu {

//! ���̓��C�A�E�g�̐ݒ�
void setInputLayout(ptr<const gpu::InputLayout> inputLayout);

// ���_�o�b�t�@�̐ݒ�
void setVertexBuffer(ptr<const gpu::Buffer> buffer, u32 stride, u32 offset = 0);

//! �C���f�b�N�X�o�b�t�@�̐ݒ�
void setIndexBuffer(ptr<const gpu::Buffer> buffer, u32 offset = 0);

//! �V�F�[�_�[�̐ݒ�(VS,PS)
void setShader(ptr<const gpu::ShaderVs> vs, ptr<const gpu::ShaderPs> ps);

//! �V�F�[�_�[�̐ݒ�(CS)
void setShader(ptr<const gpu::ShaderCs> cs);

//! �萔�o�b�t�@�̐ݒ�
void setConstantBuffer(u32 slot, ptr<gpu::Buffer> buffer);

//! �e�N�X�`���̐ݒ�
void setTexture(u32 slot, ptr<gpu::Texture> texture);

//! RW�e�N�X�`���̐ݒ�
void setRWTexture(u32 slot, ptr<gpu::Texture> texture);

//! �J�����O�ݒ�
//!	@param [in]	cullMode	�J�����O
void setCullMode(D3D11_RASTERIZER_DESC rd);

//! �`��
//!	@param [in]	type	�v���~�e�B�u�̎��
//!	@param [in]	vertexCount	���_��
//!	@param [in]	vertexOffet	���_�ԍ��̃I�t�Z�b�g
void draw(PRIMITIVE_TYPE type, u32 vertexCount, u32 vertexOffset = 0);

//! �C���f�b�N�X�`��
//!	@param [in]	type	�v���~�e�B�u�̎��
//!	@param [in]	indexCount	�C���f�b�N�X��
//!	@param [in]	indexOffset	�C���f�b�N�X�ԍ��̃I�t�Z�b�g
//!	@param [in]	vertexOffet	���_�ԍ��̃I�t�Z�b�g
void drawIndexed(PRIMITIVE_TYPE type, u32 indexCount, u32 indexOffset = 0, u32 vertexOffset = 0);

//! ���Z���s
//!	@param	[in]	groupCountX	�X���b�h�O���[�v��X
//!	@param	[in]	groupCountY	�X���b�h�O���[�v��Y
//!	@param	[in]	groupCountZ	�X���b�h�O���[�v��Z
void dispatch(u32 groupCountX, u32 groupCountY, u32 groupCountZ);

// �`��^�[�Q�b�g�̐ݒ�
void setRenderTarget(ptr<gpu::Texture> renderTarget, ptr<gpu::Texture> depthStencil = nullptr);

// �`��^�[�Q�b�g�̐ݒ�(MRT�p)
void setRenderTarget(u32 renderTargetCount, ptr<gpu::Texture>* renderTargets, ptr<gpu::Texture> depthStencil = nullptr);

// �`��^�[�Q�b�g�̃N���A
void clearTarget(ptr<gpu::Texture> target, const Vector4& color = Vector4(0.0f, 0.0f, 0.0f, 0.0f), f32 depth = 1.0f, u8 stencil = 0);

}   // namespace gpu
