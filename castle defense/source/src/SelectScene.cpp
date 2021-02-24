#include "SelectScene.h"
//---------------------------------------------
// �萔�̐錾
//---------------------------------------------
const int SELECT_X = WINDOW_W / 2 + 280;	// �Z���N�g�����̊���WX
const int GAME_Y = 300;						// �Q�[���X�^�[�g�����̍��WY
const int OPERATION_Y =490;					// ������������̍��WY
const int RANKING_Y = 680;					// �����L���O�����̍��WY
const int TITLE_Y = 870;					// �^�C�g�������̍��WY

//---------------------------------------------
// �R���X�g���N�^
//---------------------------------------------
SelectScene::SelectScene()
{
	// ���̃V�[���^�C�v���w��
	scene_type = SELECT;
	// �Z���N�g�^�C�v��ݒ�
	select_type = SELECT_LOAD;
	// �摜�̃t�@�C���p�X��ݒ�
	SetGraphHandle("select/select_background1.png", (WINDOW_W / 2), (WINDOW_H / 2));
	SetGraphHandle("select/select_character1.png", (WINDOW_W / 4), (WINDOW_H / 2) , 0.4f);
	SetGraphHandle("select/select_UI1.png", WINDOW_W - 700, (WINDOW_H / 2),0.5f);

	ranking = nullptr;

	// �Z���N�g����
	font_handle.push_back(CreateFontToHandle(NULL, 70, -1));
	font_handle.push_back(CreateFontToHandle(NULL, 90, -1));
}
//---------------------------------------------
// �f�X�g���N�^
//---------------------------------------------
SelectScene::~SelectScene()
{
}
//---------------------------------------------
// �X�V����
//---------------------------------------------
void SelectScene::Update()
{
	if (ranking != nullptr) {
		ranking->Update();
		//�@�V�[���̃^�C�v���Z���N�g�ɂȂ�����
		if (ranking->GetSceneType() == SELECT) {
			// �t�H���g�n���h��������
			ranking->DeleteFontHandle();
			// �����L��������
			delete ranking;
			ranking = nullptr;
		}
	}else{
		// �Z���N�g��ς���
		ChangeSelect();
		// ����
		Decision();
	}

}
//---------------------------------------------
// �`�揈��
//---------------------------------------------
void SelectScene::Draw()
{
	// �摜��`��
	DrawGraphHandle();

	if (ranking != nullptr) {
		ranking->Draw();
	}
	else {
		DrawStringType(SELECT_LOAD,GAME_Y,"�Q�[���X�^�[�g");
		DrawStringType(SELECT_OPERATION, OPERATION_Y,"�������");
		DrawStringType(SELECT_RANKING, RANKING_Y,"�����L���O");
		DrawStringType(SELECT_TITLE, TITLE_Y,"�^�C�g��");
	}

}
//---------------------------------------------
// �Z���N�g��ς���
//---------------------------------------------
void SelectScene::ChangeSelect()
{
	// W�L�[���������Ƃ�
	if (IsKeyOn(KEY_INPUT_W) || IsPadOn(PAD_UP)) {
		// �Z���N�g�^�C�v���}�C�i�X
		select_type--;
		// �Z���N�g�^�C�v���Z���N�g���[�g��菬�����Ȃ�����
		if (select_type < SELECT_LOAD) {
			// �Z���N�g�^�C�g���ɂ���
			select_type = SELECT_TITLE;
		}
	}
	// S�L�[���������Ƃ�
	if (IsKeyOn(KEY_INPUT_S) || IsPadOn(PAD_DOWN)) {
		// �Z���N�g�^�C�v���v���X
		select_type++;
		// �Z���N�g�^�C�v���Z���N�g�^�C�g�����傫��������
		if (select_type > SELECT_TITLE) {
			// �Z���N�g���[�h�ɂ���
			select_type = SELECT_LOAD;
		}
	}
}
//---------------------------------------------
// ����
//---------------------------------------------
void SelectScene::Decision()
{
	if (IsKeyOn(KEY_INPUT_SPACE) || IsPadOn(PAD_B)) {
		if (select_type == SELECT_LOAD) {
			scene_type = LOAD;
		}
		if (select_type == SELECT_OPERATION) {
			scene_type = OPERATION;
		}
		if (select_type == SELECT_RANKING) {
			if (ranking == nullptr) {
				ranking = new RankingScene();
			}
		}
		if (select_type == SELECT_TITLE) {
			scene_type = TITLE;
		}
	}
}
//---------------------------------------------
// �����̎�ނŕ`��
//---------------------------------------------
void SelectScene::DrawStringType(int type_, int y_, string str_)
{
	// ����
	if (select_type == type_) {
		DrawCenterString(SELECT_X, y_, str_.c_str(), SKYBLUE, font_handle[FONT_90]);
	}
	else {
		DrawCenterString(SELECT_X, y_, str_.c_str(), BLACK, font_handle[FONT_70]);
	}
}
