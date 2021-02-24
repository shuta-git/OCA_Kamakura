#include "PauseScreen.h"


//----------------------------------------------------------------------
// �R���X�g���N�^
//----------------------------------------------------------------------
PauseScreen::PauseScreen()
{
	start = false;
	operation = false;
	game = false;
	spawn = false;
	pause_num = PAUSE_GAME;
}
//----------------------------------------------------------------------
// �f�X�g���N�^
//----------------------------------------------------------------------
PauseScreen::~PauseScreen()
{
}
//----------------------------------------------------------------------
// ����������
//----------------------------------------------------------------------
void PauseScreen::Init()
{
}
//----------------------------------------------------------------------
// �X�V����
//----------------------------------------------------------------------
void PauseScreen::Update()
{
#if ! defined(NDEBUG)
	KeySelect();
#else
	SelectChange();
	ConSelect();
#endif
}
//----------------------------------------------------------------------
// �`�揈��
//----------------------------------------------------------------------
void PauseScreen::Draw(Font font, Font font1)
{
#if ! defined(NDEBUG)
	DrawMouse(font, font1);
#else
	DrawCon(font, font1);
#endif

}
//----------------------------------------------------------------------
// �I������
//----------------------------------------------------------------------
void PauseScreen::Exit()
{
}
//----------------------------------------------------------------------
// "�Q�[���ɖ߂�" �̓����蔻��p
//----------------------------------------------------------------------
RectF PauseScreen::GetGame() const
{
	return RectF(WINDOW_W / 2 - 240, WINDOW_H / 2 - WINDOW_H / 3 - 70, 440, 75);
}
//----------------------------------------------------------------------
// "�������" �̓����蔻��p
//----------------------------------------------------------------------
RectF PauseScreen::GetOperation() const
{
	return RectF(WINDOW_W / 2 - 170, WINDOW_H / 2 - 230, 300, 75);
}
//----------------------------------------------------------------------
// "�X�^�[�g�����蒼��" �̓����蔻��p
//----------------------------------------------------------------------
RectF PauseScreen::GetSpawn() const
{
	return RectF(WINDOW_W / 2 - 380, WINDOW_H / 2 - 30, 740, 75);
}
//----------------------------------------------------------------------
// "�X�^�[�g��ʂɖ߂�" �̓����蔻��
//----------------------------------------------------------------------
RectF PauseScreen::GetStart() const
{
	return RectF(WINDOW_W / 2 - 350, WINDOW_H / 2 + 170, 665, 75);
}
//----------------------------------------------------------------------
// �L�[�{�[�h�őI��
//----------------------------------------------------------------------
void PauseScreen::KeySelect()
{
	// "�Q�[���ɖ߂�" ���N���b�N������
	if (Mouse::Pos().intersects(GetGame())) {
		if (Input::MouseL.clicked) {
			game = true;
		}
	}
	//  "�������" ���N���b�N������
	if (Mouse::Pos().intersects(GetOperation())) {
		if (Input::MouseL.clicked) {
			operation = true;
		}
	}
	// "�X�^�[�g�����蒼��" ���N���b�N������
	if (Mouse::Pos().intersects(GetSpawn())) {
		if (Input::MouseL.clicked) {
			spawn = true;
		}
	}
	// "�X�^�[�g��ʂɖ߂�" ���N���b�N������
	if (Mouse::Pos().intersects(GetStart())) {
		if (Input::MouseL.clicked) {
			start = true;
		}
	}
}
//----------------------------------------------------------------------
// �R���g���[���őI��
//----------------------------------------------------------------------
void PauseScreen::ConSelect()
{
	if (pause_num == PAUSE_GAME) {
		if (controller.buttonA.clicked) {
			game = true;
		}
	}
	if (pause_num == PAUSE_OPERATION) {
		if (controller.buttonA.clicked) {
			operation = true;
		}
	}
	if (pause_num == PAUSE_SPAWN) {
		if (controller.buttonA.clicked) {
			spawn = true;
		}
	}
	if (pause_num == PAUSE_START) {
		if (controller.buttonA.clicked) {
			start = true;
		}
	}

}
//----------------------------------------------------------------------
// �Z���N�g��ς���
//----------------------------------------------------------------------
void PauseScreen::SelectChange()
{
	if (controller.buttonUp.clicked ) {
		pause_num--;
		if(pause_num < PAUSE_GAME){
			pause_num = PAUSE_START;
		}
	}
	if (controller.buttonDown.clicked) {
		pause_num++;
		if (pause_num > PAUSE_START) {
			pause_num = PAUSE_GAME;
		}
	}
}
//----------------------------------------------------------------------
// �L�[�{�[�h�`��
//----------------------------------------------------------------------
void PauseScreen::DrawMouse(Font font, Font font1)
{
	if (Mouse::Pos().intersects(GetGame())) {
		font1(L"�Q�[���ɖ߂�").draw(WINDOW_W / 2 - 260, WINDOW_H / 2 - WINDOW_H / 3 - 80, Palette::Yellow);
	}
	else {
		font(L"�Q�[���ɖ߂�").draw(WINDOW_W / 2 - 240, WINDOW_H / 2 - WINDOW_H / 3 - 80);
	}
	if (Mouse::Pos().intersects(GetOperation())) {
		font1(L"�������").draw(WINDOW_W / 2 - 190, WINDOW_H / 2 - 240, Palette::Yellow);
	}
	else {
		font(L"�������").draw(WINDOW_W / 2 - 170, WINDOW_H / 2 - 240);
	}
	if (Mouse::Pos().intersects(GetSpawn())) {
		font1(L"�X�^�[�g�����蒼��").draw(WINDOW_W / 2 - 400, WINDOW_H / 2 - 40, Palette::Yellow);
	}
	else {
		font(L"�X�^�[�g�����蒼��").draw(WINDOW_W / 2 - 380, WINDOW_H / 2 - 40);
	}
	if (Mouse::Pos().intersects(GetStart())) {
		font1(L"�X�^�[�g��ʂɖ߂�").draw(WINDOW_W / 2 - 370, WINDOW_H / 2 + 160, Palette::Yellow);
	}
	else {
		font(L"�X�^�[�g��ʂɖ߂�").draw(WINDOW_W / 2 - 350, WINDOW_H / 2 + 160);
	}
}
//----------------------------------------------------------------------
// �R���g���[���[�`��
//----------------------------------------------------------------------
void PauseScreen::DrawCon(Font font, Font font1)
{
	if (pause_num == PAUSE_GAME) {
		font1(L"�Q�[���ɖ߂�").draw(WINDOW_W / 2 - 260, WINDOW_H / 2 - WINDOW_H / 3 - 80, Palette::Yellow);
	}
	else {
		font(L"�Q�[���ɖ߂�").draw(WINDOW_W / 2 - 240, WINDOW_H / 2 - WINDOW_H / 3 - 80);
	}
	if (pause_num == PAUSE_OPERATION) {
		font1(L"�������").draw(WINDOW_W / 2 - 190, WINDOW_H / 2 - 240, Palette::Yellow);
	}
	else {
		font(L"�������").draw(WINDOW_W / 2 - 170, WINDOW_H / 2 - 240);
	}
	if (pause_num == PAUSE_SPAWN) {
		font1(L"�X�^�[�g�����蒼��").draw(WINDOW_W / 2 - 400, WINDOW_H / 2 - 40, Palette::Yellow);
	}
	else {
		font(L"�X�^�[�g�����蒼��").draw(WINDOW_W / 2 - 380, WINDOW_H / 2 - 40);
	}
	if (pause_num == PAUSE_START) {
		font1(L"�X�^�[�g��ʂɖ߂�").draw(WINDOW_W / 2 - 370, WINDOW_H / 2 + 160, Palette::Yellow);
	}
	else {
		font(L"�X�^�[�g��ʂɖ߂�").draw(WINDOW_W / 2 - 350, WINDOW_H / 2 + 160);
	}
}
