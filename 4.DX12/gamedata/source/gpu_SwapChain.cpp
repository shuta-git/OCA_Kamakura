//---------------------------------------------------------------------------
//!	@file	gpu_SwapChain.cpp
//!	@brief	GPU�X���b�v�`�F�C��
//---------------------------------------------------------------------------

namespace gpu {

//===========================================================================
//! �V�F�[�_�[��������
//===========================================================================
class SwapChainImpl final : public gpu::SwapChain
{
public:
    // �V�F�[�_�[���
    enum class Type
    {
        Unknown,   //!< ����`
        Vs,        //!< ���_�V�F�[�_�[
        Ps,        //!< �s�N�Z���V�F�[�_�[
        Cs,        //!< ���Z�V�F�[�_�[�i�R���s���[�g�V�F�[�_�[�j
    };

    //! �R���X�g���N�^
    SwapChainImpl() = default;

    // �f�X�g���N�^
    virtual ~SwapChainImpl()
    {
        if(fenceEvent_) {
            CloseHandle(fenceEvent_);
        }
    }

    //! ������
    bool initialize(u32 width, u32 height, DXGI_FORMAT dxgiFormat, u32 bufferCount);

    //! ��ʍX�V
    virtual void present(u32 vsyncInterval) override
    {
        dxgiSwapChain_->Present(vsyncInterval, 0);
        // ���̃t���[���̏���
        moveToNextFrame();
    }

        //! GPU�����s�����ɂȂ�܂ő҂�
    //! ���S�ɃA�C�h����ԂɂȂ�܂Ńu���b�N���܂�
    virtual void waitForGpu() override;

    //! ���̃t���[����`�悷�鏀��
    void moveToNextFrame();

    //! �t���X�N���[���ƃE�B���h�E���[�h�̐؂�ւ�
    virtual void toggleFullScreen() override;

    //----------------------------------------------------------
    //! @name �Q��
    //----------------------------------------------------------
    //@{

    //! ���݂̃t���[���C���f�b�N�X�ԍ����擾
    //! @example bufferCount=3�̂Ƃ��� 0,1,2, 0,1,2 �̂悤�ɕω����܂�
    virtual u32 frameIndex() const override
    {
        return frameIndex_;
    }

    //! DXGI�X���b�v�`�F�C�����擾
    virtual IDXGISwapChain3* dxgiSwapChain() const override
    {
        return dxgiSwapChain_.Get();
    }

    //! �o�b�N�o�b�t�@���擾
    virtual ID3D12Resource* backBuffer(u32 index) const override
    {
        return backBuffers_[index].Get();
    }

    //@}
private:
    // �R�s�[�֎~/move�֎~
    SwapChainImpl(const SwapChainImpl&) = delete;
    SwapChainImpl(SwapChainImpl&&)      = delete;
    SwapChainImpl& operator=(const SwapChainImpl&) = delete;
    SwapChainImpl& operator=(SwapChainImpl&&) = delete;

private:

    u32 width_       = 0;           //!< ��
    u32 height_      = 0;           //!< ����
    u32 bufferCount_ = 0;           //!< �o�b�t�@��
    bool isFullScreen_   = false;   //!< �t���X�N���[�����[�h���ǂ���
    RECT windowPosition_ = {};      //!< �E�B���h�E���[�h���̃E�B���h�E�ʒu

    com_ptr<IDXGISwapChain3>             dxgiSwapChain_;   //!< �X���b�v�`�F�C��
    std::vector<com_ptr<ID3D12Resource>> backBuffers_;     //!< �o�b�N�o�b�t�@


    //----------------------------------------------------------
    //! @name GPU�����I�u�W�F�N�g
    //----------------------------------------------------------
    //@{

    u32                  frameIndex_ = 0;         //!< �t���[���ԍ�(FRAME_COUNT�Ń��[�v)
    HANDLE               fenceEvent_ = nullptr;   //!< �t���[�������p�̃C�x���g�n���h��
    com_ptr<ID3D12Fence> d3d12Fence_;             //!< GPU�t�F���X
    std::vector<u64>     fenceValues_;            //!< �t�F���X�p�̒l

