//---------------------------------------------------------------------------
//!	@file	gpu_Texture.h
//!	@brief	GPUテクスチャ
//---------------------------------------------------------------------------
#pragma once

namespace gpu {

//===========================================================================
//! テクスチャ
//===========================================================================
class Texture
{
public:
    //----------------------------------------------------------
    //! テクスチャ情報
    //----------------------------------------------------------
    struct Desc
    {
        u32         width_;        //!< 幅
        u32         height_;       //!< 高さ
        DXGI_FORMAT dxgiFormat_;   //!< ピクセル形式
        u32         mipLevels_;    //!< ミップレベル
        u32         arrayCount_;   //!< 配列数
        bool        isCubemap_;    //!< キューブマップかどうか
    };

    virtual ~Texture() = default;

    //----------------------------------------------------------
    //! @name 参照
    //----------------------------------------------------------
    //@{

    //! テクスチャ情報を取得
    virtual const Texture::Desc& desc() const = 0;

    //! D3D12リソースを取得
    virtual ID3D12Resource* d3dResource() const = 0;

    //! SRVを取得
    virtual u32 srv() const = 0;

    //! RTVを取得
    virtual u32 rtv() const = 0;

    //! DSVを取得
    virtual u32 dsv() const = 0;

    //! UAVを取得
    virtual u32 uav() const = 0;

    //@}
protected:
    Texture() = default;
};

//! テクスチャをファイルから作成
//! @param  [in]    path    ファイルパス
std::shared_ptr<gpu::Texture> createTextureFromFile(std::string_view path);

}   // namespace gpu
