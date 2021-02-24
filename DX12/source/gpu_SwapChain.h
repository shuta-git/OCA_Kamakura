#pragma once
//---------------------------------------------------------------------------
//!	@file	gpu_SwapChain.h
//!	@brief	GPU�X���b�v�`�F�C��
//---------------------------------------------------------------------------

namespace gpu {

//===========================================================================
//! �X���b�v�`�F�C��
//===========================================================================
class SwapChain
{
public:
    virtual ~SwapChain() = default;

        //! ��ʍX�V
    //! @param  [in]    vsyncInterval   VSYNC�̊Ԋu(0:�m�[�E�F�C�g 1:60fps 2:30fps 3:20fps 4:15fps)
    virtual void present(u32 vsyncInterval) = 0;

    //! GPU�����s�����ɂȂ�܂ő҂�
    //! ���S�ɃA�C�h����ԂɂȂ�܂Ńu���b�N���܂�
    virtual void waitForGpu() = 0;

    //! �t���X�N���[���ƃE�B���h�E���[�h�̐؂�ւ�
    virtual void toggleFullScreen() = 0;

    //----------------------------------------------------------
    //! @name �Q��
    //----------------------------------------------------------
    //@{

    //! ���݂̃t���[���C���f�b�N�X�ԍ����擾
    //! @example bufferCount=3�̂Ƃ��� 0,1,2, 0,1,2 �̂悤�ɕω����܂�
    virtual u32 frameIndex() const = 0;

    //! DXGI�X���b�v�`�F�C�����擾
    virtual IDXGISwapChain3* dxgiSwapChain() const = 0;

    //! �o�b�N�o�b�t�@���擾
    virtual ID3D12Resource* backBuffer(u32 index) const = 0;

    //@}
protected:
    // new�֎~
    SwapChain() = default;
};
    
//! �X���b�v�`�F�C���̍쐬
//! @param  [in]    width       ��
//! @param  [in]    height      ����
//! @param  [in]    dxgiFormat  �s�N�Z���`��
//! @param  [in]    bufferCount �o�b�t�@��( 2 or 3)
std::unique_ptr<gpu::SwapChain> createSwapChain(u32 width, u32 height, DXGI_FORMAT dxgiFormat, u32 bufferCount);


}   // namespace gpu