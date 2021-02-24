#include "PauseScene.h"
//---------------------------------------------
// �萔�̐錾
//---------------------------------------------
const int PAUSE_X = WINDOW_W / 2;	// �Z���N�g�����̊���WX
const int GAME_Y = 300;				// �Q�[���ɖ߂�̍��WX
const int LOAD_Y = 420;				// ���X�^�[�g�����̍��WX
const int OPERATION_Y = 540;		// ������������̍��WX
const int SELECT_Y = 660;			// �Z���N�g��ʂɖ߂�̍��WX	
const int TITLE_Y = 780;			// �^�C�g���ɖ߂�̍��WX

//---------------------------------------------
// �R���X�g���N�^
//---------------------------------------------
PauseScene::PauseScene()
{
	scene_type = PAUSE;

	// �摜�̃t�@�C���p�X��ݒ�
	SetGraphHandle("pause/pause.png", (WINDOW_W / 2), (WINDOW_H / 2),0.4f);
	SetGraphHandle("pause/character.png", (WINDOW_W / 2) + 450, (WINDOW_H / 2) + 330,0.3f);
	SetGraphHandle("pause/arrow.png", (WINDOW_W / 2), (WINDOW_H / 2),0.3f);

	// �Z���N�g����
	font_handle.push_back(CreateFontToHandle(NULL, 50, -1));
	font_handle.push_back(CreateFontToHandle(NULL, 60, -1));
}
//---------------------------------------------
// �f�X�g���N�^
//---------------------------------------------
PauseScene::~PauseScene()
{
	
}
//---------------------------------------------
// �X�V����
//---------------------------------------------
void PauseScene::Update()
{
	ChangeSelect();
	Decision();

	// ���̃|�W�V�������W�ݒ�
	DrawSetPosition("pause/arrow.png", arrow_pos_x - 40, arrow_pos_y + 30);
}
//---------------------------------------------
// �`�揈��
//---------------------------------------------
void PauseScene::Draw()
{
	// �摜��`��
	DrawGraphHandle();

	DrawStringType(PAUSE_GAME,GAME_Y,"�Q�[���ɖ߂�");
	DrawStringType(PAUSE_LOAD, LOAD_Y,"���X�^�[�g");
	DrawStringType(PAUSE_OPERATION, OPERATION_Y,"�������");
	DrawStringType(PAUSE_SELECT, SELECT_Y,"�Z���N�g��ʂɖ߂�");
	DrawStringType(PAUSE_TITLE, TITLE_Y,"�^�C�g��");
}
//---------------------------------------------
// �Z���N�g��ς���
//---------------------------------------------
void PauseScene::ChangeSelect()
{
	// W�L�[���������Ƃ�
	if (IsKeyOn(KEY_INPUT_W) || IsPadOn(PAD_UP)) {
		// �Z���N�g�^�C�v���}�C�i�X
		select_type--;
		// �Z���N�g�^�C�v���Z���N�g���[�g��菬�����Ȃ�����
		if (select_type < PAUSE_GAME) {
			// �Z���N�g�^�C�g���ɂ���
			select_type = PAUSE_TITLE;
		}
	}

	// S�L�[���������Ƃ�
	if (IsKeyOn(KEY_INPUT_S) || IsPadOn(PAD_DOWN)) {
		// �Z���N�g�^�C�v���v���X
		select_type++;
		// �Z���N�g�^�C�v���Z���N�g�^�C�g�����傫��������
		if (select_type > PAUSE_TITLE) {
			// �Z���N�g���[�h�ɂ���
			select_type = PAUSE_GAME;
		}
	}
}
//---------------------------------------------
// ����
//---------------------------------------------
void PauseScene::Decision()
{
	if (IsKeyOn(KEY_INPUT_SPACE) || IsPadOn(PAD_B)) {
		if (select_type == PAUSE_GAME) {
			scene_type = GAME_MAIN;
		}
		if (select_type == PAUSE_LOAD) {
			scene_type = LOAD;
		}
		if (select_type == PAUSE_OPERATION) {
			scene_type = OPERATION;
		}
		if (select_type == PAUSE_SELECT) {
			scene_type = SELECT;
		}
		if (select_type == PAUSE_TITLE) {
			scene_type = TITLE;
		}
	}
}
//---------------------------------------------
// �����̎�ނŕ`��
//---------------------------------------------
void PauseScene::DrawStringType(int type_, int y_, string str_)
{
	// ����
	if (select_type == type_) {
		DrawCenterString(PAUSE_X, y_, str_.c_str(), SKYBLUE, font_handle[FONT_60] );
		arrow_pos_x = GetStringPosX(PAUSE_X,str_, font_handle[FONT_60]);
		arrow_pos_y = y_;
	}
	else {
		DrawCenterString(PAUSE_X, y_, str_.c_str(), BLACK, font_handle[FONT_50]);
	}
}
