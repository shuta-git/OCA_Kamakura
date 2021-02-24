//---------------------------------------------------------------------------
//!	@file	gpu_SwapChain.cpp
//!	@brief	GPUスワップチェイン
//---------------------------------------------------------------------------

namespace gpu {

//===========================================================================
//! シェーダー内部実装
//===========================================================================
class SwapChainImpl final : public gpu::SwapChain
{
public:
    // シェーダー種類
    enum class Type
    {
        Unknown,   //!< 未定義
        Vs,        //!< 頂点シェーダー
        Ps,        //!< ピクセルシェーダー
        Cs,        //!< 演算シェーダー（コンピュートシェーダー）
    };

    //! コンストラクタ
    SwapChainImpl() = default;

    // デストラクタ
    virtual ~SwapChainImpl()
    {
        if(fenceEvent_) {
            CloseHandle(fenceEvent_);
        }
    }

    //! 初期化
    bool initialize(u32 width, u32 height, DXGI_FORMAT dxgiFormat, u32 bufferCount);

    //! 画面更新
    virtual void present(u32 vsyncInterval) override
    {
        dxgiSwapChain_->Present(vsyncInterval, 0);
        // 次のフレームの準備
        moveToNextFrame();
    }

        //! GPUが実行完了になるまで待つ
    //! 完全にアイドル状態になるまでブロックします
    virtual void waitForGpu() override;

    //! 次のフレームを描画する準備
    void moveToNextFrame();

    //! フルスクリーンとウィンドウモードの切り替え
    virtual void toggleFullScreen() override;

    //----------------------------------------------------------
    //! @name 参照
    //----------------------------------------------------------
    //@{

    //! 現在のフレームインデックス番号を取得
    //! @example bufferCount=3のときは 0,1,2, 0,1,2 のように変化します
    virtual u32 frameIndex() const override
    {
        return frameIndex_;
    }

    //! DXGIスワップチェインを取得
    virtual IDXGISwapChain3* dxgiSwapChain() const override
    {
        return dxgiSwapChain_.Get();
    }

    //! バックバッファを取得
    virtual ID3D12Resource* backBuffer(u32 index) const override
    {
        return backBuffers_[index].Get();
    }

    //@}
private:
    // コピー禁止/move禁止
    SwapChainImpl(const SwapChainImpl&) = delete;
    SwapChainImpl(SwapChainImpl&&)      = delete;
    SwapChainImpl& operator=(const SwapChainImpl&) = delete;
    SwapChainImpl& operator=(SwapChainImpl&&) = delete;

private:

    u32 width_       = 0;           //!< 幅
    u32 height_      = 0;           //!< 高さ
    u32 bufferCount_ = 0;           //!< バッファ数
    bool isFullScreen_   = false;   //!< フルスクリーンモードかどうか
    RECT windowPosition_ = {};      //!< ウィンドウモード時のウィンドウ位置

    com_ptr<IDXGISwapChain3>             dxgiSwapChain_;   //!< スワップチェイン
    std::vector<com_ptr<ID3D12Resource>> backBuffers_;     //!< バックバッファ


    //----------------------------------------------------------
    //! @name GPU同期オブジェクト
    //----------------------------------------------------------
    //@{

    u32                  frameIndex_ = 0;         //!< フレーム番号(FRAME_COUNTでループ)
    HANDLE               fenceEvent_ = nullptr;   //!< フレーム同期用のイベントハンドル
    com_ptr<ID3D12Fence> d3d12Fence_;             //!< GPUフェンス
    std::vector<u64>     fenceValues_;            //!< フェンス用の値

