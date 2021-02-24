//---------------------------------------------------------------------------
//!	@file   gpu_Texture.cpp
//!	@brief	GPUテクスチャ
//---------------------------------------------------------------------------
#include "DirectXTex/DirectXTex/DirectXTex.h"
#include <iostream>
#include <fstream>
#include "IndexAllocator.h"
#include "DelayedRelease.h"
#include "gpu_AsyncCopy.h"  // 非同期コピー転送

namespace gpu {

//===========================================================================
//! テクスチャ実装部
//===========================================================================
class TextureImpl final : public gpu::Texture
{
public:
    //! コンストラクタ
    TextureImpl(const Texture::Desc& desc, ID3D12Resource* d3d12Resource)
    {
        desc_          = desc;
        d3d12Resource_ = d3d12Resource;
    }

    //! デストラクタ
    virtual ~TextureImpl()
    {
        if(srv_) {
            gpu::indexAllocator(gpu::DESCRIPTOR_TYPE::CBV_SRV_UAV)->free(srv_);
        }
    }

    // 初期化
    bool initialize()
    {
        //------------------------------------------------------
        // SRVの初期化
        //------------------------------------------------------
        {
            // SRVの番号を取得
            srv_ = gpu::indexAllocator(gpu::DESCRIPTOR_TYPE::CBV_SRV_UAV)->alloc();
            if(!srv_)
                return false;

            // テクスチャ1
            D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
            srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            srvDesc.Format                  = desc().dxgiFormat_;
            srvDesc.ViewDimension           = D3D12_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MipLevels     = 1;

            auto index = srv_ & 0x7ffffffful;

            gpu::d3dDevice()->CreateShaderResourceView(d3dResource(), &srvDesc, descriptorHeap(gpu::DESCRIPTOR_TYPE::CBV_SRV_UAV)->at(index));
        }

        return true;
    }

    //----------------------------------------------------------
    //! @name 参照
    //----------------------------------------------------------
    //@{

    //! テクスチャ情報を取得
    virtual const Texture::Desc& desc() const override { return desc_; }

    //! D3D12リソースを取得
    virtual ID3D12Resource* d3dResource() const override { return d3d12Resource_.Get(); }

    //! SRVを取得
    virtual u32 srv() const override { return srv_ & 0x7ffffffful; }

    //! RTVを取得
    virtual u32 rtv() const override { return rtv_ & 0x7ffffffful; }

    //! DSVを取得
    virtual u32 dsv() const override { return dsv_ & 0x7ffffffful; }

    //! UAVを取得
    virtual u32 uav() const override { return uav_ & 0x7ffffffful; }

    //@}
private:
    // コピー禁止/move禁止
    TextureImpl(const TextureImpl&) = delete;
    TextureImpl(TextureImpl&&)      = delete;
    TextureImpl& operator=(const TextureImpl&) = delete;
    TextureImpl& operator=(TextureImpl&&) = delete;

private:
    Texture::Desc           desc_ = {};
    com_ptr<ID3D12Resource> d3d12Resource_;   //!< テクスチャGPUメモリ(HEAP_DEFAULT)

    u32 srv_ = 0;   //!< SRVのDescriptorHeapインデックス番号
    u32 rtv_ = 0;
    u32 dsv_ = 0;
    u32 uav_ = 0;
};

//---------------------------------------------------------------------------
//! テクスチャをファイルから作成
//---------------------------------------------------------------------------
std::shared_ptr<gpu::Texture> createTextureFromFile(std::string_view path)
{
    //---------------------------------------------------------------------
    // ファイルからイメージを読み込み
    //---------------------------------------------------------------------
    std::vector<char> image;
    {
        // ファイルから読み込み
        std::ifstream file(std::string(path), std::ios::in | std::ios::binary | std::ios::ate);   // ateを指定すると最初からファイルポインタが末尾に移動
        if(!file.is_open()) {
            return nullptr;
        }
        auto size = file.tellg();
        image.resize(size);

        file.seekg(0, std::ios::beg);
        file.read(image.data(), size);
        file.close();
    }

    // DDSファイルをロード
    DirectX::TexMetadata  metadata;
    DirectX::ScratchImage scratchImage;
    {
        u32 ddsFlags = 0;

        if(FAILED(DirectX::LoadFromDDSMemory(image.data(),       // [in]  DDSファイルの先頭アドレス
                                             image.size(),       // [in]  DDSファイルのサイズ
                                             ddsFlags,           // [in]  フラグ(DirectX::DDS_FLAGS)
                                             &metadata,          // [out] テクスチャメタ情報
                                             scratchImage))) {   // [out] イメージ情報
            return nullptr;
        }
    }
    image.clear();

    // パラメーターの保存
    Texture::Desc desc{};
    desc.width_      = static_cast<u32>(metadata.width);
    desc.height_     = static_cast<u32>(metadata.height);
    desc.dxgiFormat_ = metadata.format;
    desc.mipLevels_  = static_cast<u32>(metadata.mipLevels);
    desc.arrayCount_ = static_cast<u32>(metadata.arraySize);
    desc.isCubemap_  = metadata.IsCubemap();

    com_ptr<ID3D12Resource> d3dResource;
    {
        u32  resourceFlags = D3D12_RESOURCE_FLAG_NONE;
        bool forceSRGB     = false;

        // テクスチャに合わせたGPUメモリを確保(ここでは領域の確保のみ)
        // リソース状態はCOPY_DESTで作成される
        if(FAILED(DirectX::CreateTextureEx(gpu::d3dDevice(),                                   // [in]  D3D12デバイス
                                           metadata,                                           // [in]  テクスチャメタ情報
                                           static_cast<D3D12_RESOURCE_FLAGS>(resourceFlags),   // [in]  リソースフラグ
                                           forceSRGB,                                          // [in]  強制的にsRGBとして読み込むかどうか true:強制sRGB false:指定なし
                                           &d3dResource))) {                                   // [out] D3D12リソース
            return nullptr;
        }
    }

    //---------------------------------------------------------------------
    // VRAMへイメージ転送するための情報作成
    //---------------------------------------------------------------------
    std::vector<D3D12_SUBRESOURCE_DATA> subresources;

    if(FAILED(DirectX::PrepareUpload(gpu::d3dDevice(),               // [in]  D3D12デバイス
                                     scratchImage.GetImages(),       // [in]  Image配列の先頭アドレス
                                     scratchImage.GetImageCount(),   // [in]  Imageの数
                                     metadata,                       // [in]  テクスチャメタ情報
                                     subresources))) {               // [out] サブリソース情報
        return nullptr;
    }


    //---------------------------------------------------------------------
    // 非同期転送
    //---------------------------------------------------------------------
    gpu::asyncCopy(d3dResource.Get(), subresources.data(), subresources.size());

    //---------------------------------------------------------------------
    // テクスチャクラスの初期化
    //---------------------------------------------------------------------
   auto p = std::make_shared<gpu::TextureImpl>(desc, d3dResource.Get());

    if(!p->initialize()) {
        p.reset();
    }
    return p;
}

}   // namespace gpu
