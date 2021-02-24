#include "SelectScene.h"
//---------------------------------------------
// 定数の宣言
//---------------------------------------------
const int SELECT_X = WINDOW_W / 2 + 280;	// セレクト文字の基準座標X
const int GAME_Y = 300;						// ゲームスタート文字の座標Y
const int OPERATION_Y =490;					// 操作説明文字の座標Y
const int RANKING_Y = 680;					// ランキング文字の座標Y
const int TITLE_Y = 870;					// タイトル文字の座標Y

//---------------------------------------------
// コンストラクタ
//---------------------------------------------
SelectScene::SelectScene()
{
	// このシーンタイプを指定
	scene_type = SELECT;
	// セレクトタイプを設定
	select_type = SELECT_LOAD;
	// 画像のファイルパスを設定
	SetGraphHandle("select/select_background1.png", (WINDOW_W / 2), (WINDOW_H / 2));
	SetGraphHandle("select/select_character1.png", (WINDOW_W / 4), (WINDOW_H / 2) , 0.4f);
	SetGraphHandle("select/select_UI1.png", WINDOW_W - 700, (WINDOW_H / 2),0.5f);

	ranking = nullptr;

	// セレクト文字
	font_handle.push_back(CreateFontToHandle(NULL, 70, -1));
	font_handle.push_back(CreateFontToHandle(NULL, 90, -1));
}
//---------------------------------------------
// デストラクタ
//---------------------------------------------
SelectScene::~SelectScene()
{
}
//---------------------------------------------
// 更新処理
//---------------------------------------------
void SelectScene::Update()
{
	if (ranking != nullptr) {
		ranking->Update();
		//　シーンのタイプがセレクトになったら
		if (ranking->GetSceneType() == SELECT) {
			// フォントハンドルを消去
			ranking->DeleteFontHandle();
			// ランキンを消去
			delete ranking;
			ranking = nullptr;
		}
	}else{
		// セレクトを変える
		ChangeSelect();
		// 決定
		Decision();
	}

}
//---------------------------------------------
// 描画処理
//---------------------------------------------
void SelectScene::Draw()
{
	// 画像を描画
	DrawGraphHandle();

	if (ranking != nullptr) {
		ranking->Draw();
	}
	else {
		DrawStringType(SELECT_LOAD,GAME_Y,"ゲームスタート");
		DrawStringType(SELECT_OPERATION, OPERATION_Y,"操作説明");
		DrawStringType(SELECT_RANKING, RANKING_Y,"ランキング");
		DrawStringType(SELECT_TITLE, TITLE_Y,"タイトル");
	}

}
//---------------------------------------------
// セレクトを変える
//---------------------------------------------
void SelectScene::ChangeSelect()
{
	// Wキーを押したとき
	if (IsKeyOn(KEY_INPUT_W) || IsPadOn(PAD_UP)) {
		// セレクトタイプをマイナス
		select_type--;
		// セレクトタイプがセレクトロートより小さくなったら
		if (select_type < SELECT_LOAD) {
			// セレクトタイトルにする
			select_type = SELECT_TITLE;
		}
	}
	// Sキーを押したとき
	if (IsKeyOn(KEY_INPUT_S) || IsPadOn(PAD_DOWN)) {
		// セレクトタイプをプラス
		select_type++;
		// セレクトタイプがセレクトタイトルより大きかったら
		if (select_type > SELECT_TITLE) {
			// セレクトロードにする
			select_type = SELECT_LOAD;
		}
	}
}
//---------------------------------------------
// 決定
//---------------------------------------------
void SelectScene::Decision()
{
	if (IsKeyOn(KEY_INPUT_SPACE) || IsPadOn(PAD_B)) {
		if (select_type == SELECT_LOAD) {
			scene_type = LOAD;
		}
		if (select_type == SELECT_OPERATION) {
			scene_type = OPERATION;
		}
		if (select_type == SELECT_RANKING) {
			if (ranking == nullptr) {
				ranking = new RankingScene();
			}
		}
		if (select_type == SELECT_TITLE) {
			scene_type = TITLE;
		}
	}
}
//---------------------------------------------
// 文字の種類で描画
//---------------------------------------------
void SelectScene::DrawStringType(int type_, int y_, string str_)
{
	// 文字
	if (select_type == type_) {
		DrawCenterString(SELECT_X, y_, str_.c_str(), SKYBLUE, font_handle[FONT_90]);
	}
	else {
		DrawCenterString(SELECT_X, y_, str_.c_str(), BLACK, font_handle[FONT_70]);
	}
}
