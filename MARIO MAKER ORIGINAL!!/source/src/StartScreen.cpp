#include "WinMain.h"
#include "StartScreen.h"

//-----------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------
StartScreen::StartScreen()
{
	if (cur == nullptr) {
		cur = new Cursor();
	}
}
//-----------------------------------------------------------
// �f�X�g���N�^
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
//	�`��
//-----------------------------------------------------------
void StartScreen::Draw()
{
	if (cur != nullptr) {
		cur->Move();
		// �Q�[���X�^�[�g
		if (cur->GetX() >= 60 && cur->GetY() >= 150 &&
			cur->GetX() <= 260 && cur->GetY() <= 250) {
			DrawBox(60, 150, 260, 250, GetColor(255, 255, 0), FALSE);
		}
		else {
			DrawBox(60, 150, 260, 250, GetColor(255, 255, 255), FALSE);
		}
		// �}�b�v�r���_�[
		if (cur->GetX() >= 380 && cur->GetY() >= 150 &&
			cur->GetX() <= 580 && cur->GetY() <= 250) {
			DrawBox(380, 150, 580, 250, GetColor(255, 255, 0), FALSE);
		}
		else {
			DrawBox(380, 150, 580, 250, GetColor(255, 255, 255), FALSE);
		}
		// �������
		if (cur->GetX() >= 60 && cur->GetY() >= 290 &&
			cur->GetX() <= 260 && cur->GetY() <= 350) {
			DrawBox(60, 290, 260, 350, GetColor(255, 255, 0), FALSE);
		}
		else {
			DrawBox(60, 290, 260, 350, GetColor(255, 255, 255), FALSE);
		}
		// �L�����N�^�[����
		if (cur->GetX() >= 60 && cur->GetY() >= 380 &&
			cur->GetX() <= 260 && cur->GetY() <= 440) {
			DrawBox(60, 380, 260, 440, GetColor(255, 255, 0), FALSE);
		}
		else {
			DrawBox(60, 380, 260, 440, GetColor(255, 255, 255), FALSE);
		}
		// �����L���O
		if (cur->GetX() >= 380 && cur->GetY() >= 290 &&
			cur->GetX() <= 580 && cur->GetY() <= 350) {
			DrawBox(380, 290, 580, 350, GetColor(255, 255, 0), FALSE);
		}
		else {
			DrawBox(380, 290, 580, 350, GetColor(255, 255, 255), FALSE);
		}
		// �`���[�g���A��
		if (cur->GetX() >= 380 && cur->GetY() >= 380 &&
			cur->GetX() <= 580 && cur->GetY() <= 440) {
			DrawBox(380, 380, 580, 440, GetColor(255, 255, 0), FALSE);
		}
		else {
			DrawBox(380, 380, 580, 440, GetColor(255, 255, 255), FALSE);
		}
	}
	// �Q�[���X�^�[�g
	DrawBox(61 ,151,259,249, GetColor(0, 0, 0), TRUE);
	// �}�b�v�r���_�[
	DrawBox(381,151,579,249, GetColor(0, 0, 0), TRUE);
	// �������
	DrawBox(61,291,259,349, GetColor(0, 0, 0), TRUE);
	// �L�����N�^�[����
	DrawBox(61, 381, 259, 439, GetColor(0, 0, 0), TRUE);
	// �����L���O
	DrawBox(381, 291, 579, 349, GetColor(0, 0, 0), TRUE);
	// �`���[�g���A��
	DrawBox(381, 381, 579, 439, GetColor(0, 0, 0), TRUE);

	SetFontSize(50);
	SetFontThickness(1);
	ChangeFont("�S�V�b�N��");
	DrawString(50, 75, "MARIO MAKER ORIGINAL!!", GetColor(0, 0, 0), TRUE);
	SetFontSize(25);
	DrawString(67, 187, "�Q�[���X�^�[�g", GetColor(255, 255, 255), TRUE);
	DrawString(390, 187, "�}�b�v�r���_�[", GetColor(255, 255, 255), TRUE);
	SetFontSize(20);
	DrawString(120, 310, "�������", GetColor(255, 255, 255), TRUE);
	DrawString(80, 400, "�L�����N�^�[����", GetColor(255, 255, 255), TRUE);
	DrawString(430, 310, "�����L���O", GetColor(255, 255, 255), TRUE);
	DrawString(410, 400, "�`���[�g���A��", GetColor(255, 255, 255), TRUE);

}

void StartScreen::ReloadGraph()
{
}











