#pragma once
//---------------------------------------------------------------------------
//!	@file	gpu_SwapChain.h
//!	@brief	GPUスワップチェイン
//---------------------------------------------------------------------------

namespace gpu {

//===========================================================================
//! スワップチェイン
//===========================================================================
class SwapChain
{
public:
    virtual ~SwapChain() = default;

        //! 画面更新
    //! @param  [in]    vsyncInterval   VSYNCの間隔(0:ノーウェイト 1:60fps 2:30fps 3:20fps 4:15fps)
    virtual void present(u32 vsyncInterval) = 0;

    //! GPUが実行完了になるまで待つ
    //! 完全にアイドル状態になるまでブロックします
    virtual void waitForGpu() = 0;

    //! フルスクリーンとウィンドウモードの切り替え
    virtual void toggleFullScreen() = 0;

    //----------------------------------------------------------
    //! @name 参照
    //----------------------------------------------------------
    //@{

    //! 現在のフレームインデックス番号を取得
    //! @example bufferCount=3のときは 0,1,2, 0,1,2 のように変化します
    virtual u32 frameIndex() const = 0;

    //! DXGIスワップチェインを取得
    virtual IDXGISwapChain3* dxgiSwapChain() const = 0;

    //! バックバッファを取得
    virtual ID3D12Resource* backBuffer(u32 index) const = 0;

    //@}
protected:
    // new禁止
    SwapChain() = default;
};
    
//! スワップチェインの作成
//! @param  [in]    width       幅
//! @param  [in]    height      高さ
//! @param  [in]    dxgiFormat  ピクセル形式
//! @param  [in]    bufferCount バッファ数( 2 or 3)
std::unique_ptr<gpu::SwapChain> createSwapChain(u32 width, u32 height, DXGI_FORMAT dxgiFormat, u32 bufferCount);


}   // namespace gpu