    //@}

};

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool SwapChainImpl::initialize(u32 width,u32 height,DXGI_FORMAT dxgiFormat, u32 bufferCount)
{
    width_ = width;
    height_ = height;
    bufferCount_ = bufferCount;

    fenceValues_.resize(bufferCount);

    //----------------------------------------------------------
    //! �X���b�v�`�F�C���̍쐬
    //----------------------------------------------------------
    {
        DXGI_SWAP_CHAIN_DESC1 desc = {};
        desc.Width                 = width_;                                   // ��
        desc.Height                = height_;                                  // ����
        desc.Format                = DXGI_FORMAT_R8G8B8A8_UNORM;               // �s�N�Z���`��
        desc.Stereo                = false;                                    // ���̎��o��
        desc.SampleDesc.Count      = 1;                                        // �}���`�T���v����
        desc.SampleDesc.Quality    = 0;                                        // �}���`�T���v���i��
        desc.BufferUsage           = DXGI_USAGE_BACK_BUFFER;                   // �o�b�t�@�p�r
        desc.BufferCount           = 2;                                        // �o�b�N�o�b�t�@��
        desc.Scaling               = DXGI_SCALING_STRETCH;                     // �o�b�N�o�b�t�@�͐L�яk�݉\
        desc.SwapEffect            = DXGI_SWAP_EFFECT_FLIP_DISCARD;            // Swap�����^�C�v
        desc.AlphaMode             = DXGI_ALPHA_MODE_UNSPECIFIED;              // �A���t�@�̎��
        desc.Flags                 = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;   // �t���O

        com_ptr<IDXGISwapChain1> swapChain;
        // �t���X�N�����ݒ�͂��܂͏����Ȃ�
        if(FAILED(gpu::dxgiFactory()->CreateSwapChainForHwnd(
               gpu::render()->commandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT), // [in]  �L���[�̋����t���b�V���ɑΉ�����ȂǂŃR�}���h�L���[���K�v
               application::hwnd(),                                         // [in]�@�E�B���h�E�n���h��
               &desc,                                                       // [in]�@�I�v�V����
               nullptr,                                                     // [in]�@�t���X�N���[���\���I�v�V����
               nullptr,                                                     // [in]�@�o�͐�i�R���e���c���o�̓^�[�Q�b�g�ɐ������Ȃ��ꍇ��nullptr�j
               &swapChain))) {                                              // [out] �������ꂽ�X���b�v�`�F�C��
            return false;
        }

        if(FAILED(swapChain.As(&dxgiSwapChain_))) {
            return false;
        }
        // ���݂̃t���[���C���f�b�N�X�l���擾
        frameIndex_ = dxgiSwapChain_->GetCurrentBackBufferIndex();
    }

    //-------------------------------------------------
    // �X���b�v�`�F�C������o�b�N�o�b�t�@��Resource���擾
    //-------------------------------------------------
    for(u32 n = 0; n < bufferCount; ++n) {
        com_ptr<ID3D12Resource> buffer;

        // �����ŎQ�ƃJ�E���^�������Ă��邽�ߒ���
        if(FAILED(dxgiSwapChain_->GetBuffer(n, IID_PPV_ARGS(&buffer)))) {
            return false;
        }
        backBuffers_.push_back(buffer);
    }

    //-------------------------------------------------
    // GPU�����I�u�W�F�N�g�̍쐬
    //-------------------------------------------------
    {
        // GPU�t�F���X�̍쐬
        if(FAILED(gpu::d3dDevice()->CreateFence(fenceValues_[frameIndex_], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&d3d12Fence_)))) {
            return false;
        }
        fenceValues_[frameIndex_]++;   // �����l

        // �t���[�������p�̃C�x���g�n���h�����擾
        fenceEvent_ = CreateEvent(nullptr, false, false, nullptr);
        if(fenceEvent_ == nullptr) {
            return false;
        }

        waitForGpu();
    }

    return true;
}

