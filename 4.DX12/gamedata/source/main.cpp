//---------------------------------------------------------------------------
//!	@file	main.cpp
//!	@brief	�A�v���P�[�V�������C��
//---------------------------------------------------------------------------
#include"gpu_Render.h"

// ������Ԃ̃t���X�N���[�����[�h
constexpr bool FULLSCREEN_MODE = false;

namespace {
HWND hwnd_          = nullptr;   //!< �E�B���h�E�n���h��
u32  mouseDragging_ = 0;         //!< �E�B���h�E��Ń}�E�X���h���b�O���Ă��邩�ǂ����@bit0:���{�^�� bit1:�E�{�^�� bit2:�����{�^��
}   // namespace

//---------------------------------------------------------------------------
//! �E�B���h�E�v���V�[�W���\
//!	@param	[in]	hwnd	�E�B���h�E�n���h��
//!	@param	[in]	message	�E�B���h�E���b�Z�[�W
//!	@param	[in]	wparam	�p�����[�^�[1
//!	@param	[in]	lparam	�p�����[�^�[2
//!	@return �E�B���h�E�v���V�[�W���̂��ꂼ��̃��b�Z�[�W������̒l
//---------------------------------------------------------------------------
extern LRESULT   ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch(message) {
        case WM_CREATE:   // �E�B���h�E������(�R���X�g���N�^����)
        {
            // ������GWLP_USERDATA��this�|�C���^��ۑ�
            auto* cs = reinterpret_cast<CREATESTRUCT*>(lparam);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(cs->lpCreateParams));
            return 0;
        }

        case WM_SYSKEYDOWN:
        {
            auto vk = wparam;          // ���݉�����Ă��鉼�z�L�[�R�[�h
            if(lparam & (1 << 29)) {   // Alt�L�[��������Ă��邩�ǂ���
                // Alt+Enter
                if(vk == VK_RETURN) {
                    // �t���X�N���[�����[�h��؂�ւ�
                    auto* gpuSwapChain = gpu::render()->swapChain();
                    gpuSwapChain->toggleFullScreen();
                    return 0;
                }
            }
            break;
        }

        // �}�E�X���{�^����������
        case WM_LBUTTONDOWN:
            mouseDragging_ |= 1 << 0;
            SetCapture(hwnd);
            break;
        // �}�E�X�E�{�^����������
        case WM_RBUTTONDOWN:
            mouseDragging_ |= 1 << 1;
            SetCapture(hwnd);
            break;
        // �}�E�X�����{�^����������
        case WM_MBUTTONDOWN:
            mouseDragging_ |= 1 << 2;
            SetCapture(hwnd);
            break;

            // �}�E�X���{�^���𗣂���
        case WM_LBUTTONUP:
            mouseDragging_ &= ~(1 << 0);
            if(mouseDragging_ == 0) {
                ReleaseCapture();
            }
            break;
            // �}�E�X�E�{�^���𗣂���
        case WM_RBUTTONUP:
            mouseDragging_ &= ~(1 << 1);
            if(mouseDragging_ == 0) {
                ReleaseCapture();
            }
            break;
            // �}�E�X�����{�^���𗣂���
        case WM_MBUTTONUP:
            mouseDragging_ &= ~(1 << 2);
            if(mouseDragging_ == 0) {
                ReleaseCapture();
            }
            break;

        case WM_PAINT:   // �ĕ`��
            gpu::render()->update();
            gpu::render()->render();
            return 0;

        case WM_DESTROY:   // �E�B���h�E�j����(�f�X�g���N�^����)
            PostQuitMessage(0);
            return 0;
    }

    // ImGui�̃E�B���h�E�v���V�[�W��
    ImGui_ImplWin32_WndProcHandler(hwnd, message, wparam, lparam);
    //-------------------------------------------------------------
    // �f�t�H���g�̃E�B���h�E�v���V�[�W��
    // �����Ӂ� UNICODE��CreateWindowW�𗘗p����ꍇ�͂��̊֐���UNICODE�łɂ���K�v����
    //-------------------------------------------------------------
    return DefWindowProcW(hwnd, message, wparam, lparam);
}

