#include "WinMain.h"
#include "ClearScreen.h"

ClearScreen::ClearScreen()
{
	image[0] = LoadGraph("data/clear/clear.png");
}

ClearScreen::~ClearScreen()
{
}

void ClearScreen::Update()
{
}

void ClearScreen::Draw()
{
	DrawGraph(0, 0, image[0], TRUE);
	SetFontSize(25);
	DrawString(340, 450, "TABでスタート画面に戻る", GetColor(0, 0, 0), TRUE);
}

void ClearScreen::ReloadGraph()
{
	image[0] = LoadGraph("data/clear/clear.png");
}












