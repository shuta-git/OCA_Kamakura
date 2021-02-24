#include "MusyaOperation.h"
#include "WinMain.h"


MusyaOperation::MusyaOperation()
{
}


MusyaOperation::~MusyaOperation()
{
}

void MusyaOperation::Draw()
{
	DrawBox(60, 60, 580, 420, GetColor(255, 255, 255), FALSE);
	DrawBox(61, 61, 579, 419, GetColor(0, 0, 0), TRUE);
	SetFontSize(25);
	DrawString(80, 100, "名前 : musya", GetColor(255, 255, 255), TRUE);
	DrawString(80, 130, "特性", GetColor(255, 255, 255), TRUE);
	DrawString(100, 160, "ダッシュしながら土ブロックに触れると", GetColor(255, 255, 255), TRUE);
	DrawString(100, 190, "破壊することができる", GetColor(255, 255, 255), TRUE);


	DrawString(420, 430, "※TABで戻る", GetColor(255, 0, 0), TRUE);

}
