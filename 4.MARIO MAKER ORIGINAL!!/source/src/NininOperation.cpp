#include "NininOperation.h"
#include "WinMain.h"


NininOperation::NininOperation()
{
}


NininOperation::~NininOperation()
{
}

void NininOperation::Draw()
{
	DrawBox(60, 60, 580, 420, GetColor(255, 255, 255), FALSE);
	DrawBox(61, 61, 579, 419, GetColor(0, 0, 0), TRUE);
	SetFontSize(25);
	DrawString(80, 100, "名前 : ninin", GetColor(255, 255, 255), TRUE);
	DrawString(80, 130, "特性", GetColor(255, 255, 255), TRUE);
	DrawString(100, 160, "一マス分の隙間を通ることができる", GetColor(255, 255, 255), TRUE);
	DrawString(100, 190, "石ブロックのみ壁ジャンプができる", GetColor(255, 255, 255), TRUE);
	DrawString(100, 220, "石ブロックのみ壁に捕まることができる", GetColor(255, 255, 255), TRUE);


	DrawString(420, 430, "※TABで戻る", GetColor(255, 0, 0), TRUE);

}
