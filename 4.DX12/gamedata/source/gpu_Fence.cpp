//---------------------------------------------------------------------------
//!	@file	gpu_Fence.cpp
//!	@brief	GPUフェンス
//---------------------------------------------------------------------------

namespace gpu {

//===========================================================================
//! フェンス
//===========================================================================
class FenceImpl final : public Fence
{
public:
    FenceImpl(u64 initialValue, bool bindEvent)
    {
        //------------------------------------------------------
        // D3Dフェンスの作成
        //------------------------------------------------------
        if(FAILED(gpu::d3dDevice()->CreateFence(initialValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&d3d12Fence_)))) {
            return;
        }

        //------------------------------------------------------
        // 連動イベントの作成
        //------------------------------------------------------
        if(bindEvent) {
            // 自動シグナル制御、初期値は非シグナル状態
            handle_ = CreateEvent(nullptr, false, false, nullptr);
        }
    }

    virtual ~FenceImpl()
    {
        // イベントの解放
        if(handle_) {
            CloseHandle(handle_);
        }
    }

    // CPUがフェンスを待つ
    void waitForValue(u64 waitValue)
    {
        // フェンスの値を待つときにイベント連動
        d3d12Fence_->SetEventOnCompletion(waitValue, handle_);

        // GPUからのイベントを待つ
        WaitForSingleObject(handle_, INFINITE);
    }

    //----------------------------------------------------------
    //! @name 参照
    //----------------------------------------------------------
    //@{

    //! D3D12フェンスを取得
    virtual ID3D12Fence* d3d12Fence() const { return d3d12Fence_.Get(); }

    //! イベントハンドル
    virtual HANDLE event() const { return handle_; }

    //@}

    // このオブジェクトが有効かどうかを取得
    operator bool()
    {
        return d3d12Fence_;
    }

private:
    com_ptr<ID3D12Fence> d3d12Fence_;         //!< D3D12フェンス
    HANDLE               handle_ = nullptr;   //!< イベントハンドル
};

//---------------------------------------------------------------------------
//! フェンスの作成
//---------------------------------------------------------------------------
std::shared_ptr<gpu::Fence> createFence(u64 initialValue, bool bindEvent)
{
    auto p = std::make_shared<gpu::FenceImpl>(initialValue, bindEvent);
    if(*p == false) {
        p.reset();
    }
    return p;
}

}   // namespace gpu