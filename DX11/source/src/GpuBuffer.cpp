//===========================================================================
//! @file   GpuBuffer.cpp
//!	@brief	GPUバッファー
//===========================================================================
namespace gpu {

//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
Buffer::Buffer()
: d3dBuffer_(nullptr)
{
}

//---------------------------------------------------------------------------
//! デストラクタ
//---------------------------------------------------------------------------
Buffer::~Buffer()
{
    if(d3dBuffer_) {
        d3dBuffer_->Release();
    }
}

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool Buffer::initialize(size_t size, D3D11_USAGE usage, D3D11_BIND_FLAG bindFlags, const void* initialData)
{
    D3D11_BUFFER_DESC desc{};
    desc.ByteWidth      = (size + 15) & ~(16ul - 1);   // バッファのサイズ(16の倍数)
    desc.Usage          = usage;                       // バッファの配置場所(VRAM内)
    desc.BindFlags      = bindFlags;                   // 頂点バッファとして設定
    desc.CPUAccessFlags = (usage == D3D11_USAGE_DYNAMIC) ? D3D11_CPU_ACCESS_WRITE : 0;

    // バッファの初期データを渡して頂点バッファを作成
    D3D11_SUBRESOURCE_DATA initData{};
    initData.pSysMem = initialData;   // 初期データの先頭アドレス

    if(FAILED(D3DDevice()->CreateBuffer(&desc, initialData ? &initData : nullptr, &d3dBuffer_))) {
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------
//! メモリマップ開始
//---------------------------------------------------------------------------
void* Buffer::map()
{
    // バッファをmapする
    auto                     d3dBuffer = getD3DBuffer();
    D3D11_MAPPED_SUBRESOURCE data;
    D3DContext()->Map(d3dBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

    return data.pData;
}

//---------------------------------------------------------------------------
//! メモリマップ終了
//---------------------------------------------------------------------------
void Buffer::unmap()
{
    // バッファをunmapする
    auto d3dBuffer = getD3DBuffer();
    D3DContext()->Unmap(d3dBuffer, 0);
}

}   // namespace gpu
