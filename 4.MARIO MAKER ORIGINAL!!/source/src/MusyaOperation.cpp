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
	DrawString(80, 100, "���O : musya", GetColor(255, 255, 255), TRUE);
	DrawString(80, 130, "����", GetColor(255, 255, 255), TRUE);
	DrawString(100, 160, "�_�b�V�����Ȃ���y�u���b�N�ɐG����", GetColor(255, 255, 255), TRUE);
	DrawString(100, 190, "�j�󂷂邱�Ƃ��ł���", GetColor(255, 255, 255), TRUE);


	DrawString(420, 430, "��TAB�Ŗ߂�", GetColor(255, 0, 0), TRUE);

}
