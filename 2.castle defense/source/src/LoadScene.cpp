//---------------------------------------------
// 定数の宣言
//---------------------------------------------
const int GAUGE_W = (WINDOW_W - 40);		// ゲージの横幅
const int GAUGE_H = 24;						// ゲージの縦幅
const int GAUGE_X = 20;						// ゲージのX座標
const int GAUGE_Y = WINDOW_H - 60;			// ゲージのY座標
const int MAX_LOAD_TIME = 10;				// 最大ロードタイム


//---------------------------------------------
// コンストラクタ
//---------------------------------------------
LoadScene::LoadScene()
{
	// 初期化
	handle_count = 0;
	load_count = 0;
	load_time = 0;
	load_str_num = 0;
	font_handle.push_back(CreateFontToHandle(NULL, 20, -1));

	// このシーンタイプを指定
	scene_type = LOAD;
	// 最初に読み込む種類を指定
	load_type = MODEL;
	// ファイルを読み込み
	ReadModelFilePath();
	ReadGraphFilePath();
	ReadEffectFilePath();
	// 文字列を入れる
	load_str.push_back("Now Loading .");
	load_str.push_back("Now Loading ..");
	load_str.push_back("Now Loading ...");
	//  画像のファイルパスを設定
	SetGraphHandle("road/road1_1.png", (WINDOW_W / 2), (WINDOW_H / 2));
}
//---------------------------------------------
// デストラクタ
//---------------------------------------------
LoadScene::~LoadScene()
{

}
//---------------------------------------------
// 更新処理
//---------------------------------------------
void LoadScene::Update()
{

	LoadUpdate(0);

	switch (load_type) {
		// モデル
	case MODEL:
		// 読み込むモデルの数とロードした数が同じなら
		if(model_path.size() == load_count)
		{
			// カウント初期化
			load_count = 0;
			// ロードの種類を画像に変更
			load_type = GRAPH;
		}
		else {
			// モデルロード関数
			LoadModelHandle(model_path[load_count].c_str());
		}
		break;
		// 画像
	case GRAPH:
		// 読み込む画像の数とロードした数が同じなら
		if (graph_path.size() == load_count)
		{
			// カウント初期化
			load_count = 0;
			// ロードの種類をエフェクトに変更
			load_type = EFFECT;
		}
		else {
			// 画像ロード関数
			LoadGraphHandle(graph_path[load_count].c_str());
		}
		break;
		// エフェクト
	case EFFECT:
		// 読み込むエフェクトの数とロードした数が同じなら
		if (effect_path.size() == load_count)
		{
			// カウント初期化
			load_count = 0;
			// ロードの種類をフィニッシュに変更
			load_type = FINISH;
		}
		else {
			// エフェクトロード関数
			LoadEffectHandle(effect_path[load_count].c_str());
		}
		break;
	}

#if(NDEBUG)
	// ロードが終わったら
	if (load_type == FINISH && CheckHitKey(KEY_INPUT_SPACE)) {
		// シーンをゲームメインに切り替え
		scene_type = GAME_MAIN;
	}
#else
	// ロードが終わったら
	if (load_type == FINISH) {
		// シーンをゲームメインに切り替え
		scene_type = GAME_MAIN;
	}
#endif
}
//---------------------------------------------
// 描画処理
//---------------------------------------------
void LoadScene::Draw()
{
	// 画像の描画
	DrawGraphHandle();
	// 読み込み画面描画関数
	DrawLoadingScreen();

	//DrawStringToHandle(WINDOW_W,WINDOW_H );

#if (NDEBUG)
	printfDx("ロード \n");
	for (int i = 0; i < model_path.size(); i ++) {
		printfDx(model_path[i].c_str());
		printfDx("\n");
	}
	for (int i = 0; i < graph_path.size(); i++) {
		printfDx(graph_path[i].c_str());
		printfDx("\n");
	}
	for (int i = 0; i < effect_path.size(); i++) {
		printfDx(effect_path[i].c_str());
		printfDx("\n");
	}
	printfDx("%d \n",handle_count);
	printfDx("%d \n", gauge_count);
#endif
}
//---------------------------------------------
// 読み込み画面描画関数
//---------------------------------------------
void LoadScene::DrawLoadingScreen()
{
	// ロードの文字とゲージの座標用
	int loading_x, loading_y;
	
	loading_x = GAUGE_X;
	loading_y = GAUGE_Y - 20;
	// ロードの文字
	DrawStringToHandle(loading_x, loading_y, load_str[load_str_num].c_str(), GetColor(255, 255, 255), font_handle[0]);

	loading_x = GAUGE_X;
	loading_y = GAUGE_Y;
	// ゲージの全体を灰色で描画
	DrawBox(loading_x, loading_y, loading_x + GAUGE_W, loading_y + GAUGE_H, GetColor(125, 125, 125), TRUE);

	// 現時点で読み込み終わっている分を白色で描画
	DrawBox(loading_x, loading_y,loading_x +( gauge_count * GAUGE_W / handle_count), loading_y + GAUGE_H, SKYBLUE, TRUE);
}
//---------------------------------------------
// モデルパスファイルを読み込み
//---------------------------------------------
void LoadScene::ReadModelFilePath()
{
	file.open("data/txt/LoadModelPath.txt", ios::in);

	if (!file.is_open()) {
		// ファイルを開くことに失敗しました
		return;
	}

	string str;
	while (getline(file, str))
	{
		model_path.push_back(str);
		handle_count++;
	}

	// ファイルを閉じる
	file.close();
}
//---------------------------------------------
// 画像パスファイルを読み込み
//---------------------------------------------
void LoadScene::ReadGraphFilePath()
{
	file.open("data/txt/LoadGraphPath.txt", ios::in);

	if (!file.is_open()) {
		// ファイルを開くことに失敗しました
		return;
	}

	string str;
	while (getline(file, str))
	{
		graph_path.push_back(str);
		handle_count++;
	}

	// ファイルを閉じる
	file.close();
}
//---------------------------------------------
//  エフェクトパスファイルを読み込み
//---------------------------------------------
void LoadScene::ReadEffectFilePath()
{
	file.open("data/txt/LoadEffectPath.txt", ios::in);

	if (!file.is_open()) {
		// ファイルを開くことに失敗しました
		return;
	}

	string str;
	while (getline(file, str))
	{
		effect_path.push_back(str);
		handle_count++;
	}

	// ファイルを閉じる
	file.close();
}
//---------------------------------------------
// ロードタイムの更新処理
// load_str_num がサイズ分超えたときに何番に調整するか
//---------------------------------------------
void LoadScene::LoadUpdate(int num)
{
	load_time++;
	if (load_time % MAX_LOAD_TIME == 0) {
		load_time = 0;
		load_str_num++;
	}
	if (load_str_num == load_str.size()) {
		load_str_num = num;
	}
}
//---------------------------------------------
// ロードモデルハンドル
//---------------------------------------------
void LoadScene::LoadModelHandle(string str)
{
	// パスを設定
	string data = "data/";
	string path = data + str;
	resource::LoadModel(path.c_str());
	load_count++;
	gauge_count++;
}
//---------------------------------------------
// ロードグラフィックハンドル
//---------------------------------------------
void LoadScene::LoadGraphHandle(string str)
{
	// パスを設定
	string data = "data/";
	string path = data + str;
	resource::LoadGraph(path.c_str());
	load_count++;
	gauge_count++;
}
//---------------------------------------------
// ロードエフェクトハンドル
//---------------------------------------------
void LoadScene::LoadEffectHandle(string str)
{
	// パスを設定
	string data = "data/effect/";
	string path = data + str;
	resource::LoadEffekseerEffect(path.c_str());
	load_count++;
	gauge_count++;
}
