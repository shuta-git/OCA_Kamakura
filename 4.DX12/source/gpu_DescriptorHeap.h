//---------------------------------------------------------------------------
//!	@file	gpu_DepscriptorHeap.h
//!	@brief	Descriptorヒープ管理
//---------------------------------------------------------------------------
#pragma once

namespace gpu {

//===========================================================================
// Descriptorのメモリ管理
//===========================================================================
class DescriptorHeap final
{
private:
    // コピー禁止
    DescriptorHeap(const DescriptorHeap&) = delete;
    DescriptorHeap& operator=(const DescriptorHeap&) = delete;

public:
    DescriptorHeap(DescriptorHeap&& lhs)
    {
        operator=(std::move(lhs));
    }
    DescriptorHeap& operator=(DescriptorHeap&& lhs)
    {
        heap_           = std::move(lhs.heap_);
        stride_         = lhs.stride_;
        cpuBaseAddress_ = lhs.cpuBaseAddress_;
        gpuBaseAddress_ = lhs.gpuBaseAddress_;

        return *this;
    }

    // CPUとGPUアドレスを自動的に切り替え
    struct Address
    {
        // CPUアドレスにキャスト
        operator D3D12_CPU_DESCRIPTOR_HANDLE() const
        {
            return cpuAddress_;
        }
        // GPUアドレスにキャスト
        operator D3D12_GPU_DESCRIPTOR_HANDLE() const
        {
            return gpuAddress_;
        }

        D3D12_CPU_DESCRIPTOR_HANDLE cpuAddress_ = {};   //!< CPUアドレス
        D3D12_GPU_DESCRIPTOR_HANDLE gpuAddress_ = {};   //!< GPUアドレス
    };

    DescriptorHeap() = default;

    //! 初期化
    //! @param  [in]    count   配列個数
    //! @param  [in]    type    Viewの種類
    DescriptorHeap(u32 count, D3D12_DESCRIPTOR_HEAP_TYPE type);

    //! 配列
    Address operator[](u32 index) const;

    //! 配列
    Address at(u32 index) const;

    //! 初期化されているかどうかを取得
    operator bool() const;

    //! 配列の先頭アドレスを取得
    Address data();

    //! 配列の先頭アドレスを取得(const)
    Address data() const;

    //! D3D12ヒープを取得
    ID3D12DescriptorHeap* get() const { return heap_.Get(); }

private:
    com_ptr<ID3D12DescriptorHeap> heap_;                 // Descriptor配列
    u32                           stride_         = 0;   // Descriptor構造体1個あたりのバイトサイズ
    D3D12_CPU_DESCRIPTOR_HANDLE   cpuBaseAddress_ = {};
    D3D12_GPU_DESCRIPTOR_HANDLE   gpuBaseAddress_ = {};
};

}   // namespace gpu