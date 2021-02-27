//---------------------------------------------------------------------------
//!	@file	gpu_Buffer.h
//!	@brief	GPUバッファ
//---------------------------------------------------------------------------
#pragma once

namespace gpu {

//===========================================================================
//! バッファ
//===========================================================================
class Buffer
{
public:
    //----------------------------------------------------------
    //! バッファ情報
    //----------------------------------------------------------
    struct Desc
    {
        u32 sizeInBytes_;   //!< サイズ
        D3D12_HEAP_TYPE heapType_ = D3D12_HEAP_TYPE_DEFAULT;//!< メモリの配置場所(D3D11_USAGEと同等)
    };

    virtual ~Buffer() = default;

    //----------------------------------------------------------
    //! @name 参照
    //----------------------------------------------------------
    //@{

    //! 情報を取得
    virtual const Buffer::Desc& desc() const = 0;

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
};

//! バッファを作成
//! @param  [in]    desc            オプション設定
//! @param  [in]    initializeData  初期データーの場所(nullptrの場合は初期化なし)
std::shared_ptr<gpu::Buffer> createBuffer(const Buffer::Desc& desc, void* initializeData=nullptr);

}   // namespace gpu
