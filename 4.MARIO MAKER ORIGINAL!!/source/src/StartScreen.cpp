#include "WinMain.h"
#include "StartScreen.h"

//-----------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------
StartScreen::StartScreen()
{
	if (cur == nullptr) {
		cur = new Cursor();
	}
}
//-----------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------
StartScreen::~StartScreen()
{
	if (cur != nullptr) {
		delete cur;
		cur = nullptr;
	}
	Exit();
}

void StartScreen::Update()
{
}

//-----------------------------------------------------------
//	描画
//-----------------------------------------------------------
void StartScreen::Draw()
{
	if (cur != nullptr) {
		cur->Move();
		// ゲームスタート
		if (cur->GetX() >= 60 && cur->GetY() >= 150 &&
			cur->GetX() <= 260 && cur->GetY() <= 250) {
			DrawBox(60, 150, 260, 250, GetColor(255, 255, 0), FALSE);
		}
		else {
			DrawBox(60, 150, 260, 250, GetColor(255, 255, 255), FALSE);
		}
		// マップビルダー
		if (cur->GetX() >= 380 && cur->GetY() >= 150 &&
			cur->GetX() <= 580 && cur->GetY() <= 250) {
			DrawBox(380, 150, 580, 250, GetColor(255, 255, 0), FALSE);
		}
		else {
			DrawBox(380, 150, 580, 250, GetColor(255, 255, 255), FALSE);
		}
		// 操作説明
		if (cur->GetX() >= 60 && cur->GetY() >= 290 &&
			cur->GetX() <= 260 && cur->GetY() <= 350) {
			DrawBox(60, 290, 260, 350, GetColor(255, 255, 0), FALSE);
		}
		else {
			DrawBox(60, 290, 260, 350, GetColor(255, 255, 255), FALSE);
		}
		// キャラクター説明
		if (cur->GetX() >= 60 && cur->GetY() >= 380 &&
			cur->GetX() <= 260 && cur->GetY() <= 440) {
			DrawBox(60, 380, 260, 440, GetColor(255, 255, 0), FALSE);
		}
		else {
			DrawBox(60, 380, 260, 440, GetColor(255, 255, 255), FALSE);
		}
		// ランキング
		if (cur->GetX() >= 380 && cur->GetY() >= 290 &&
			cur->GetX() <= 580 && cur->GetY() <= 350) {
			DrawBox(380, 290, 580, 350, GetColor(255, 255, 0), FALSE);
		}
		else {
			DrawBox(380, 290, 580, 350, GetColor(255, 255, 255), FALSE);
		}
		// チュートリアル
		if (cur->GetX() >= 380 && cur->GetY() >= 380 &&
			cur->GetX() <= 580 && cur->GetY() <= 440) {
			DrawBox(380, 380, 580, 440, GetColor(255, 255, 0), FALSE);
		}
		else {
			DrawBox(380, 380, 580, 440, GetColor(255, 255, 255), FALSE);
		}
	}
	// ゲームスタート
	DrawBox(61 ,151,259,249, GetColor(0, 0, 0), TRUE);
	// マップビルダー
	DrawBox(381,151,579,249, GetColor(0, 0, 0), TRUE);
	// 操作説明
	DrawBox(61,291,259,349, GetColor(0, 0, 0), TRUE);
	// キャラクター説明
	DrawBox(61, 381, 259, 439, GetColor(0, 0, 0), TRUE);
	// ランキング
	DrawBox(381, 291, 579, 349, GetColor(0, 0, 0), TRUE);
	// チュートリアル
	DrawBox(381, 381, 579, 439, GetColor(0, 0, 0), TRUE);

	SetFontSize(50);
	SetFontThickness(1);
	ChangeFont("ゴシック体");
	DrawString(50, 75, "MARIO MAKER ORIGINAL!!", GetColor(0, 0, 0), TRUE);
	SetFontSize(25);
	DrawString(67, 187, "ゲームスタート", GetColor(255, 255, 255), TRUE);
	DrawString(390, 187, "マップビルダー", GetColor(255, 255, 255), TRUE);
	SetFontSize(20);
	DrawString(120, 310, "操作説明", GetColor(255, 255, 255), TRUE);
	DrawString(80, 400, "キャラクター説明", GetColor(255, 255, 255), TRUE);
	DrawString(430, 310, "ランキング", GetColor(255, 255, 255), TRUE);
	DrawString(410, 400, "チュートリアル", GetColor(255, 255, 255), TRUE);

}

void StartScreen::ReloadGraph()
{
}