//---------------------------------------------------------------------------
//! �t���X�N���[���ƃE�B���h�E���[�h�̐؂�ւ�
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
        // �t���X�N���[�����[�h
        //------------------------------------------------------
        x = 0;
        y = 0;
        w = GetSystemMetrics(SM_CXSCREEN);
        h = GetSystemMetrics(SM_CYSCREEN);

        windowOrder = HWND_TOP;   // �őO��

        //------------------------------------------------------
        // �}���`���j�^�[�̃t���X�N���[���Ή�
        // ���֐��̌���B���݂̃E�B���h�E�ʒu�̃��j�^�[�Ńt���X�N���[����
        //------------------------------------------------------
        constexpr bool supportForMultiMonitor = true;

        if constexpr(supportForMultiMonitor) {
            // ���݂̃��j�^�[���
            std::vector<RECT> monitorRects;

            // ��Windows�R�[���o�b�N�Ƀ����_����n���ꍇ�̓L���v�`���w��s��
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

            // ���j�^�[��
            EnumDisplayMonitors(nullptr, nullptr, MyInfoEnumProc, reinterpret_cast<LPARAM>(&monitorRects));

            // ���ɖ߂��Ƃ��̂��߂Ɍ��݂̃E�B���h�E�̈ʒu�ƃT�C�Y��ۑ�
            GetWindowRect(hwnd, &windowPosition_);

            // ���S���W���v�Z
            s32 centerX = (windowPosition_.left + windowPosition_.right) >> 1;
            s32 centerY = (windowPosition_.bottom + windowPosition_.top) >> 1;

            // ���݂̃E�B���h�E���S�ʒu���ǂ̃��j�^�[��ɂ��邩���肵�ăt���X�N���[�������郂�j�^�[��I��
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
        // �E�B���h�E���[�h
        //----------------------------------------------
        // ���ɖ߂�
        x = windowPosition_.left;
        y = windowPosition_.top;
        w = windowPosition_.right - windowPosition_.left;
        h = windowPosition_.bottom - windowPosition_.top;
    }
    // �E�B���h�E�ʒu�ƗD��x���X�V
    SetWindowPos(hwnd,                                    // [in] �E�B���h�E�n���h��
                 windowOrder,                             // [in] �E�B���h�E�̕\���D�揇��
                 x, y,                                    // [in] �ʒu
                 w, h,                                    // [in] ������
                 SWP_FRAMECHANGED | SWP_NOOWNERZORDER);   // [in] �ύX�Ώۂ̑I���t���O (�E�B���h�E�X�^�C���̕ύX)
}

//---------------------------------------------------------------------------
//! GPU�����s�����ɂȂ�܂ő҂�
//---------------------------------------------------------------------------
void SwapChainImpl::waitForGpu()
{
    //----------------------------------------------------------
    // �V�O�i���R�}���h���L���[�ɓo�^
    //----------------------------------------------------------
    const u64 currentFenceValue = fenceValues_[frameIndex_];
    auto* d3d12CommandQueue = gpu::render()->commandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);
    if(FAILED(d3d12CommandQueue->Signal(d3d12Fence_.Get(),currentFenceValue))) {
        assert(0);
    }

    //----------------------------------------------------------
    // GPU�t�F���X���V�O�i���R�}���h������܂ő҂�
    //----------------------------------------------------------
    if(FAILED(d3d12Fence_->SetEventOnCompletion(fenceValues_[frameIndex_],fenceEvent_))) {
        assert(0);
    }
    WaitForSingleObjectEx(fenceEvent_, INFINITE, false);

    // ���݂̃t���[���̃t�F���X�l���C���N�������g
    fenceValues_[frameIndex_]++;
}

//---------------------------------------------------------------------------
//! ���Ƀt���[����`�悷�鏀��
//---------------------------------------------------------------------------
void SwapChainImpl::moveToNextFrame()
{
    //----------------------------------------------------------
    // �V�O�i���R�}���h���L���[�ɓo�^
    //----------------------------------------------------------
    const u64 currentFenceValue = fenceValues_[frameIndex_];
    auto*     d3d12CommandQueue = gpu::render()->commandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);
    if(FAILED(d3d12CommandQueue->Signal(d3d12Fence_.Get(), currentFenceValue))) {
        assert(0);
    }

    // �t���[���C���f�b�N�X���X�V
    frameIndex_ = dxgiSwapChain_->GetCurrentBackBufferIndex();

    // ���̃t���[�����܂������_�����O���鏀�����ł��Ă��Ȃ��ꍇ�͏������ł���܂ő҂��܂��B
    if(d3d12Fence_->GetCompletedValue() < fenceValues_[frameIndex_]) {
        if(FAILED(d3d12Fence_->SetEventOnCompletion(fenceValues_[frameIndex_],fenceEvent_))) {
            assert(0);
        }
        WaitForSingleObjectEx(fenceEvent_, INFINITE, false);
    }

    // ���̃t���[���̃t�F���X�l��ݒ�
    fenceValues_[frameIndex_] = currentFenceValue + 1;
}

//---------------------------------------------------------------------------
//! �X���b�v�`�F�C���̍쐬
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