//---------------------------------------------------------------------------
// ���C������
//---------------------------------------------------------------------------
int run()
{
    CoInitialize(NULL);

    // ��DPI���[�h�Ή�
    // �E�B���h�EUI�̂�DPI�X�P�[�����O����A�N���C�A���g�̈��dot-by-dot
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    //============================================================================================================
    //�@�E�B���h�E�쐬
    //============================================================================================================
    //-----------------------------------------------------------------------
    // �E�B���h�E�N���X�̍쐬
    //-----------------------------------------------------------------------
    WNDCLASSEXW windowClass{};
    windowClass.cbSize        = sizeof(windowClass);            // �\���̃T�C�Y
    windowClass.style         = CS_HREDRAW | CS_VREDRAW;        // �N���X�X�^�C��
    windowClass.lpfnWndProc   = &WindowProcedure;               // �R�[���o�b�N�֐��̎w��(�E�B���h�E�v���V�[�W��)
    windowClass.hInstance     = application::instance();        // �n���h���̎擾
    windowClass.hCursor       = LoadCursor(NULL, IDC_CROSS);    // �J�[�]���̎��
    windowClass.lpszClassName = L"DX12SampleClass";             // �A�v���P�[�V�����N���X��(�K���ł����ł�)
    
    RegisterClassExW(&windowClass);   // �A�v���P�[�V�����N���X(���������̍�邩���낵������OS�ɗ\������)
   
    //----------------------------------------------------------
    // �E�B���h�E���쐬
    //----------------------------------------------------------
    u32 style   = WS_OVERLAPPEDWINDOW;   // �E�B���h�E�X�^�C��
    u32 styleEx = 0;                     // �g���E�B���h�E�X�^�C��
    s32 x       = CW_USEDEFAULT;
    s32 y       = CW_USEDEFAULT;
    s32 w       = CW_USEDEFAULT;
    s32 h       = CW_USEDEFAULT;
    
    //----------------------------------------------------------
    // �E�B���h�E�T�C�Y��UI�T�C�Y���l�����ĕ␳
    //----------------------------------------------------------
    {
        RECT windowRect{ 0, 0, 1920, 1080 };

        AdjustWindowRectEx(&windowRect, style, false, styleEx);
        w = windowRect.right - windowRect.left;
        h = windowRect.bottom - windowRect.top;
    }

    //-----------------------------------------------------------------------
    // �E�B���h�E�쐬
    //-----------------------------------------------------------------------
    hwnd_ = CreateWindowExW(
        styleEx,
        windowClass.lpszClassName,   // �N���X���w��
        L"Direct3D12 �T���v��",      // �^�C�g���o�[�̕���
        style,                       // �^�C�g���o�[�Ƌ��E��������E�B���h�E�ł�
        x,                           // �\��X���W��OS�ɂ��C�����܂�
        y,                           // �\��Y���W��OS�ɂ��C�����܂�
        w,                           // �E�B���h�E��
        h,                           // �E�B���h�E��
        nullptr,                     // �e�E�B���h�E�n���h��
        nullptr,                     // ���j���[�n���h��
        application::instance(),     // �Ăяo���A�v���P�[�V�����n���h��
        nullptr);                    // �ǉ��p�����[�^

    //-----------------------------------------------------------------------
    // ������
    //-----------------------------------------------------------------------
    if(!gpu::render()->initialize(1920, 1080)) {
        return false;
    }

     if constexpr(FULLSCREEN_MODE) {
        auto swapChain = gpu::render()->swapChain();
        swapChain->toggleFullScreen();
    }


    if(!ImGui::CreateContext()) {
        assert(0);
        return false;
    }

    bool blnResult = ImGui_ImplWin32_Init(hwnd_);
    if(!blnResult) {
        assert(0);
        return false;
    }

    blnResult = ImGui_ImplDX12_Init(
        gpu::d3dDevice(),
        3,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        gpu::render()->GetHeapForImgui().Get(),
        gpu::render()->GetHeapForImgui()->GetCPUDescriptorHandleForHeapStart(),
        gpu::render()->GetHeapForImgui()->GetGPUDescriptorHandleForHeapStart());


    // �E�B���h�E�\��
    {
        // GetWindowPlacement����WinMain�̈�����nCmdShow���擾�\
        WINDOWPLACEMENT placement;
        GetWindowPlacement(application::hwnd(), &placement);
        ShowWindow(application::hwnd(), placement.showCmd);
    }

    //=============================================================
    // ���C�����b�Z�[�W���[�v
    //=============================================================
    MSG msg{};
    while(msg.message != WM_QUIT) {
        if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    //----------------------------------------------------------
    // ���
    //----------------------------------------------------------
    gpu::render()->finalize();

    // WM_QUIT���b�Z�[�W�̖߂�l���I���R�[�h�Ƃ��ĕԂ�
    return static_cast<int>(msg.wParam);
}

namespace application {

//---------------------------------------------------------------------------
//! �E�B���h�E�n���h�����擾
//---------------------------------------------------------------------------
HWND hwnd()
{
    return hwnd_;
}

//---------------------------------------------------------------------------
//! �A�v���P�[�V�����C���X�^���X�n���h�����擾
//---------------------------------------------------------------------------
HINSTANCE instance()
{
    return GetModuleHandle(nullptr);
}

//---------------------------------------------------------------------------
//! �}�E�X�t�H�[�J�X�������Ă��邩�ǂ���
//---------------------------------------------------------------------------
bool hasMouseFocus()
{
    return mouseDragging_ != 0;
}

}   // namespace application

//===========================================================================
//! �A�v���P�[�V�����J�n�_
//===========================================================================
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    return run();
}