//---------------------------------------------------------------------------
//!	@file   gpu_Buffer.cpp
//!	@brief	GPUバッファ
//---------------------------------------------------------------------------
#include"IndexAllocator.h"
#include"DelayedRelease.h"
#include"gpu_AsyncCopy.h"

namespace gpu {

//===========================================================================
//! バッファ実装部
//===========================================================================
class BufferImpl final : public gpu::Buffer
{
public:
    //! コンストラクタ
    BufferImpl(const Buffer::Desc& desc)
    {
        desc_ = desc;
    }

    //! デストラクタ
    virtual ~BufferImpl()
    {
        if(mappedAddress_) {
            CD3DX12_RANGE writeRange(0, 0);   // この直後に破棄するため範囲指定を省略
            d3d12Resource_->Unmap(0, &writeRange);
        }

        if(srv_) {
            gpu::indexAllocator(gpu::DESCRIPTOR_TYPE::CBV_SRV_UAV)->free(srv_);
        }
    }

    // 初期化
    bool initialize(void* initializeData);

    //----------------------------------------------------------
    //! @name 参照
    //----------------------------------------------------------
    //@{

    //! テクスチャ情報を取得
    virtual const Buffer::Desc& desc() const override { return desc_; }

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
    BufferImpl(const BufferImpl&) = delete;
    BufferImpl(BufferImpl&&)      = delete;
    BufferImpl& operator=(const BufferImpl&) = delete;
    BufferImpl& operator=(BufferImpl&&) = delete;

private:
    Buffer::Desc            desc_ = {};
    com_ptr<ID3D12Resource> d3d12Resource_;             //!< GPUメモリ(HEAP_DEFAULT)
    void*                   mappedAddress_ = nullptr;   //!< マップされた先頭アドレス

    u32 srv_ = 0;   //!< SRVのDescriptorHeapインデックス番号
    u32 rtv_ = 0;
    u32 dsv_ = 0;
    u32 uav_ = 0;
};

//---------------------------------------------------------------------------
// 初期化
//---------------------------------------------------------------------------
bool BufferImpl::initialize(void* initializeData)
{
    //------------------------------------------------------
    // GPUメモリの確保 (64KB単位の確保)
    //------------------------------------------------------
    {
        auto heapProperties = CD3DX12_HEAP_PROPERTIES(desc_.heapType_);            // メモリの配置場所
        auto resourceDesc   = CD3DX12_RESOURCE_DESC::Buffer(desc_.sizeInBytes_);   // byteサイズ
        if(FAILED(gpu::d3dDevice()->CreateCommittedResource(
               &heapProperties,
               D3D12_HEAP_FLAG_NONE,
               &resourceDesc,
               D3D12_RESOURCE_STATE_COPY_DEST,   // メモリの属性(DMA転送先)
               nullptr,
               IID_PPV_ARGS(&d3d12Resource_)))) {
            return false;
        }
    }

    //---------------------------------------------------------------------
    // 非同期転送
    //---------------------------------------------------------------------

    // UPLOAD_HEAPとして使う場合はこのコピーは不要だがメモリをmapする必要あり。
    if(desc_.heapType_ == D3D12_HEAP_TYPE_UPLOAD) {
        // メモリに配置することで可視状態になる
        CD3DX12_RANGE readRange(0, 0);   // CPU側で読み取らないため範囲指定なし
        if(FAILED(d3d12Resource_->Map(0, &readRange, &mappedAddress_))) {
            return false;
        }
        // 初期化データーのコピー
        if(initializeData) {
            memory::fastCopy(mappedAddress_, initializeData, desc_.sizeInBytes_);
        }
    }
    else {
        // DEFAULT_HEAPの場合はGPU側のメモリに転送
        D3D12_SUBRESOURCE_DATA subresource{ initializeData, 0, 0 };
        gpu::asyncCopy(d3d12Resource_.Get(), &subresource, 1);
    }

#if 0
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
#endif
    return true;
}

//---------------------------------------------------------------------------
//! バッファを作成
//---------------------------------------------------------------------------
std::shared_ptr<gpu::Buffer> createBuffer(const Buffer::Desc& desc, void* initializeData)
{
    //---------------------------------------------------------------------
    // バッファクラスの初期化
    //---------------------------------------------------------------------
    auto p = std::make_shared<gpu::BufferImpl>(desc);

    if(!p->initialize(initializeData)) {
        p.reset();
    }
    return p;
}

}   // namespace gpu
