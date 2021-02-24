//===========================================================================
//!
//!	@file	main.cpp
//!	@brief	�A�v���P�[�V�����G���g���[�|�C���g
//!
//===========================================================================

namespace {

    // �V�[���؂�ւ�
    SceneController* scene;

    // �Ɩ��p�X�i�ꎞ�I�ɔz�u�j
    PassLighting  passLighting;
    PassFilterHDR passFilterHDR;

    // FPS
    FPSCounter fps;
    double     fpsCount;

    f32 deltaTime;


}   // namespace

//---------------------------------------------------------------------------
//! ���C�����[�v
//---------------------------------------------------------------------------
void mainLoop()
{

    // ���炩���߃^�C�}�[�̎��g�����擾���Ă���
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    // ���Ԍv���������n�_�ŃJ�E���^���擾
    LARGE_INTEGER start;
    QueryPerformanceCounter(&start);

    //---- imgui�̃t���[���̏�����
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // FPS �X�V
    fpsCount = fps.getFPS();

    {
        scene->update();
        scene->render();

        //=============================================================
        //	�Ɩ�
        //=============================================================
        passLighting.begin();
        passLighting.execute();
        passLighting.end();

        //=============================================================
        // HDR�t�B���^�[
        //=============================================================
        passFilterHDR.begin();
        passFilterHDR.execute();
        passFilterHDR.end();
    }

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    GmRender()->swap(1);

    // ���Ԍv���������I�_�ŃJ�E���^���擾
    LARGE_INTEGER end;
    QueryPerformanceCounter(&end);
    // �J�E���^�̍��������g���Ŋ���Όo�ߎ��Ԃ�b�Ŏ擾�ł���
    LONGLONG span = end.QuadPart - start.QuadPart;
    deltaTime     = (f32)span / (f32)frequency.QuadPart;

}

//---------------------------------------------------------------------------
//	�E�B���h�E�v���V�[�W��
//!	@param	[in]	hWnd	�E�B���h�E�n���h��
//!	@param	[in]	message	�E�B���h�E���b�Z�[�W
//!	@param	[in]	wparam	�p�����[�^1
//!	@param	[in]	lparam	�p�����[�^2
//---------------------------------------------------------------------------

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    if(ImGui_ImplWin32_WndProcHandler(hWnd, message, wparam, lparam))
        return true;

    switch(message) {
        case WM_PAINT:   // WM_PAINT		�E�B���h�E�ĕ`��
        {
            //	PAINTSTRUCT ps;
            //	HDC			hdc = BeginPaint(hWnd, &ps);
            //
            //	EndPaint(hWnd, &ps);
        } break;
        case WM_DESTROY:   //	WM_DESTROY	�E�B���h�E�̔j��

            //-----------------------------------------------------------
            // �����I��
            //-----------------------------------------------------------
            std::quick_exit(EXIT_SUCCESS);

            PostQuitMessage(0);
            return 0;
        default:   // ���̑��f�t�H���g
            break;
    }
    return DefWindowProc(hWnd, message, wparam, lparam);
}

//---------------------------------------------------------------------------
//	�A�v���P�[�V�����G���g���[�|�C���g
//---------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    FileWatcher fileWatcher;

    fileWatcher.initialize("data");

    static const char* titleName = "DirectX11 Sample";   // �E�B���h�E�^�C�g����
    static const char* className = "DX11Sample";         // �E�B���h�E�N���X��

    //-------------------------------------------------------------
    // �E�B���h�E�N���X�̓o�^
    //-------------------------------------------------------------
    {
        WNDCLASSEX wcex;
        wcex.cbSize        = sizeof(WNDCLASSEX);
        wcex.style         = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc   = WndProc;
        wcex.cbClsExtra    = 0;
        wcex.cbWndExtra    = 0;
        wcex.hInstance     = GetModuleHandle(nullptr);
        wcex.hIcon         = nullptr;
        wcex.hCursor       = LoadCursor(nullptr, IDC_CROSS);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName  = nullptr;
        wcex.lpszClassName = className;
        wcex.hIconSm       = nullptr;
        //---- �o�^
        RegisterClassEx(&wcex);
    }

    //-------------------------------------------------------------
    // �E�B���h�E�\���ʒu����ʒ����ɃZ���^�����O
    //-------------------------------------------------------------
