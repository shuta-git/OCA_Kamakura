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
	DrawString(80, 100, "���O : ninin", GetColor(255, 255, 255), TRUE);
	DrawString(80, 130, "����", GetColor(255, 255, 255), TRUE);
	DrawString(100, 160, "��}�X���̌��Ԃ�ʂ邱�Ƃ��ł���", GetColor(255, 255, 255), TRUE);
	DrawString(100, 190, "�΃u���b�N�̂ݕǃW�����v���ł���", GetColor(255, 255, 255), TRUE);
	DrawString(100, 220, "�΃u���b�N�̂ݕǂɕ߂܂邱�Ƃ��ł���", GetColor(255, 255, 255), TRUE);


	DrawString(420, 430, "��TAB�Ŗ߂�", GetColor(255, 0, 0), TRUE);

}
