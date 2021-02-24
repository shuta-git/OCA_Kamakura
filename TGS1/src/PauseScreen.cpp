#include "PauseScreen.h"


//----------------------------------------------------------------------
// コンストラクタ
//----------------------------------------------------------------------
PauseScreen::PauseScreen()
{
	start = false;
	operation = false;
	game = false;
	spawn = false;
	pause_num = PAUSE_GAME;
}
//----------------------------------------------------------------------
// デストラクタ
//----------------------------------------------------------------------
PauseScreen::~PauseScreen()
{
}
//----------------------------------------------------------------------
// 初期化処理
//----------------------------------------------------------------------
void PauseScreen::Init()
{
}
//----------------------------------------------------------------------
// 更新処理
//----------------------------------------------------------------------
void PauseScreen::Update()
{
#if ! defined(NDEBUG)
	KeySelect();
#else
	SelectChange();
	ConSelect();
#endif
}
//----------------------------------------------------------------------
// 描画処理
//----------------------------------------------------------------------
void PauseScreen::Draw(Font font, Font font1)
{
#if ! defined(NDEBUG)
	DrawMouse(font, font1);
#else
	DrawCon(font, font1);
#endif

}
//----------------------------------------------------------------------
// 終了処理
//----------------------------------------------------------------------
void PauseScreen::Exit()
{
}
//----------------------------------------------------------------------
// "ゲームに戻る" の当たり判定用
//----------------------------------------------------------------------
RectF PauseScreen::GetGame() const
{
	return RectF(WINDOW_W / 2 - 240, WINDOW_H / 2 - WINDOW_H / 3 - 70, 440, 75);
}
//----------------------------------------------------------------------
// "操作説明" の当たり判定用
//----------------------------------------------------------------------
RectF PauseScreen::GetOperation() const
{
	return RectF(WINDOW_W / 2 - 170, WINDOW_H / 2 - 230, 300, 75);
}
//----------------------------------------------------------------------
// "スタートからやり直す" の当たり判定用
//----------------------------------------------------------------------
RectF PauseScreen::GetSpawn() const
{
	return RectF(WINDOW_W / 2 - 380, WINDOW_H / 2 - 30, 740, 75);
}
//----------------------------------------------------------------------
// "スタート画面に戻る" の当たり判定
//----------------------------------------------------------------------
RectF PauseScreen::GetStart() const
{
	return RectF(WINDOW_W / 2 - 350, WINDOW_H / 2 + 170, 665, 75);
}
//----------------------------------------------------------------------
// キーボードで選択
//----------------------------------------------------------------------
void PauseScreen::KeySelect()
{
	// "ゲームに戻る" をクリックしたら
	if (Mouse::Pos().intersects(GetGame())) {
		if (Input::MouseL.clicked) {
			game = true;
		}
	}
	//  "操作説明" をクリックしたら
	if (Mouse::Pos().intersects(GetOperation())) {
		if (Input::MouseL.clicked) {
			operation = true;
		}
	}
	// "スタートからやり直す" をクリックしたら
	if (Mouse::Pos().intersects(GetSpawn())) {
		if (Input::MouseL.clicked) {
			spawn = true;
		}
	}
	// "スタート画面に戻る" をクリックしたら
	if (Mouse::Pos().intersects(GetStart())) {
		if (Input::MouseL.clicked) {
			start = true;
		}
	}
}
//----------------------------------------------------------------------
// コントロールで選択
//----------------------------------------------------------------------
void PauseScreen::ConSelect()
{
	if (pause_num == PAUSE_GAME) {
		if (controller.buttonA.clicked) {
			game = true;
		}
	}
	if (pause_num == PAUSE_OPERATION) {
		if (controller.buttonA.clicked) {
			operation = true;
		}
	}
	if (pause_num == PAUSE_SPAWN) {
		if (controller.buttonA.clicked) {
			spawn = true;
		}
	}
	if (pause_num == PAUSE_START) {
		if (controller.buttonA.clicked) {
			start = true;
		}
	}

}
//----------------------------------------------------------------------
// セレクトを変える
//----------------------------------------------------------------------
void PauseScreen::SelectChange()
{
	if (controller.buttonUp.clicked ) {
		pause_num--;
		if(pause_num < PAUSE_GAME){
			pause_num = PAUSE_START;
		}
	}
	if (controller.buttonDown.clicked) {
		pause_num++;
		if (pause_num > PAUSE_START) {
			pause_num = PAUSE_GAME;
		}
	}
}
//----------------------------------------------------------------------
// キーボード描画
//----------------------------------------------------------------------
void PauseScreen::DrawMouse(Font font, Font font1)
{
	if (Mouse::Pos().intersects(GetGame())) {
		font1(L"ゲームに戻る").draw(WINDOW_W / 2 - 260, WINDOW_H / 2 - WINDOW_H / 3 - 80, Palette::Yellow);
	}
	else {
		font(L"ゲームに戻る").draw(WINDOW_W / 2 - 240, WINDOW_H / 2 - WINDOW_H / 3 - 80);
	}
	if (Mouse::Pos().intersects(GetOperation())) {
		font1(L"操作説明").draw(WINDOW_W / 2 - 190, WINDOW_H / 2 - 240, Palette::Yellow);
	}
	else {
		font(L"操作説明").draw(WINDOW_W / 2 - 170, WINDOW_H / 2 - 240);
	}
	if (Mouse::Pos().intersects(GetSpawn())) {
		font1(L"スタートからやり直す").draw(WINDOW_W / 2 - 400, WINDOW_H / 2 - 40, Palette::Yellow);
	}
	else {
		font(L"スタートからやり直す").draw(WINDOW_W / 2 - 380, WINDOW_H / 2 - 40);
	}
	if (Mouse::Pos().intersects(GetStart())) {
		font1(L"スタート画面に戻る").draw(WINDOW_W / 2 - 370, WINDOW_H / 2 + 160, Palette::Yellow);
	}
	else {
		font(L"スタート画面に戻る").draw(WINDOW_W / 2 - 350, WINDOW_H / 2 + 160);
	}
}
//----------------------------------------------------------------------
// コントローラー描画
//----------------------------------------------------------------------
void PauseScreen::DrawCon(Font font, Font font1)
{
	if (pause_num == PAUSE_GAME) {
		font1(L"ゲームに戻る").draw(WINDOW_W / 2 - 260, WINDOW_H / 2 - WINDOW_H / 3 - 80, Palette::Yellow);
	}
	else {
		font(L"ゲームに戻る").draw(WINDOW_W / 2 - 240, WINDOW_H / 2 - WINDOW_H / 3 - 80);
	}
	if (pause_num == PAUSE_OPERATION) {
		font1(L"操作説明").draw(WINDOW_W / 2 - 190, WINDOW_H / 2 - 240, Palette::Yellow);
	}
	else {
		font(L"操作説明").draw(WINDOW_W / 2 - 170, WINDOW_H / 2 - 240);
	}
	if (pause_num == PAUSE_SPAWN) {
		font1(L"スタートからやり直す").draw(WINDOW_W / 2 - 400, WINDOW_H / 2 - 40, Palette::Yellow);
	}
	else {
		font(L"スタートからやり直す").draw(WINDOW_W / 2 - 380, WINDOW_H / 2 - 40);
	}
	if (pause_num == PAUSE_START) {
		font1(L"スタート画面に戻る").draw(WINDOW_W / 2 - 370, WINDOW_H / 2 + 160, Palette::Yellow);
	}
	else {
		font(L"スタート画面に戻る").draw(WINDOW_W / 2 - 350, WINDOW_H / 2 + 160);
	}
}
