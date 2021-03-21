//---------------------------------------------
// 定数の宣言
//---------------------------------------------
const int STRING_X = WINDOW_W / 2 - 200;	// 文字の座標X
const int STRING_Y = WINDOW_H - 100;		// 文字の座標Y

//---------------------------------------------
// コンストラクタ
//---------------------------------------------
TitleScene::TitleScene()
{
	// このシーンタイプを指定
	scene_type = TITLE;
	// 画像のファイルパスを設定
	SetGraphHandle("title/title2_1.png",(WINDOW_W /2),(WINDOW_H / 2));
	// 座標を指定
	pos = { WINDOW_W / 2,WINDOW_H / 2,0.0f };
	// サイズを宣言
	scale = { 130.0f,130.0f,130.0f };
	// エフェクトの生成
	effect.push_back(new Effect("Atmosphere.efk", pos, false));
	effect.push_back(new Effect("Atmosphere.efk", pos, false));
	effect.push_back(new Effect("Atmosphere.efk", pos, false));
	effect.push_back(new Effect("Atmosphere.efk", pos, false));
	effect.push_back(new Effect("Atmosphere.efk", pos, false));

	flush = 0;

	// フォントを設定
	font_handle.push_back( CreateFontToHandle(NULL, 70, -1));
}
//---------------------------------------------
// デストラクタ
//---------------------------------------------
TitleScene::~TitleScene()
{
	// フォントハンドルを消去
	DeleteFontHandle();
}
//---------------------------------------------
// 更新処理
//---------------------------------------------
void TitleScene::Update()
{
	// 点滅用のカウントを回す
	flush++;

	if (IsKeyOn(KEY_INPUT_SPACE) || IsPadOn(PAD_B)) {
		scene_type = SELECT;
	}

	// エフェクトの処理
	for (int i = 0; i < effect.size(); i++) {
		// エフェクトのサイズを指定
		effect[i]->SetScale(scale);
		// エフェクトの更新処理
		effect[i]->Update();
		// デリート処理
		DeleteEffect(i);
	}
}
//---------------------------------------------
// 描画処理
//---------------------------------------------
void TitleScene::Draw()
{
	// 画像を描画
	DrawGraphHandle();

	// エフェクトの描画
	for (int i = 0; i < effect.size(); i++) {
		effect[i]->Draw();
	}
	// カウントするタイミング
	if (flush % 40 <= 20) {
		DrawStringToHandle(STRING_X, STRING_Y, "Push B Button", WHITE, font_handle[0]);
	}

	printfDx("タイトル \n");
}
