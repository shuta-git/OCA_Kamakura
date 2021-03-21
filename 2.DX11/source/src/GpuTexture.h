//===========================================================================
//! @file   GpuTexture.h
//!	@brief	テクスチャ
//===========================================================================
#pragma once

namespace gpu {

//===========================================================================
//! テクスチャクラス
//===========================================================================
class Texture
{
public:
    //! コンストラクタ
    Texture();

    //! デストラクタ
    ~Texture();

    //! 初期化(D3DResource指定)
    bool initialize(ID3D11Resource* d3dResource);

    u32         getWidth() const { return width_; }
    u32         getHeight() const { return height_; }
    u32         getMipLevels() const { return mipLevels_; }
    DXGI_FORMAT getFormat() const { return format_; }

    ID3D11Resource*            getD3DResource() const { return d3dResource_.Get(); }   // テクスチャメモリー
    ID3D11ShaderResourceView*  getD3DSrv() const { return d3dSrv_.Get(); }             // SRV
    ID3D11RenderTargetView*    getD3DRtv() const { return d3dRtv_.Get(); }             // RTV
    ID3D11DepthStencilView*    getD3DDsv() const { return d3dDsv_.Get(); }             // DSV
    ID3D11UnorderedAccessView* getD3DUav() const { return d3dUav_.Get(); }             // UAV

private:
    u32         width_;       //!< 幅
    u32         height_;      //!< 高さ
    u32         depth_;       //!< 奥行き
    u32         mipLevels_;   //!< ミップレベル数
    DXGI_FORMAT format_;      //!< ピクセルフォーマット
    u32         bindFlags_;   //!< 関連付け情報 (D3D11_BIND_FLAGの組み合わせ)

    com_ptr<ID3D11Resource>            d3dResource_;   //!< テクスチャメモリー
    com_ptr<ID3D11ShaderResourceView>  d3dSrv_;        //!< SRV
    com_ptr<ID3D11RenderTargetView>    d3dRtv_;        //!< RTV
    com_ptr<ID3D11DepthStencilView>    d3dDsv_;        //!< DSV
    com_ptr<ID3D11UnorderedAccessView> d3dUav_;        //!< UAV
};

//! テクスチャ作成（ファイルパス指定）
gpu::Texture* createTexture(char* path);

//! テクスチャを作成(ID3D11Resource指定)
gpu::Texture* createTexture(ID3D11Resource* d3dResouce);

//! 描画ターゲット作成
//!	@param	[in]	width	幅
//!	@param	[in]	height	高さ
//!	@param	[in]	format	ピクセル形式
gpu::Texture*	createTarget(u32 width, u32 height, DXGI_FORMAT format);


}   // namespace gpu
