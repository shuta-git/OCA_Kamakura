
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int GradTexHandle;
	int PixelShaderHandle;			// ピクセルシェーダー
	int VertexShaderHandle;			// 頂点シェーダー

	SetOutApplicationLogValidFlag(FALSE);			// ログ出力 否
	ChangeWindowMode(TRUE);							// ウィンドウモード 可
	SetGraphMode(WINDOW_W, WINDOW_H, 32);			// 画面サイズ
	SetMainWindowText("3D");	// ウィンドウタイトル
	SetZBufferBitDepth(24);

	// Effekseerを使用するには必ず設定する。
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	if (DxLib_Init() == -1)		return -1;

	// Effekseerを初期化する。
	// 引数には画面に表示する最大パーティクル数を設定する。
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
		return -1;
	}

	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	// Effekseerを使用する場合は必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	// DXライブラリのデバイスロストした時のコールバックを設定する。
	// ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// Effekseerに2D描画の設定をする。
	Effekseer_Set2DSetting(WINDOW_W, WINDOW_H);

	SetMouseDispFlag(MOUSE_FLG);
	SetMousePoint(WINDOW_W / 2, WINDOW_H / 2);

	SetDrawScreen(DX_SCREEN_BACK);
	SetBackgroundColor(100, 100, 100);

	// カメラの描画距離
	SetCameraNearFar(0.1f, 2000.0f);

	// ---------------
	// 初期化処理
	// ---------------
	KeyInputInit();
	PadInputInit();
	MouseInputInit();
	FpsInit();

	// Zバッファを有効にする。
	// Effekseerを使用する場合、2DゲームでもZバッファを使用する。
	SetUseZBuffer3D(TRUE);

	// Zバッファへの書き込みを有効にする。
	// Effekseerを使用する場合、2DゲームでもZバッファを使用する。
	SetWriteZBuffer3D(TRUE);

	// マウスカーソルを非表示(true で表示)
	SetMouseDispFlag(false);

	// シーンマネジャーを呼ぶ
	SceneManager* scene = new SceneManager();



	// 頂点シェーダーを読み込む
	VertexShaderHandle = LoadVertexShader("shader/SkinMesh4_DirLight_ToonVS.vso");
	// ピクセルシェーダーを読み込む
	PixelShaderHandle = LoadPixelShader("shader/SkinMesh4_DirLight_ToonPS.pso");
	// グラデーションテクスチャを読み込む
	GradTexHandle = LoadGraph("shader/data/GradTex.bmp");

	// モデルの描画にオリジナルシェーダーを使用する設定をＯＮにする
	//MV1SetUseOrigShader(TRUE);
	// 使用するテクスチャ１にグラデーションテクスチャをセットする
	SetUseTextureToShader(1, GradTexHandle);
	// 使用する頂点シェーダーをセット
	SetUseVertexShader(VertexShaderHandle);
	// 使用するピクセルシェーダーをセット
	SetUsePixelShader(PixelShaderHandle);

	// ゲームメインループ
	while (!ProcessMessage() && CheckHitKey(KEY_INPUT_ESCAPE) == 0 && KeyInputUpdate() == 0)
	{
		clsDx();
		ClearDrawScreen();

		// ---------------
		// 更新処理
		// ---------------
		FpsUpdate();
		if (scene != nullptr) {
			scene->Update();
		}
		// DXライブラリのカメラとEffekseerのカメラを同期する。
		Effekseer_Sync3DSetting();

		KeyInputUpdate();
		MouseInputUpdate();
		PadInputUpdate();
	


		FpsDraw();
		if (scene != nullptr) {
			scene->Draw();
		}
		ScreenFlip();

		ScreenFlip();

		// Fps待機
		FpsWait();
	}

	// ---------------
	// 終了処理
	// ---------------
	KeyInputExit();
	PadInputExit();
	MouseInputExit();
	FpsExit();

	// 使用するテクスチャからグラデーションテクスチャを外す
	SetUseTextureToShader(1, -1);
	// グラデーションテクスチャを削除
	DeleteGraph(GradTexHandle);
	// 読み込んだ頂点シェーダーの削除
	DeleteShader(VertexShaderHandle);
	// 読み込んだピクセルシェーダーの削除
	DeleteShader(PixelShaderHandle);

	delete scene;

	// Effekseerを終了する。
	Effkseer_End();

	DxLib_End();

	return 0;
}
