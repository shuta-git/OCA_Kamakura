//---------------------------------------------------------------------------
//!	@file	gpu_DepscriptorHeap.cpp
//!	@brief	Descriptorヒープ管理
//---------------------------------------------------------------------------

namespace gpu {

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
DescriptorHeap::DescriptorHeap(u32 count, D3D12_DESCRIPTOR_HEAP_TYPE type)
{
    // シェーダーから可視のViewはシェーダースロットに設定するオブジェクトが対象
    bool shaderVisible = false;
    if(type == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
        shaderVisible = true;
    if(type == D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER)
        shaderVisible = true;

    // Descriptor用ヒープ作成
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
    heapDesc.NumDescriptors = count;   // 個数
    heapDesc.Type           = type;    // DSV指定
    heapDesc.Flags          = (shaderVisible) ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    if(FAILED(gpu::d3dDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap_)))) {
        return;
    }

    cpuBaseAddress_ = heap_->GetCPUDescriptorHandleForHeapStart();
    gpuBaseAddress_ = heap_->GetGPUDescriptorHandleForHeapStart();

    // 要素分のサイズを取得
    stride_ = gpu::d3dDevice()->GetDescriptorHandleIncrementSize(type);
}

//---------------------------------------------------------------------------
//! 配列
//---------------------------------------------------------------------------
DescriptorHeap::Address DescriptorHeap::operator[](u32 index) const
{
    //    descriptor 1要素あたりのサイズ stride_
    //           |←-→|
    //
    //           +---+---+---+
    //  heap_    |   |   |   |
    //           +---+---+---+
    //            [0] [1] [2]
    //             ↑   ↑   ↑　index
    auto offset = stride_ * index;   // 先頭からのオフセット位置

    auto cpuAddress = cpuBaseAddress_;
    auto gpuAddress = gpuBaseAddress_;
    cpuAddress.ptr += offset;
    gpuAddress.ptr += offset;

    return { cpuAddress, gpuAddress };
}

//---------------------------------------------------------------------------
//! 配列
//---------------------------------------------------------------------------
DescriptorHeap::Address DescriptorHeap::at(u32 index) const
{
    return operator[](index);
}

//! 初期化されているかどうかを取得
DescriptorHeap::operator bool() const
{
    return heap_.Get() != nullptr;
}

//! 配列の先頭アドレスを取得
DescriptorHeap::Address DescriptorHeap::data()
{
    return operator[](0);
}

//! 配列の先頭アドレスを取得(const)
DescriptorHeap::Address DescriptorHeap::data() const
{
    return operator[](0);
}

}   // namespace gpu
