#include "OperationScreen.h"
#include "WinMain.h"

//-----------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------
OperationScreen::OperationScreen()
{
}
//-----------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------
OperationScreen::~OperationScreen()
{
}
//-----------------------------------------------------------
// �X�V
//-----------------------------------------------------------
void OperationScreen::Update()
{
}
//-----------------------------------------------------------
//	�`��
//-----------------------------------------------------------
void OperationScreen::Draw()
{
	DrawBox(60, 60, 580, 420, GetColor(255, 255, 255), FALSE);
	DrawBox(61, 61, 579, 419, GetColor(0, 0, 0), TRUE);

	SetFontSize(25);
	SetFontThickness(1);
	ChangeFont("�S�V�b�N��");
	DrawString(255, 100, "A�@�F�@���Ɉړ�", GetColor(255, 255, 255), TRUE);
	DrawString(255, 150, "D�@�F�@�E�Ɉړ�", GetColor(255, 255, 255), TRUE);
	DrawString(205, 200, "SPACE�@�F�@�W�����v", GetColor(255, 255, 255), TRUE);
	DrawString(205,250,"SHIFT�@�F�@�_�b�V��",GetColor(255,255,255),TRUE);
	DrawString(80, 300, "�����L�[��1,2,3�@�F�@�L�����N�^�[�ύX", GetColor(255, 255, 255), TRUE);
	DrawString(400, 380, "��TAB�Ŗ߂�", GetColor(255, 0, 0), TRUE);

}

void OperationScreen::ReloadGraph()
{
}
