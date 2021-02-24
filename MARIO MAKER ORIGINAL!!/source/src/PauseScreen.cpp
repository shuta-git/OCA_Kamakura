#include "PauseScreen.h"
#include "WinMain.h"

PauseScreen::PauseScreen()
{
	if (cur == nullptr) {
		cur = new Cursor();
	}
}

PauseScreen::~PauseScreen()
{
	if (cur != nullptr) {
		delete cur;
		cur = nullptr;
	}
	Exit();
}

void PauseScreen::Update()
{
}

void PauseScreen::Draw()
{
	SetFontSize(25);
	SetFontThickness(1);
	ChangeFont("ゴシック体");

	if (cur != nullptr) {
		cur->Move();
		if (cur->GetX() >= 290 && cur->GetY() >= 100 &&
			cur->GetX() <= 340 && cur->GetY() <= 125) {
			DrawString(290, 100, "戻る", GetColor(255, 255, 0), TRUE);
		}
		else {
			DrawString(290, 100, "戻る", GetColor(255, 255, 255), TRUE);
		}
		if (cur->GetX() >= 230 && cur->GetY() >= 150 &&
			cur->GetX() <= 413 && cur->GetY() <= 175) {
			DrawString(230, 150, "マップビルダー", GetColor(255, 255, 0), TRUE);
		}
		else {
			DrawString(230, 150, "マップビルダー", GetColor(255, 255, 255), TRUE);
		}
		if (cur->GetX() >= 200 && cur->GetY() >= 200 &&
			cur->GetX() <= 441 && cur->GetY() <= 225) {
			DrawString(208, 200, "スタート画面に戻る", GetColor(255, 255, 0), TRUE);
		}
		else {
			DrawString(208, 200, "スタート画面に戻る", GetColor(255, 255, 255), TRUE);
		}
		if (cur->GetX() >= 270 && cur->GetY() >= 250 &&
			cur->GetX() <= 370 && cur->GetY() <= 275) {
			DrawString(270, 250, "操作説明", GetColor(255, 255, 0), TRUE);
		}
		else {
			DrawString(270, 250, "操作説明", GetColor(255, 255, 255), TRUE);
		}
		if (cur->GetX() >= 255 && cur->GetY() >= 300 &&
			cur->GetX() <= 388 && cur->GetY() <= 325) {
			DrawString(255, 300, "リスポーン", GetColor(255, 255, 0), TRUE);
		}
		else {
			DrawString(255, 300, "リスポーン", GetColor(255, 255, 255), TRUE);
		}
	}
}

void PauseScreen::ReloadGraph()
{
}