#ifdef _DEBUG
    //u32  style   = WS_OVERLAPPEDWINDOW; // �E�B���h�E���[�h
    u32 style = WS_VISIBLE | WS_POPUP;   // �t���X�N���[�����[�h
#else
    u32  style   = WS_VISIBLE | WS_POPUP; // �t���X�N���[�����[�h
#endif

    u32  styleEx = 0;
    RECT rect    = { 0, 0, WINDOW_W, WINDOW_H };   // �E�B���h�E�̕`��̈�̃T�C�Y

#if LIVE_CODING
    rect.right /= 2;
    rect.bottom /= 2;
    styleEx |= WS_EX_TOPMOST;
#endif

    //---- �E�B���h�E�T�C�Y�̘g���݂ł̃T�C�Y���v�Z���␳
    AdjustWindowRect(&rect, style, false);

    s32 w = rect.right - rect.left;
    s32 h = rect.bottom - rect.top;
    s32 x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
    s32 y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

#if LIVE_CODING
    x = 0;   // ��ʍ���Ɋ񂹂�
    y = 0;
#endif

    //-------------------------------------------------------------
    // �E�B���h�E�̍쐬
    //-------------------------------------------------------------
    HWND hWnd = CreateWindowEx(styleEx,
                               className,                  // [in] �E�B���h�E�N���X��
                               titleName,                  // [in]	�E�B���h�E�^�C�g����
                               style,                      // [in]	�E�B���h�E�n���h��
                               x, y,                       // [in] XY���W
                               w, h,                       // [in]	���E����
                               nullptr,                    // [in] �e�E�B���h�E�n���h��
                               nullptr,                    // [in] ���j���[�n���h��
                               GetModuleHandle(nullptr),   // [in] �A�v���P�[�V�����C���X�^���X�n���h��
                               nullptr);                   // [in] WM_CREATE�̈����i�C�Ӂj

    if(hWnd == nullptr)
        return 0;

    //---- �E�B���h�E�̕\���J�n�i������Ԃ͔�\���̂��߂����ŕ\��ON)
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    //-------------------------------------------------------------
    // �A�v���P�[�V�����̏�����
    //-------------------------------------------------------------
    if(GmRender()->setup(WINDOW_W, WINDOW_H, hWnd) == false)
        return false;

    if(!PRIM_Initialize())
        return false;

    //---- imgui�̏�����
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();   // �J���[�X�^�C����Dark�ɐݒ�
   

    // Win32/D3D�̏�����
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(gpu::D3DDevice(), gpu::D3DContext());

    // �t�H���g��ύX
    ImGuiIO& io = ImGui::GetIO();
    //io.Fonts->AddFontFromFileTTF("C:/windows/fonts/meiryob.ttc", 80.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    io.Fonts->AddFontFromFileTTF("data/font/DotGothic16-Regular.ttf", 80.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    io.FontGlobalScale = 0.25f;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigDockingWithShift = false;
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.ConfigWindowsResizeFromEdges = true;

    passLighting.initialize();
    passFilterHDR.initialize();

#if 1
    // �V�[���R���g���[���[��������
    scene = new SceneController();
    if(scene->initialize() == false) return false;

#endif

    // FPS�J�E���^�[������
    fps = FPSCounter(60);

    //-------------------------------------------------------------
    // ���C�����b�Z�[�W���[�v
    //-------------------------------------------------------------
    MSG msg;
    for(;;) {
        if(PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE)) {
            if(GetMessage(&msg, nullptr, 0, 0) == 0 ) {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            //if(GetKeyState(VK_ESCAPE) & 0x8000) {
            //    PostQuitMessage(WM_QUIT);
            //}
            fileWatcher.update();
            mainLoop();
        }
    }

    //-------------------------------------------------------------
    // �A�v���P�[�V�����̉��
    //-------------------------------------------------------------

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    PRIM_Finalize();

    passFilterHDR.finalize();
    passLighting.finalize();

    GmRender()->cleanup();

    //---- �I���R�[�h
    return (int)msg.wParam;
}
//-------------------------------------------------------------
//! FPS�擾
//-------------------------------------------------------------
double getFPS()
{
    return fpsCount;
}
//-------------------------------------------------------------
//! �f���^�^�C���̎擾
//-------------------------------------------------------------
f32 getDeltaTime()
{
    return deltaTime;
}

//===========================================================================
// END OF FILE
//===========================================================================
