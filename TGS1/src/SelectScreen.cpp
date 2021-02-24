#include "SelectScreen.h"

//----------------------------------------------------------------------
// コンストラクタ
//----------------------------------------------------------------------
SelectScreen::SelectScreen()
{
	map_num = MAP_COUNT;
	operation = false;
	select_num = SELECT_OPERATION;
}
//----------------------------------------------------------------------
// デストラクタ
//----------------------------------------------------------------------
SelectScreen::~SelectScreen()
{
}
//----------------------------------------------------------------------
// 初期化処理
//----------------------------------------------------------------------
void SelectScreen::Init()
{
}
//----------------------------------------------------------------------
// 更新処理
//----------------------------------------------------------------------
void SelectScreen::Update()
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
void SelectScreen::Draw(Font font, Font font1)
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
void SelectScreen::Exit()
{
}
//----------------------------------------------------------------------
// "EASY" の当たり判定用
//----------------------------------------------------------------------
RectF SelectScreen::GetEasy() const
{
	return RectF(WINDOW_W / 2 + 200, WINDOW_H / 2 - WINDOW_H / 3 - 50, 250, 80);
}
//----------------------------------------------------------------------
// "NORMAL" の当たり判定用
//----------------------------------------------------------------------
RectF SelectScreen::GetNormal() const
{
	return RectF(WINDOW_W / 2 + 200, WINDOW_H / 2 - 70, 420, 80);
}
//----------------------------------------------------------------------
// "HARD" の当たり判定用
//----------------------------------------------------------------------
RectF SelectScreen::GetHard() const
{
	return RectF(WINDOW_W / 2 + 200, WINDOW_H / 2 + WINDOW_H / 3 - 70, 260, 80);
}
//----------------------------------------------------------------------
// "操作説明" の当たり判定用
//----------------------------------------------------------------------
RectF SelectScreen::GetOperation() const
{
	return RectF(200, WINDOW_H / 2 - 80, 430, 100);
}
//----------------------------------------------------------------------
// キーボードで選択
//----------------------------------------------------------------------
void SelectScreen::KeySelect()
{
	// EASY をクリックしたら
	if (Mouse::Pos().intersects(GetEasy())) {
		if (Input::MouseL.clicked) {
			map_num = MAP_ONE;
		}
	}
	// NORMAL をクリックしたら
	if (Mouse::Pos().intersects(GetNormal())) {
		if (Input::MouseL.clicked) {
			map_num = MAP_TWO;
		}
	}
	// HARD をクリックしたら
	if (Mouse::Pos().intersects(GetHard())) {
		if (Input::MouseL.clicked) {
			map_num = MAP_THREE;
		}
	}
	// 操作説明をクリックしたら
	if (Mouse::Pos().intersects(GetOperation())) {
		if (Input::MouseL.clicked) {
			operation = true;
		}
	}
}
//----------------------------------------------------------------------
// コントロールで選択
//----------------------------------------------------------------------
void SelectScreen::ConSelect()
{
	if (select_num == SELECT_EASY) {
		if (controller.buttonA.clicked) {
			map_num = MAP_ONE;
		}
	}
	if (select_num == SELECT_NORMAL) {
		if (controller.buttonA.clicked) {
			map_num = MAP_TWO;
		}
	}
	if (select_num == SELECT_HARD) {
		if (controller.buttonA.clicked) {
			map_num = MAP_THREE;
		}
	}
	if (select_num == SELECT_OPERATION) {
		if (controller.buttonA.clicked) {
			operation = true;
		}
	}
}

//----------------------------------------------------------------------
// セレクトを変える
//----------------------------------------------------------------------
void SelectScreen::SelectChange()
{
	if (controller.buttonRight.clicked && select_num == SELECT_OPERATION) {
		select_num = SELECT_NORMAL;
	}
	if (controller.buttonLeft.clicked && select_num != SELECT_OPERATION) {
		select_num = SELECT_OPERATION;
	}
	if (controller.buttonUp.clicked && select_num != SELECT_OPERATION) {
		select_num--;
		if (select_num < SELECT_EASY) {
			select_num = SELECT_OPERATION;
		}
	}
	else if (controller.buttonUp.clicked && select_num == SELECT_OPERATION) {
			select_num = SELECT_EASY;
		
	}
	if (controller.buttonDown.clicked&& select_num != SELECT_OPERATION) {
		select_num++;
		if (select_num > SELECT_HARD) {
			select_num = SELECT_OPERATION;
		}
	}else if (controller.buttonDown.clicked&& select_num == SELECT_OPERATION) {
		select_num = SELECT_HARD;
	}
	

}
//----------------------------------------------------------------------
// キーボード描画
//----------------------------------------------------------------------
void SelectScreen::DrawMouse(Font font, Font font1)
{
	if (Mouse::Pos().intersects(GetEasy())) {
		font1(L"EASY").draw(WINDOW_W / 2 + 180, WINDOW_H / 2 - WINDOW_H / 3 - 80, Palette::Yellow);
	}
	else {
		font(L"EASY").draw(WINDOW_W / 2 + 200, WINDOW_H / 2 - WINDOW_H / 3 - 80);
	}
	if (Mouse::Pos().intersects(GetNormal())) {
		font1(L"NORMAL").draw(WINDOW_W / 2 + 180, WINDOW_H / 2 - 100, Palette::Yellow);
	}
	else {
		font(L"NORMAL").draw(WINDOW_W / 2 + 200, WINDOW_H / 2 - 100);
	}
	if (Mouse::Pos().intersects(GetHard())) {
		font1(L"HARD").draw(WINDOW_W / 2 + 180, WINDOW_H / 2 + WINDOW_H / 3 - 100, Palette::Yellow);
	}
	else {
		font(L"HARD").draw(WINDOW_W / 2 + 200, WINDOW_H / 2 + WINDOW_H / 3 - 100);
	}
	if (Mouse::Pos().intersects(GetOperation())) {
		font1(L"操作説明").draw(180, WINDOW_H / 2 - 100, Palette::Yellow);
	}
	else {
		font(L"操作説明").draw(200, WINDOW_H / 2 - 100);
	}
}
//----------------------------------------------------------------------
// コントローラー描画
//----------------------------------------------------------------------
void SelectScreen::DrawCon(Font font, Font font1)
{
	if (select_num == SELECT_EASY) {
		font1(L"EASY").draw(WINDOW_W / 2 + 180, WINDOW_H / 2 - WINDOW_H / 3 - 80, Palette::Yellow);
	}
	else {
		font(L"EASY").draw(WINDOW_W / 2 + 200, WINDOW_H / 2 - WINDOW_H / 3 - 80);
	}
	if (select_num == SELECT_NORMAL) {
		font1(L"NORMAL").draw(WINDOW_W / 2 + 180, WINDOW_H / 2 - 100, Palette::Yellow);
	}
	else {
		font(L"NORMAL").draw(WINDOW_W / 2 + 200, WINDOW_H / 2 - 100);
	}
	if (select_num == SELECT_HARD) {
		font1(L"HARD").draw(WINDOW_W / 2 + 180, WINDOW_H / 2 + WINDOW_H / 3 - 100, Palette::Yellow);
	}
	else {
		font(L"HARD").draw(WINDOW_W / 2 + 200, WINDOW_H / 2 + WINDOW_H / 3 - 100);
	}
	if (select_num == SELECT_OPERATION) {
		font1(L"操作説明").draw(180, WINDOW_H / 2 - 100, Palette::Yellow);
	}
	else {
		font(L"操作説明").draw(200, WINDOW_H / 2 - 100);
	}
}