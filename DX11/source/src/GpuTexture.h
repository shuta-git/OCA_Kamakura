//===========================================================================
//! @file   GpuTexture.h
//!	@brief	�e�N�X�`��
//===========================================================================
#pragma once

namespace gpu {

//===========================================================================
//! �e�N�X�`���N���X
//===========================================================================
class Texture
{
public:
    //! �R���X�g���N�^
    Texture();

    //! �f�X�g���N�^
    ~Texture();

    //! ������(D3DResource�w��)
    bool initialize(ID3D11Resource* d3dResource);

    u32         getWidth() const { return width_; }
    u32         getHeight() const { return height_; }
    u32         getMipLevels() const { return mipLevels_; }
    DXGI_FORMAT getFormat() const { return format_; }

    ID3D11Resource*            getD3DResource() const { return d3dResource_.Get(); }   // �e�N�X�`���������[
    ID3D11ShaderResourceView*  getD3DSrv() const { return d3dSrv_.Get(); }             // SRV
    ID3D11RenderTargetView*    getD3DRtv() const { return d3dRtv_.Get(); }             // RTV
    ID3D11DepthStencilView*    getD3DDsv() const { return d3dDsv_.Get(); }             // DSV
    ID3D11UnorderedAccessView* getD3DUav() const { return d3dUav_.Get(); }             // UAV

private:
    u32         width_;       //!< ��
    u32         height_;      //!< ����
    u32         depth_;       //!< ���s��
    u32         mipLevels_;   //!< �~�b�v���x����
    DXGI_FORMAT format_;      //!< �s�N�Z���t�H�[�}�b�g
    u32         bindFlags_;   //!< �֘A�t����� (D3D11_BIND_FLAG�̑g�ݍ��킹)

    com_ptr<ID3D11Resource>            d3dResource_;   //!< �e�N�X�`���������[
    com_ptr<ID3D11ShaderResourceView>  d3dSrv_;        //!< SRV
    com_ptr<ID3D11RenderTargetView>    d3dRtv_;        //!< RTV
    com_ptr<ID3D11DepthStencilView>    d3dDsv_;        //!< DSV
    com_ptr<ID3D11UnorderedAccessView> d3dUav_;        //!< UAV
};

//! �e�N�X�`���쐬�i�t�@�C���p�X�w��j
gpu::Texture* createTexture(char* path);

//! �e�N�X�`�����쐬(ID3D11Resource�w��)
gpu::Texture* createTexture(ID3D11Resource* d3dResouce);

//! �`��^�[�Q�b�g�쐬
//!	@param	[in]	width	��
//!	@param	[in]	height	����
//!	@param	[in]	format	�s�N�Z���`��
gpu::Texture*	createTarget(u32 width, u32 height, DXGI_FORMAT format);


}   // namespace gpu
