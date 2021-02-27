//===========================================================================
//!
//!	@file	main.cpp
//!	@brief	アプリケーションエントリーポイント
//!
//===========================================================================

namespace {

    // シーン切り替え
    SceneController* scene;

    // 照明パス（一時的に配置）
    PassLighting  passLighting;
    PassFilterHDR passFilterHDR;

    // FPS
    FPSCounter fps;
    double     fpsCount;

    f32 deltaTime;


}   // namespace

//---------------------------------------------------------------------------
//! メインループ
//---------------------------------------------------------------------------
void mainLoop()
{

    // あらかじめタイマーの周波数を取得しておく
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    // 時間計測したい始点でカウンタを取得
    LARGE_INTEGER start;
    QueryPerformanceCounter(&start);

    //---- imguiのフレームの初期化
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // FPS 更新
    fpsCount = fps.getFPS();

    {
        scene->update();
        scene->render();

        //=============================================================
        //	照明
        //=============================================================
        passLighting.begin();
        passLighting.execute();
        passLighting.end();

        //=============================================================
        // HDRフィルター
        //=============================================================
        passFilterHDR.begin();
        passFilterHDR.execute();
        passFilterHDR.end();
    }

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    GmRender()->swap(1);

    // 時間計測したい終点でカウンタを取得
    LARGE_INTEGER end;
    QueryPerformanceCounter(&end);
    // カウンタの差分を周波数で割れば経過時間を秒で取得できる
    LONGLONG span = end.QuadPart - start.QuadPart;
    deltaTime     = (f32)span / (f32)frequency.QuadPart;

}

//---------------------------------------------------------------------------
//	ウィンドウプロシージャ
//!	@param	[in]	hWnd	ウィンドウハンドル
//!	@param	[in]	message	ウィンドウメッセージ
//!	@param	[in]	wparam	パラメータ1
//!	@param	[in]	lparam	パラメータ2
//---------------------------------------------------------------------------

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    if(ImGui_ImplWin32_WndProcHandler(hWnd, message, wparam, lparam))
        return true;

    switch(message) {
        case WM_PAINT:   // WM_PAINT		ウィンドウ再描画
        {
            //	PAINTSTRUCT ps;
            //	HDC			hdc = BeginPaint(hWnd, &ps);
            //
            //	EndPaint(hWnd, &ps);
        } break;
        case WM_DESTROY:   //	WM_DESTROY	ウィンドウの破棄

            //-----------------------------------------------------------
            // 高速終了
            //-----------------------------------------------------------
            std::quick_exit(EXIT_SUCCESS);

            PostQuitMessage(0);
            return 0;
        default:   // その他デフォルト
            break;
    }
    return DefWindowProc(hWnd, message, wparam, lparam);
}

//---------------------------------------------------------------------------
//	アプリケーションエントリーポイント
//---------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    FileWatcher fileWatcher;

    fileWatcher.initialize("data");

    static const char* titleName = "DirectX11 Sample";   // ウィンドウタイトル名
    static const char* className = "DX11Sample";         // ウィンドウクラス名

    //-------------------------------------------------------------
    // ウィンドウクラスの登録
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
        //---- 登録
        RegisterClassEx(&wcex);
    }

    //-------------------------------------------------------------
    // ウィンドウ表示位置を画面中央にセンタリング
    //-------------------------------------------------------------
#ifdef _DEBUG
    //u32  style   = WS_OVERLAPPEDWINDOW; // ウィンドウモード
    u32 style = WS_VISIBLE | WS_POPUP;   // フルスクリーンモード
#else
    u32  style   = WS_VISIBLE | WS_POPUP; // フルスクリーンモード
#endif

    u32  styleEx = 0;
    RECT rect    = { 0, 0, WINDOW_W, WINDOW_H };   // ウィンドウの描画領域のサイズ

#if LIVE_CODING
    rect.right /= 2;
    rect.bottom /= 2;
    styleEx |= WS_EX_TOPMOST;
#endif

    //---- ウィンドウサイズの枠込みでのサイズを計算＆補正
    AdjustWindowRect(&rect, style, false);

    s32 w = rect.right - rect.left;
    s32 h = rect.bottom - rect.top;
    s32 x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
    s32 y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

#if LIVE_CODING
    x = 0;   // 画面左上に寄せる
    y = 0;
#endif

    //-------------------------------------------------------------
    // ウィンドウの作成
    //-------------------------------------------------------------
    HWND hWnd = CreateWindowEx(styleEx,
                               className,                  // [in] ウィンドウクラス名
                               titleName,                  // [in]	ウィンドウタイトル名
                               style,                      // [in]	ウィンドウハンドル
                               x, y,                       // [in] XY座標
                               w, h,                       // [in]	幅・高さ
                               nullptr,                    // [in] 親ウィンドウハンドル
                               nullptr,                    // [in] メニューハンドル
                               GetModuleHandle(nullptr),   // [in] アプリケーションインスタンスハンドル
                               nullptr);                   // [in] WM_CREATEの引数（任意）

    if(hWnd == nullptr)
        return 0;

    //---- ウィンドウの表示開始（初期状態は非表示のためここで表示ON)
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    //-------------------------------------------------------------
    // アプリケーションの初期化
    //-------------------------------------------------------------
    if(GmRender()->setup(WINDOW_W, WINDOW_H, hWnd) == false)
        return false;

    if(!PRIM_Initialize())
        return false;

    //---- imguiの初期化
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();   // カラースタイルをDarkに設定
   

    // Win32/D3Dの初期化
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(gpu::D3DDevice(), gpu::D3DContext());

    // フォントを変更
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
    // シーンコントローラーを初期化
    scene = new SceneController();
    if(scene->initialize() == false) return false;

#endif

    // FPSカウンター初期化
    fps = FPSCounter(60);

    //-------------------------------------------------------------
    // メインメッセージループ
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
    // アプリケーションの解放
    //-------------------------------------------------------------

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    PRIM_Finalize();

    passFilterHDR.finalize();
    passLighting.finalize();

    GmRender()->cleanup();

    //---- 終了コード
    return (int)msg.wParam;
}
//-------------------------------------------------------------
//! FPS取得
//-------------------------------------------------------------
double getFPS()
{
    return fpsCount;
}
//-------------------------------------------------------------
//! デルタタイムの取得
//-------------------------------------------------------------
f32 getDeltaTime()
{
    return deltaTime;
}

//===========================================================================
// END OF FILE
//===========================================================================
