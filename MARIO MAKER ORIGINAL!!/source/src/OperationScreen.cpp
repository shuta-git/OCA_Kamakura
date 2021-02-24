#include "OperationScreen.h"
#include "WinMain.h"

//-----------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------
OperationScreen::OperationScreen()
{
}
//-----------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------
OperationScreen::~OperationScreen()
{
}
//-----------------------------------------------------------
// 更新
//-----------------------------------------------------------
void OperationScreen::Update()
{
}
//-----------------------------------------------------------
//	描画
//-----------------------------------------------------------
void OperationScreen::Draw()
{
	DrawBox(60, 60, 580, 420, GetColor(255, 255, 255), FALSE);
	DrawBox(61, 61, 579, 419, GetColor(0, 0, 0), TRUE);

	SetFontSize(25);
	SetFontThickness(1);
	ChangeFont("ゴシック体");
	DrawString(255, 100, "A　：　左に移動", GetColor(255, 255, 255), TRUE);
	DrawString(255, 150, "D　：　右に移動", GetColor(255, 255, 255), TRUE);
	DrawString(205, 200, "SPACE　：　ジャンプ", GetColor(255, 255, 255), TRUE);
	DrawString(205,250,"SHIFT　：　ダッシュ",GetColor(255,255,255),TRUE);
	DrawString(80, 300, "数字キーの1,2,3　：　キャラクター変更", GetColor(255, 255, 255), TRUE);
	DrawString(400, 380, "※TABで戻る", GetColor(255, 0, 0), TRUE);

}

void OperationScreen::ReloadGraph()
{
}
