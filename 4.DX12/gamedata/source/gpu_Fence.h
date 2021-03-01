//---------------------------------------------------------------------------
//!	@file	gpu_Fence.h
//!	@brief	GPUフェンス
//---------------------------------------------------------------------------
#pragma once

namespace gpu {

//===========================================================================
//! フェンス
//===========================================================================
class Fence
{
public:
    virtual ~Fence() = default;

    //----------------------------------------------------------
    //! @name 参照
    //----------------------------------------------------------
    //@{

    //! D3D12フェンスを取得
    virtual ID3D12Fence* d3d12Fence() const = 0;

    //! イベントハンドル
    virtual HANDLE event() const = 0;

    //@}
protected:
    Fence() = default;
};

//! フェンスの作成
//! @param  [in]    initialValue    初期値
//! @param  [in]    bindEvent       true:連動するイベントを生成 false:生成しない
std::shared_ptr<gpu::Fence> createFence(u64 initialValue, bool bindEvent = false);

}   // namespace gpu
