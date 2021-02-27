#include "ResultScene.h"
//---------------------------------------------
// �萔�̐錾
//---------------------------------------------
const int CENTER_POS_X = WINDOW_W / 2;
const int CENTER_POS_Y = WINDOW_H / 2;
const int RANKING_SIZE = 4;				// �����L���O�R�@�v�Z�p�ɂP

//---------------------------------------------
// �R���X�g���N�^
//---------------------------------------------
ResultScene::ResultScene()
{
	// �V�[���^�C�v
	scene_type = RESULT;
	// ���U���g�t�@�C���ǂݍ���
	ResultFileRead();
	// �����L���O�t�@�C���ǂݍ���
	RankingFileRead();
	// �X�R�A�̃g�[�^�����v�Z
	TotalCalculation();

	if (result) { // win
		SetGraphHandle("result/win/win_background.png",CENTER_POS_X, CENTER_POS_Y);
		SetGraphHandle("result/win/win_Foundation.png", CENTER_POS_X + 400, CENTER_POS_Y, 0.4f);
		SetGraphHandle("result/win/win_character.png", CENTER_POS_X + 400, CENTER_POS_Y - 150, 0.3f);
		SetGraphHandle("result/win/win_letter.png", CENTER_POS_X - 450, CENTER_POS_Y - 350, 0.4f);
		SetGraphHandle("result/win/win_frame.png", CENTER_POS_X, CENTER_POS_Y,0.4f);
	}
	else {// lose
		SetGraphHandle("result/lose/lose_background.png", CENTER_POS_X, CENTER_POS_Y);
		SetGraphHandle("result/lose/lose_spotlight.png", CENTER_POS_X + 250, CENTER_POS_Y - 100, 0.4f);
		SetGraphHandle("result/lose/lose_skeleton.png", CENTER_POS_X + 400, CENTER_POS_Y + 50, 0.38f);
		SetGraphHandle("result/lose/lose_letter.png", CENTER_POS_X - 350, CENTER_POS_Y - 350, 0.4f);
		SetGraphHandle("result/lose/lose_frame.png", CENTER_POS_X, CENTER_POS_Y, 0.4f);
	}

	font_handle.push_back(CreateFontToHandle(NULL, 90, -1));
}
//---------------------------------------------
// �f�X�g���N�^
//---------------------------------------------
ResultScene::~ResultScene()
{

}
//---------------------------------------------
// �X�V����
//---------------------------------------------
void ResultScene::Update()
{
	if (IsKeyOn(KEY_INPUT_SPACE) || IsPadOn(PAD_B)) {
		// �����L���O��ʂɈړ�
		scene_type = SELECT;
		// �����L���O�̌v�Z
		RankingCalculation();
		// �����L���O�t�@�C���Ƀ����L���O����������
		RankingFileWrite();
	}
}
//---------------------------------------------
// �`�揈��
//---------------------------------------------
void ResultScene::Draw()
{
	// �摜�̕`��
	DrawGraphHandle();

	unsigned int color;
	if (result) { // win
		color = BLACK;
	}
	else {	// lose
		color = GetColor(169, 190, 244);
	}
	// HP�����̕\��
	DrawCenterFormatString(600, 400, color, font_handle[0], "HP : %d��", hp);
	// ���j���̕\��
	DrawCenterFormatString(600, 550, color, font_handle[0], "���j�� : %d", defeat_count);
	// ���v�X�R�A�̕\��
	DrawFormatStringToHandle(300, 800, color, font_handle[0], "TOTAL : %d", total);
	DrawBox(300, 700, 900, 710, color, TRUE);


#if (NDEBUG)
	for (int i = 0; i < ranking.size(); i++) {
		printfDx("%d�� %d %d %d %d \n", i + 1, ranking[i],hp_v[i],defeat_count_v[i],score_v[i]);
	}
#else
#endif
}
//---------------------------------------------
// result�t�@�C���ǂݍ���
//---------------------------------------------
void ResultScene::ResultFileRead()
{
	file.open("data/txt/Result.txt", ios::in);

	if (!file.is_open()) {
		// �t�@�C�����J�����ƂɎ��s���܂���
		return;
	}

	string str;
	while (getline(file, str))
	{
		stringstream sstr(str);
		// win��lose���@�X�R�A�@HP�@���j��
		sstr >> result >> score >> hp >> defeat_count;
	}

	// �t�@�C�������
	file.close();
}
//---------------------------------------------
// ranking�t�@�C���ǂݍ���
//---------------------------------------------
void ResultScene::RankingFileRead()
{
	file.open("data/txt/Ranking.txt", ios::in);

	if (!file.is_open()) {
		// �t�@�C�����J�����ƂɎ��s���܂���
		return;
	}

	string str;
	while (getline(file, str))
	{
		stringstream sstr(str);
		int ranking_ ,hp_,defeat_count_, score_;
		sstr >> ranking_ >> hp_ >> defeat_count_ >> score_;
		// �����L���O��ۑ�
		ranking.push_back(ranking_);
		hp_v.push_back(hp_);
		defeat_count_v.push_back(defeat_count_);
		score_v.push_back(score_);
	}

	int count = RANKING_SIZE;
	// �t�@�C���ɉ����������܂�Ă��Ȃ��ă����L���O�ɕۑ�����Ȃ���������
	// ���ׂā@0�@�ŕۑ�
	for (int i = (int)ranking.size(); i < count;i++) {
		ranking.push_back(0);
		hp_v.push_back(0);
		defeat_count_v.push_back(0);
		score_v.push_back(0);
	}

	// �t�@�C�������
	file.close();
}
//---------------------------------------------
// ranking�t�@�C����������
//---------------------------------------------
void ResultScene::RankingFileWrite()
{
	file.open("data/txt/Ranking.txt", ios::out);

	if (!file.is_open()) {
		// �t�@�C�����J�����ƂɎ��s���܂���
		return;
	}
	
	// �����L���O���t�@�C���ɏ�������
	for (int i = 0; i < (int)ranking.size() - 1; i++) {
		file << ranking[i]<< ' ' << hp_v[i]<<' '<<defeat_count_v[i]<<' '<<score_v[i]<< '\n';
	}

	// �t�@�C�������
	file.close();
}
//---------------------------------------------
// �����L���O�̌v�Z
//---------------------------------------------
void ResultScene::RankingCalculation()
{
	// �����L���O�������猩�Ă���
	for (int i = (int)ranking.size() - 2; i > -1; --i) {
		// �X�R�A���v�̕����傫��������
		if (ranking[i] < total) {
			// �����L���O�ɉ��ɌJ�艺��
			ranking[i + 1] = ranking[i];
			hp_v[i + 1] = hp_v[i];
			defeat_count_v[i + 1] = defeat_count_v[i];
			score_v[i + 1] = score_v[i];
			// �����L���O�ɓo�^
			ranking[i] = total;
			hp_v[i] = hp;
			defeat_count_v[i] = defeat_count;
			score_v[i] = score;
		}
	}
}
//---------------------------------------------
// �g�[�^���̌v�Z
//---------------------------------------------
void ResultScene::TotalCalculation()
{
	if (result) {
		total += 10000;
	}
	total += hp * 123;
	total += defeat_count * 100;
	total += score;
}
