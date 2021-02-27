//===========================================================================
//! @file   GpuTexture.cpp
//!	@brief	テクスチャ
//===========================================================================
//---- DirectXTex
#include <../DirectXTex/DirectXTex/DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "OpenSource/DirectXTex/DirectXTex/Bin/Desktop_2017_Win10/x64/Debug/DirectXTex.lib")
#else
#pragma comment(lib, "OpenSource/DirectXTex/DirectXTex/Bin/Desktop_2017_Win10/x64/Release/DirectXTex.lib")
#endif

namespace gpu {

//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
Texture::Texture()
: width_(0)                      // 幅
, height_(0)                     // 高さ
, depth_(0)                      // 奥行き
, mipLevels_(0)                  // ミップレベル数
, format_(DXGI_FORMAT_UNKNOWN)   // ピクセルフォーマット
{
}

//---------------------------------------------------------------------------
//! デストラクタ
//---------------------------------------------------------------------------
Texture::~Texture()
{
}

//---------------------------------------------------------------------------
//! 初期化(D3DResource指定)
//---------------------------------------------------------------------------
bool Texture::initialize(ID3D11Resource* d3dResource)
{
    d3dResource_.Attach(d3dResource);

    D3D11_RESOURCE_DIMENSION type;
    d3dResource->GetType(&type);
    switch(type) {
        case D3D11_RESOURCE_DIMENSION_UNKNOWN:
            return false;
        case D3D11_RESOURCE_DIMENSION_BUFFER:
            return false;
        case D3D11_RESOURCE_DIMENSION_TEXTURE1D:   // 1Dテクスチャ
        {
            auto                 d3dTexture = static_cast<ID3D11Texture1D*>(d3dResource);
            D3D11_TEXTURE1D_DESC desc;
            d3dTexture->GetDesc(&desc);

            width_     = desc.Width;
            height_    = 1;
            depth_     = 1;
            mipLevels_ = desc.MipLevels;
            format_    = desc.Format;
            bindFlags_ = desc.BindFlags;
            break;
        }
        case D3D11_RESOURCE_DIMENSION_TEXTURE2D:   // 2Dテクスチャ
        {
            auto                 d3dTexture = static_cast<ID3D11Texture2D*>(d3dResource);
            D3D11_TEXTURE2D_DESC desc;
            d3dTexture->GetDesc(&desc);

            width_     = desc.Width;
            height_    = desc.Height;
            depth_     = 1;
            mipLevels_ = desc.MipLevels;
            format_    = desc.Format;
            bindFlags_ = desc.BindFlags;
            break;
        }
        case D3D11_RESOURCE_DIMENSION_TEXTURE3D:   // 3Dテクスチャ
        {
            auto                 d3dTexture = static_cast<ID3D11Texture3D*>(d3dResource);
            D3D11_TEXTURE3D_DESC desc;
            d3dTexture->GetDesc(&desc);

            width_     = desc.Width;
            height_    = desc.Height;
            depth_     = desc.Depth;
            mipLevels_ = desc.MipLevels;
            format_    = desc.Format;
            bindFlags_ = desc.BindFlags;

            break;
        }
    }

	//-------------------------------------------------------------
    // ビューを自動作成
	//-------------------------------------------------------------
    if(bindFlags_ & D3D11_BIND_SHADER_RESOURCE) {
        D3D11_SHADER_RESOURCE_VIEW_DESC  v{};
        D3D11_SHADER_RESOURCE_VIEW_DESC* viewDesc = nullptr;

        auto format = format_;

        if(format == DXGI_FORMAT_R32_TYPELESS) {
            format = DXGI_FORMAT_R32_FLOAT;

            v.Format                    = format;
            v.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
            v.Texture2D.MostDetailedMip = 0;
            v.Texture2D.MipLevels       = mipLevels_;

            viewDesc = &v;
        }

        if(FAILED(D3DDevice()->CreateShaderResourceView(d3dResource, viewDesc, &d3dSrv_))) {
            return false;
        }
    }
    if(bindFlags_ & D3D11_BIND_RENDER_TARGET) {
        if(FAILED(D3DDevice()->CreateRenderTargetView(d3dResource, nullptr, &d3dRtv_))) {
            return false;
        }
    }
    if(bindFlags_ & D3D11_BIND_DEPTH_STENCIL) {
        D3D11_DEPTH_STENCIL_VIEW_DESC  v{};
        D3D11_DEPTH_STENCIL_VIEW_DESC* viewDesc = nullptr;

        auto format = format_;

        if(format == DXGI_FORMAT_R32_TYPELESS) {
            format = DXGI_FORMAT_D32_FLOAT;

            v.Format             = format;
            v.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
            v.Texture2D.MipSlice = 0;

            viewDesc = &v;
        }

        if(FAILED(D3DDevice()->CreateDepthStencilView(d3dResource, viewDesc, &d3dDsv_))) {
            return false;
        }
    }
    if(bindFlags_ & D3D11_BIND_UNORDERED_ACCESS) {
        if(FAILED(D3DDevice()->CreateUnorderedAccessView(d3dResource, nullptr, &d3dUav_))) {
            return false;
        }
    }

    return true;
}

//---------------------------------------------------------------------------
//! テクスチャ作成（ファイルパス指定）
//---------------------------------------------------------------------------
gpu::Texture* createTexture(char* path)
{
    HRESULT         hr;
    std::vector<u8> textureDDS;

    //-------------------------------------------------------------
    // DDSファイルをメモリ上に読み込み
    //-------------------------------------------------------------
    FILE* fp;
    if(fopen_s(&fp, path, "rb") != 0) {
        return nullptr;
    }

    //---- ファイルサイズを取得
    fseek(fp, 0, SEEK_END);
    size_t fileSize = ftell(fp);   // ファイルサイズ
    fseek(fp, 0, SEEK_SET);

    //---- ワーク領域を確保して読み込み
    textureDDS.resize(fileSize);   // ワーク領域の確保
    if(textureDDS.empty()) {
        fclose(fp);
        return false;
    }
    fread(&textureDDS[0], fileSize, 1, fp);   // 読み込み
    fclose(fp);

    //--------------------------------------------------------------------------
    // テクスチャを作成
    //--------------------------------------------------------------------------
    u32  cpuAccessFlags = 0;
    u32  bindFlags      = D3D11_BIND_SHADER_RESOURCE;
    u32  miscFlags      = 0;
    bool forceSRGB      = false;

    // 出力結果を受け取る変数
    com_ptr<ID3D11Resource> d3dResource;
    DirectX::DDS_ALPHA_MODE ddsAlphaMode;

    //---- テクスチャ生成
    hr = DirectX::CreateDDSTextureFromMemoryEx(
        D3DDevice(),             // [in]	D3Dデバイス
        textureDDS.data(),       // [in]	DDSファイルの画像データの先頭アドレス
        textureDDS.size(),       // [in]	DDSファイルのファイルサイズ
        0,                       // [in]	生成するテクスチャの最大サイズ(0指定で自動設定)
        D3D11_USAGE_IMMUTABLE,   // [in]	用途
        bindFlags,               // [in]	関連付けされる対象
        cpuAccessFlags,          // [in]	CPUアクセス許可フラグ
        miscFlags,               // [in]	その他オプションフラグ
        forceSRGB,               // [in]	sRGBモードに設定 true:強制設定 false:設定しない
        &d3dResource,            // [out]	受け取るID3D11Texture2Dポインタ
        nullptr,                 // [out]	受け取るID3D11ShaderResourceViewポインタ
        &ddsAlphaMode);          // [out]	読み込まれたDDSのアルファモード

    if(hr != S_OK) {
        u32 loadFlags = forceSRGB ? WIC_LOADER_FORCE_SRGB : WIC_LOADER_IGNORE_SRGB;

        hr = DirectX::CreateWICTextureFromMemoryEx(
            D3DDevice(),             // [in]	D3Dデバイス
            textureDDS.data(),       // [in]	DDSファイルの画像データの先頭アドレス
            textureDDS.size(),       // [in]	DDSファイルのファイルサイズ
            0,                       // [in]	生成するテクスチャの最大サイズ(0指定で自動設定)
            D3D11_USAGE_IMMUTABLE,   // [in]	用途
            bindFlags,               // [in]	関連付けされる対象
            cpuAccessFlags,          // [in]	CPUアクセス許可フラグ
            miscFlags,               // [in]	その他オプションフラグ
            loadFlags,               // [in]	sRGBモードに設定 true:強制設定 false:設定しない
            &d3dResource,            // [out]	受け取るID3D11Texture2Dポインタ
            nullptr);                // [out]	受け取るID3D11ShaderResourceViewポインタ
        if(hr != S_OK) {
            // TGAファイルの読み込み
            DirectX::ScratchImage scratchImage;
            DirectX::TexMetadata  metaData;

            if(DirectX::LoadFromTGAMemory(
                   textureDDS.data(),
                   textureDDS.size(),
                   &metaData,
                   scratchImage) != S_OK) {
                return nullptr;
            }

            if(DirectX::CreateTexture(
                   D3DDevice(),
                   scratchImage.GetImages(), scratchImage.GetImageCount(),
                   metaData,
                   &d3dResource) != S_OK) {
                return nullptr;
            }
        }
    }

    //-------------------------------------------------------------
    // テクスチャクラスの初期化
    //-------------------------------------------------------------
    return gpu::createTexture(d3dResource.Detach());
}

//---------------------------------------------------------------------------
//! テクスチャを作成(ID3D11Resource指定)
//---------------------------------------------------------------------------
gpu::Texture* createTexture(ID3D11Resource* d3dResource)
{
    std::unique_ptr<gpu::Texture> p(new gpu::Texture);
    if(!p) {
        return nullptr;
    }

    if(!p->initialize(d3dResource)) {
        return nullptr;
    }
    return p.release();   // unique_ptrの所有権を解除
}

//---------------------------------------------------------------------------
//! 描画ターゲット作成
//!	@param	[in]	width	幅
//!	@param	[in]	height	高さ
//!	@param	[in]	format	ピクセル形式
//---------------------------------------------------------------------------
gpu::Texture* createTarget(u32 width, u32 height, DXGI_FORMAT format)
{
    //----------------------------------------------------------
    // 設定
    //----------------------------------------------------------
    D3D11_TEXTURE2D_DESC desc;

    u32 bindFlags = D3D11_BIND_SHADER_RESOURCE;

    if(format == DXGI_FORMAT_D32_FLOAT) {
        bindFlags |= D3D11_BIND_DEPTH_STENCIL;
    }
    else {
        bindFlags |= D3D11_BIND_RENDER_TARGET;
        bindFlags |= D3D11_BIND_UNORDERED_ACCESS;	// UAV対応
    }

    desc.SampleDesc.Count = 1;

    desc.Width              = width;
    desc.Height             = height;
    desc.MipLevels          = 1;
    desc.ArraySize          = 1;
    desc.Format             = (format == DXGI_FORMAT_D32_FLOAT) ? DXGI_FORMAT_R32_TYPELESS : format;
    desc.SampleDesc.Count   = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage              = D3D11_USAGE_DEFAULT;
    desc.BindFlags          = bindFlags;
    desc.CPUAccessFlags     = 0;
    desc.MiscFlags          = 0;

    //----------------------------------------------------------
    // 作成
    //----------------------------------------------------------
    com_ptr<ID3D11Texture2D> d3dTexture2D;
    if(D3DDevice()->CreateTexture2D(&desc, nullptr, &d3dTexture2D) != S_OK) {
        return nullptr;
    }

    return gpu::createTexture(d3dTexture2D.Detach());
}

}   // namespace gpu
