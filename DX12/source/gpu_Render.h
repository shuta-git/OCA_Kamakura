#pragma once
//---------------------------------------------------------------------------
//!	@file	gpu_Render.h(仮名)
//!	@brief	gpu全般
//---------------------------------------------------------------------------

// 前方宣言
class IndexAllocator;

namespace gpu {

//! 前方宣言
class SwapChain;

class Render
{
public:
    //! 初期化
    virtual bool initialize(u32 width, u32 height) = 0;

    //! 更新
    virtual void update() = 0;

    //! 描画
    virtual void render() = 0;

    //! 解放
    virtual void finalize() = 0;

    //! コマンドリストを取得
    virtual ID3D12GraphicsCommandList* graphicsCommandList() const = 0;

    //! コマンドキューを取得
    //! @param [in]                 コマンドキューの種類
    //! - D3D12_COMMAND_LIST_TYPE_DIRECT  通常キュー
    //! - D3D12_COMMAND_LIST_TYPE_COMPUTE Computeキュー
    //! - D3D12_COMMAND_LIST_TYPE_COPY    コピーキュー
    virtual ID3D12CommandQueue* commandQueue(D3D12_COMMAND_LIST_TYPE type) const = 0;

    //! スワップチェインを取得
    virtual gpu::SwapChain* swapChain() const = 0;

    //! 実体を取得
    static Render* instance();

    //! IMGUIのヒープを取得
    virtual com_ptr<ID3D12DescriptorHeap> GetHeapForImgui() const = 0;

private:

    //! 
    virtual com_ptr<ID3D12DescriptorHeap> CreateDescriptorHeapForImgui() = 0;

protected:
    // new禁止
    Render() = default;
    virtual ~Render() = default;
};
    
//! レンダリング管理クラスを取得
gpu::Render* render();

//! DXGIFactoryを取得
IDXGIFactory6* dxgiFactory();

//! D3Dデバイスを取得
ID3D12Device* d3dDevice();

// DescriptorHeapを取得
gpu::DescriptorHeap* descriptorHeap(DESCRIPTOR_TYPE type);

// インデックスアロケーターを取得
IndexAllocator* indexAllocator(DESCRIPTOR_TYPE type);

}   // namespace gpu