    //@}

};

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool SwapChainImpl::initialize(u32 width,u32 height,DXGI_FORMAT dxgiFormat, u32 bufferCount)
{
    width_ = width;
    height_ = height;
    bufferCount_ = bufferCount;

    fenceValues_.resize(bufferCount);

    //----------------------------------------------------------
    //! スワップチェインの作成
    //----------------------------------------------------------
    {
        DXGI_SWAP_CHAIN_DESC1 desc = {};
        desc.Width                 = width_;                                   // 幅
        desc.Height                = height_;                                  // 高さ
        desc.Format                = DXGI_FORMAT_R8G8B8A8_UNORM;               // ピクセル形式
        desc.Stereo                = false;                                    // 立体視出力
        desc.SampleDesc.Count      = 1;                                        // マルチサンプル数
        desc.SampleDesc.Quality    = 0;                                        // マルチサンプル品質
        desc.BufferUsage           = DXGI_USAGE_BACK_BUFFER;                   // バッファ用途
        desc.BufferCount           = 2;                                        // バックバッファ数
        desc.Scaling               = DXGI_SCALING_STRETCH;                     // バックバッファは伸び縮み可能
        desc.SwapEffect            = DXGI_SWAP_EFFECT_FLIP_DISCARD;            // Swap処理タイプ
        desc.AlphaMode             = DXGI_ALPHA_MODE_UNSPECIFIED;              // アルファの種類
        desc.Flags                 = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;   // フラグ

        com_ptr<IDXGISwapChain1> swapChain;
        // フルスクリン設定はいまは書かない
        if(FAILED(gpu::dxgiFactory()->CreateSwapChainForHwnd(
               gpu::render()->commandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT), // [in]  キューの強制フラッシュに対応するなどでコマンドキューが必要
               application::hwnd(),                                         // [in]　ウィンドウハンドル
               &desc,                                                       // [in]　オプション
               nullptr,                                                     // [in]　フルスクリーン表示オプション
               nullptr,                                                     // [in]　出力先（コンテンツを出力ターゲットに制限しない場合はnullptr）
               &swapChain))) {                                              // [out] 生成されたスワップチェイン
            return false;
        }

        if(FAILED(swapChain.As(&dxgiSwapChain_))) {
            return false;
        }
        // 現在のフレームインデックス値を取得
        frameIndex_ = dxgiSwapChain_->GetCurrentBackBufferIndex();
    }

    //-------------------------------------------------
    // スワップチェインからバックバッファのResourceを取得
    //-------------------------------------------------
    for(u32 n = 0; n < bufferCount; ++n) {
        com_ptr<ID3D12Resource> buffer;

        // ここで参照カウンタが増えているため注意
        if(FAILED(dxgiSwapChain_->GetBuffer(n, IID_PPV_ARGS(&buffer)))) {
            return false;
        }
        backBuffers_.push_back(buffer);
    }

    //-------------------------------------------------
    // GPU同期オブジェクトの作成
    //-------------------------------------------------
    {
        // GPUフェンスの作成
        if(FAILED(gpu::d3dDevice()->CreateFence(fenceValues_[frameIndex_], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&d3d12Fence_)))) {
            return false;
        }
        fenceValues_[frameIndex_]++;   // 初期値

        // フレーム同期用のイベントハンドルを取得
        fenceEvent_ = CreateEvent(nullptr, false, false, nullptr);
        if(fenceEvent_ == nullptr) {
            return false;
        }

        waitForGpu();
    }

    return true;
}

//---------------------------------------------------------------------------
//! フルスクリーンとウィンドウモードの切り替え
//---------------------------------------------------------------------------
void SwapChainImpl::toggleFullScreen()
{
    isFullScreen_ = !isFullScreen_;
    auto hwnd     = application::hwnd();

    constexpr LONG toggleStyle = WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

    auto style = GetWindowLong(hwnd, GWL_STYLE);
    SetWindowLong(hwnd, GWL_STYLE, style ^ toggleStyle);

    HWND windowOrder = HWND_NOTOPMOST;
    s32  x, y, w, h;
    if(isFullScreen_) {
        //------------------------------------------------------
        // フルスクリーンモード
        //------------------------------------------------------
        x = 0;
        y = 0;
        w = GetSystemMetrics(SM_CXSCREEN);
        h = GetSystemMetrics(SM_CYSCREEN);

        windowOrder = HWND_TOP;   // 最前面

        //------------------------------------------------------
        // マルチモニターのフルスクリーン対応
        // 利便性の向上。現在のウィンドウ位置のモニターでフルスクリーン化
        //------------------------------------------------------
        constexpr bool supportForMultiMonitor = true;

        if constexpr(supportForMultiMonitor) {
            // 現在のモニターを列挙
            std::vector<RECT> monitorRects;

            // ※Windowsコールバックにラムダ式を渡す場合はキャプチャ指定不可
            MONITORENUMPROC MyInfoEnumProc = [](HMONITOR monitor, [[maybe_unused]] HDC hdcMonitor, RECT* rectMonitor, LPARAM args) -> BOOL {
                MONITORINFOEX monitorInfo{};
                monitorInfo.cbSize = sizeof(monitorInfo);
                GetMonitorInfo(monitor, &monitorInfo);

                auto* monitorRects = reinterpret_cast<std::vector<RECT>*>(args);

                if(monitorInfo.dwFlags == DISPLAY_DEVICE_MIRRORING_DRIVER) {
                    return true;
                }
                else {
                    monitorRects->push_back(*rectMonitor);
                };
                return true;
            };

            // モニター列挙
            EnumDisplayMonitors(nullptr, nullptr, MyInfoEnumProc, reinterpret_cast<LPARAM>(&monitorRects));

            // 元に戻すときのために現在のウィンドウの位置とサイズを保存
            GetWindowRect(hwnd, &windowPosition_);

            // 中心座標を計算
            s32 centerX = (windowPosition_.left + windowPosition_.right) >> 1;
            s32 centerY = (windowPosition_.bottom + windowPosition_.top) >> 1;

            // 現在のウィンドウ中心位置がどのモニター上にあるか判定してフルスクリーン化するモニターを選択
            auto rect = std::find_if(monitorRects.begin(),
                                     monitorRects.end(),
                                     [&](RECT& r) { return r.left <= centerX && centerX < r.right &&
                                                           r.top <= centerY && centerY < r.bottom; });
            if(rect != monitorRects.end()) {
                x = (*rect).left;
                y = (*rect).top;
                w = (*rect).right - (*rect).left;
                h = (*rect).bottom - (*rect).top;
            }
        }
    }
    else {
        //----------------------------------------------
        // ウィンドウモード
        //----------------------------------------------
        // 元に戻す
        x = windowPosition_.left;
        y = windowPosition_.top;
        w = windowPosition_.right - windowPosition_.left;
        h = windowPosition_.bottom - windowPosition_.top;
    }
    // ウィンドウ位置と優先度を更新
    SetWindowPos(hwnd,                                    // [in] ウィンドウハンドル
                 windowOrder,                             // [in] ウィンドウの表示優先順位
                 x, y,                                    // [in] 位置
                 w, h,                                    // [in] 幅高さ
                 SWP_FRAMECHANGED | SWP_NOOWNERZORDER);   // [in] 変更対象の選択フラグ (ウィンドウスタイルの変更)
}

