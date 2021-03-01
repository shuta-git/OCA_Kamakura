#pragma once
//---------------------------------------------------------------------------
//!	@file	gpu_Render.h(����)
//!	@brief	gpu�S��
//---------------------------------------------------------------------------

// �O���錾
class IndexAllocator;

namespace gpu {

//! �O���錾
class SwapChain;

class Render
{
public:
    //! ������
    virtual bool initialize(u32 width, u32 height) = 0;

    //! �X�V
    virtual void update() = 0;

    //! �`��
    virtual void render() = 0;

    //! ���
    virtual void finalize() = 0;

    //! �R�}���h���X�g���擾
    virtual ID3D12GraphicsCommandList* graphicsCommandList() const = 0;

    //! �R�}���h�L���[���擾
    //! @param [in]                 �R�}���h�L���[�̎��
    //! - D3D12_COMMAND_LIST_TYPE_DIRECT  �ʏ�L���[
    //! - D3D12_COMMAND_LIST_TYPE_COMPUTE Compute�L���[
    //! - D3D12_COMMAND_LIST_TYPE_COPY    �R�s�[�L���[
    virtual ID3D12CommandQueue* commandQueue(D3D12_COMMAND_LIST_TYPE type) const = 0;

    //! �X���b�v�`�F�C�����擾
    virtual gpu::SwapChain* swapChain() const = 0;

    //! ���̂��擾
    static Render* instance();

    //! IMGUI�̃q�[�v���擾
    virtual com_ptr<ID3D12DescriptorHeap> GetHeapForImgui() const = 0;

private:

    //! 
    virtual com_ptr<ID3D12DescriptorHeap> CreateDescriptorHeapForImgui() = 0;

protected:
    // new�֎~
    Render() = default;
    virtual ~Render() = default;
};
    
//! �����_�����O�Ǘ��N���X���擾
gpu::Render* render();

//! DXGIFactory���擾
IDXGIFactory6* dxgiFactory();

//! D3D�f�o�C�X���擾
ID3D12Device* d3dDevice();

// DescriptorHeap���擾
gpu::DescriptorHeap* descriptorHeap(DESCRIPTOR_TYPE type);

// �C���f�b�N�X�A���P�[�^�[���擾
IndexAllocator* indexAllocator(DESCRIPTOR_TYPE type);

}   // namespace gpu