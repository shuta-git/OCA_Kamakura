#include "FionaOperationScreen.h"
#include "WinMain.h"



FionaOperationScreen::FionaOperationScreen()
{
}


FionaOperationScreen::~FionaOperationScreen()
{
}

void FionaOperationScreen::Draw()
{
	DrawBox(60, 60, 580, 420, GetColor(255, 255, 255), FALSE);
	DrawBox(61, 61, 579, 419, GetColor(0, 0, 0), TRUE);
	SetFontSize(25);
	DrawString(80, 100, "名前 : fiona", GetColor(255, 255, 255), TRUE);
	DrawString(80, 130, "特性", GetColor(255, 255, 255), TRUE);
	DrawString(100, 160, "三人の中で一番高くジャンプができる", GetColor(255, 255, 255), TRUE);
	DrawString(420, 430, "※TABで戻る", GetColor(255, 0, 0), TRUE);


}