//---------------------------------------------------------------------------
//! GPUが実行完了になるまで待つ
//---------------------------------------------------------------------------
void SwapChainImpl::waitForGpu()
{
    //----------------------------------------------------------
    // シグナルコマンドをキューに登録
    //----------------------------------------------------------
    const u64 currentFenceValue = fenceValues_[frameIndex_];
    auto* d3d12CommandQueue = gpu::render()->commandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);
    if(FAILED(d3d12CommandQueue->Signal(d3d12Fence_.Get(),currentFenceValue))) {
        assert(0);
    }

    //----------------------------------------------------------
    // GPUフェンスがシグナルコマンドが来るまで待つ
    //----------------------------------------------------------
    if(FAILED(d3d12Fence_->SetEventOnCompletion(fenceValues_[frameIndex_],fenceEvent_))) {
        assert(0);
    }
    WaitForSingleObjectEx(fenceEvent_, INFINITE, false);

    // 現在のフレームのフェンス値をインクリメント
    fenceValues_[frameIndex_]++;
}

//---------------------------------------------------------------------------
//! 次にフレームを描画する準備
//---------------------------------------------------------------------------
void SwapChainImpl::moveToNextFrame()
{
    //----------------------------------------------------------
    // シグナルコマンドをキューに登録
    //----------------------------------------------------------
    const u64 currentFenceValue = fenceValues_[frameIndex_];
    auto*     d3d12CommandQueue = gpu::render()->commandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);
    if(FAILED(d3d12CommandQueue->Signal(d3d12Fence_.Get(), currentFenceValue))) {
        assert(0);
    }

    // フレームインデックスを更新
    frameIndex_ = dxgiSwapChain_->GetCurrentBackBufferIndex();

    // 次のフレームがまだレンダリングする準備ができていない場合は準備ができるまで待ちます。
    if(d3d12Fence_->GetCompletedValue() < fenceValues_[frameIndex_]) {
        if(FAILED(d3d12Fence_->SetEventOnCompletion(fenceValues_[frameIndex_],fenceEvent_))) {
            assert(0);
        }
        WaitForSingleObjectEx(fenceEvent_, INFINITE, false);
    }

    // 次のフレームのフェンス値を設定
    fenceValues_[frameIndex_] = currentFenceValue + 1;
}

//---------------------------------------------------------------------------
//! スワップチェインの作成
//---------------------------------------------------------------------------
std::unique_ptr<gpu::SwapChain> createSwapChain(u32 width,u32 height,DXGI_FORMAT dxgiFormat,u32 bufferCount)
{
    auto p = std::make_unique<gpu::SwapChainImpl>();

    if(!p->initialize(width,height,dxgiFormat , bufferCount)) {
        p.reset();
    }
    return p;
}


}   // namespace gpu