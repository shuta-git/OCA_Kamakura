#include "PauseScene.h"
//---------------------------------------------
// 定数の宣言
//---------------------------------------------
const int PAUSE_X = WINDOW_W / 2;	// セレクト文字の基準座標X
const int GAME_Y = 300;				// ゲームに戻るの座標X
const int LOAD_Y = 420;				// リスタート文字の座標X
const int OPERATION_Y = 540;		// 操作説明文字の座標X
const int SELECT_Y = 660;			// セレクト画面に戻るの座標X	
const int TITLE_Y = 780;			// タイトルに戻るの座標X

//---------------------------------------------
// コンストラクタ
//---------------------------------------------
PauseScene::PauseScene()
{
	scene_type = PAUSE;

	// 画像のファイルパスを設定
	SetGraphHandle("pause/pause.png", (WINDOW_W / 2), (WINDOW_H / 2),0.4f);
	SetGraphHandle("pause/character.png", (WINDOW_W / 2) + 450, (WINDOW_H / 2) + 330,0.3f);
	SetGraphHandle("pause/arrow.png", (WINDOW_W / 2), (WINDOW_H / 2),0.3f);

	// セレクト文字
	font_handle.push_back(CreateFontToHandle(NULL, 50, -1));
	font_handle.push_back(CreateFontToHandle(NULL, 60, -1));
}
//---------------------------------------------
// デストラクタ
//---------------------------------------------
PauseScene::~PauseScene()
{
	
}
//---------------------------------------------
// 更新処理
//---------------------------------------------
void PauseScene::Update()
{
	ChangeSelect();
	Decision();

	// 矢印のポジション座標設定
	DrawSetPosition("pause/arrow.png", arrow_pos_x - 40, arrow_pos_y + 30);
}
//---------------------------------------------
// 描画処理
//---------------------------------------------
void PauseScene::Draw()
{
	// 画像を描画
	DrawGraphHandle();

	DrawStringType(PAUSE_GAME,GAME_Y,"ゲームに戻る");
	DrawStringType(PAUSE_LOAD, LOAD_Y,"リスタート");
	DrawStringType(PAUSE_OPERATION, OPERATION_Y,"操作説明");
	DrawStringType(PAUSE_SELECT, SELECT_Y,"セレクト画面に戻る");
	DrawStringType(PAUSE_TITLE, TITLE_Y,"タイトル");
}
//---------------------------------------------
// セレクトを変える
//---------------------------------------------
void PauseScene::ChangeSelect()
{
	// Wキーを押したとき
	if (IsKeyOn(KEY_INPUT_W) || IsPadOn(PAD_UP)) {
		// セレクトタイプをマイナス
		select_type--;
		// セレクトタイプがセレクトロートより小さくなったら
		if (select_type < PAUSE_GAME) {
			// セレクトタイトルにする
			select_type = PAUSE_TITLE;
		}
	}

	// Sキーを押したとき
	if (IsKeyOn(KEY_INPUT_S) || IsPadOn(PAD_DOWN)) {
		// セレクトタイプをプラス
		select_type++;
		// セレクトタイプがセレクトタイトルより大きかったら
		if (select_type > PAUSE_TITLE) {
			// セレクトロードにする
			select_type = PAUSE_GAME;
		}
	}
}
//---------------------------------------------
// 決定
//---------------------------------------------
void PauseScene::Decision()
{
	if (IsKeyOn(KEY_INPUT_SPACE) || IsPadOn(PAD_B)) {
		if (select_type == PAUSE_GAME) {
			scene_type = GAME_MAIN;
		}
		if (select_type == PAUSE_LOAD) {
			scene_type = LOAD;
		}
		if (select_type == PAUSE_OPERATION) {
			scene_type = OPERATION;
		}
		if (select_type == PAUSE_SELECT) {
			scene_type = SELECT;
		}
		if (select_type == PAUSE_TITLE) {
			scene_type = TITLE;
		}
	}
}
//---------------------------------------------
// 文字の種類で描画
//---------------------------------------------
void PauseScene::DrawStringType(int type_, int y_, string str_)
{
	// 文字
	if (select_type == type_) {
		DrawCenterString(PAUSE_X, y_, str_.c_str(), SKYBLUE, font_handle[FONT_60] );
		arrow_pos_x = GetStringPosX(PAUSE_X,str_, font_handle[FONT_60]);
		arrow_pos_y = y_;
	}
	else {
		DrawCenterString(PAUSE_X, y_, str_.c_str(), BLACK, font_handle[FONT_50]);
	}
}
