//---------------------------------------------
// �萔�̐錾
//---------------------------------------------
const int RANKING_POS_X = WINDOW_W / 2 + 150;
const int CENTER_POS_X = WINDOW_W / 2;
const int CENTER_POS_Y = WINDOW_H / 2;
const int RANKING_H = 200;
const int RANKING_SIZE = 4;				// �����L���O�R�@�v�Z�p�ɂP

//---------------------------------------------
// �R���X�g���N�^
//---------------------------------------------
RankingScene::RankingScene()
{
	scene_type = RANKING;
	select_type = RANKING1;
	letter_type = RANKING_MAX;

	// �����L���O�t�@�C���̓ǂݍ���
	RankingFileRead();

	SetGraphHandle("ranking/ranking1.png", RANKING_POS_X, CENTER_POS_Y - RANKING_H, 0.4f);
	SetGraphHandle("ranking/ranking2.png", RANKING_POS_X, CENTER_POS_Y, 0.4f);
	SetGraphHandle("ranking/ranking3.png", RANKING_POS_X, CENTER_POS_Y + RANKING_H, 0.4f);

	flush = 0;

	font_handle.push_back(CreateFontToHandle(NULL, 90, -1));
	font_handle.push_back(CreateFontToHandle(NULL, 60, -1));
}
//---------------------------------------------
// �f�X�g���N�^
//---------------------------------------------
RankingScene::~RankingScene()
{
	
}
//---------------------------------------------
// �X�V����
//---------------------------------------------
void RankingScene::Update()
{
	flush++;

	// �Z���N�g��ς���
	ChangeSelect();
	// ����
	Decision();
	// �ڍׂ̐ݒ�
	SetRankingDetails();

	// �V�[����߂�
	if (IsKeyOn(KEY_INPUT_C) || IsPadOn(PAD_A)) {
		if (letter_type == RANKING_MAX) {
			scene_type = SELECT;
		}
		else {
			letter_type = RANKING_MAX;
		}
	}
}
//---------------------------------------------
// �`�揈��
//---------------------------------------------
void RankingScene::Draw()
{
	// �摜��`��
	DrawGraphHandle();

	if (letter_type == RANKING_MAX) {
		// �����L���O�̕`��
		DrawRanking();
	}
	else {
		// �����L���O�̏ڍׂ̕`��
		DrawRankingDetails();
	}
	

	// �߂镶���̕`��
	DrawCenterString(CENTER_POS_X + 280, CENTER_POS_Y + 350,"A : �߂�", BLACK,font_handle[1]);

#if (NDEBUG)
	for (int i = 0; i < ranking.size() - 1; i++) {
		printfDx("%d�ʁ@%d\n", i + 1, ranking[i]);
	}
#else
#endif
}
//---------------------------------------------
// �Z���N�g��ς���
//---------------------------------------------
void RankingScene::ChangeSelect()
{
	// W�L�[���������Ƃ�
	if (IsKeyOn(KEY_INPUT_W) || IsPadOn(PAD_UP)) {
		// �Z���N�g�^�C�v���}�C�i�X
		select_type--;
		// �Z���N�g�^�C�v���Z���N�g���[�g��菬�����Ȃ�����
		if (select_type < RANKING1) {
			// �Z���N�g�^�C�g���ɂ���
			select_type = RANKING3;
		}
	}
	// S�L�[���������Ƃ�
	if (IsKeyOn(KEY_INPUT_S) || IsPadOn(PAD_DOWN)) {
		// �Z���N�g�^�C�v���v���X
		select_type++;
		// �Z���N�g�^�C�v���Z���N�g�^�C�g�����傫��������
		if (select_type > RANKING3) {
			// �Z���N�g���[�h�ɂ���
			select_type = RANKING1;
		}
	}
}
//---------------------------------------------
// ����
//---------------------------------------------
void RankingScene::Decision()
{
	if ( IsKeyOn(KEY_INPUT_SPACE) || IsPadOn(PAD_B)) {
		if (letter_type != RANKING_MAX) return;
		// �����L���O�P
		if (select_type == RANKING1) {
			letter_type = RANKING1;
		}
		// �����L���O�Q
		if (select_type == RANKING2) {
			letter_type = RANKING2;
		}
		// �����L���O�R
		if (select_type == RANKING3) {
			letter_type = RANKING3;
		}
	}
}
//---------------------------------------------
// �����̃J�[�\����`��
//---------------------------------------------
void RankingScene::LineCursor(int select_type_)
{
	// �����̉������擾
	int x_ = GetDrawFormatStringWidthToHandle(font_handle[0], "%d", ranking[select_type_]);
	int y_ = 0;
	if (select_type_ == RANKING1) y_ = -RANKING_H + 60;
	if (select_type_ == RANKING2) y_ = 60;
	if (select_type_ == RANKING3) y_ = RANKING_H + 60;
	
	// �J�E���g����^�C�~���O
	if (flush % 40 <= 20) {
		// �����̉��̃J�[�\��
		DrawBox(RANKING_POS_X + 200, CENTER_POS_Y + y_,
			RANKING_POS_X + 200 + x_, CENTER_POS_Y + y_ + 5, BLACK, TRUE);
	}
}
//---------------------------------------------
// �����L���O�̕`��
//---------------------------------------------
void RankingScene::DrawRanking()
{
	// �ǂݍ��񂾉摜�̕`��
	
	DrawFormatStringToHandle(RANKING_POS_X + 200, CENTER_POS_Y - RANKING_H - 30, BLACK, font_handle[0], "%d", ranking[0]);
	DrawFormatStringToHandle(RANKING_POS_X + 200, CENTER_POS_Y - 30, BLACK, font_handle[0], "%d", ranking[1]);
	DrawFormatStringToHandle(RANKING_POS_X + 200, CENTER_POS_Y + RANKING_H - 30, BLACK, font_handle[0], "%d", ranking[2]);
	// ���C���J�[�\��
	LineCursor(select_type);
}
//---------------------------------------------
// �ڍׂ̕`��
//---------------------------------------------
void RankingScene::DrawRankingDetails()
{
	if(letter_type == RANKING1) DrawSetPosition("ranking/ranking1.png", CENTER_POS_X + 230,140);
	if(letter_type == RANKING2) DrawSetPosition("ranking/ranking2.png", CENTER_POS_X + 230,140);
	if(letter_type == RANKING3) DrawSetPosition("ranking/ranking3.png", CENTER_POS_X + 230,140);

	DrawCenterFormatString(CENTER_POS_X + 280, CENTER_POS_Y - RANKING_H , BLACK, font_handle[1], "HP : %d��", hp[letter_type]);
	DrawCenterFormatString(CENTER_POS_X + 280, CENTER_POS_Y - 30, BLACK, font_handle[1], "���j�� : %d", defeat_count[letter_type]);
	DrawCenterFormatString(CENTER_POS_X + 280, CENTER_POS_Y + RANKING_H - 30, BLACK, font_handle[1], "SCORE : %d", score[letter_type]);
}
//---------------------------------------------
// �ڍׂ̐ݒ�
//---------------------------------------------
void RankingScene::SetRankingDetails()
{
	switch (letter_type)
	{
	case RANKING1:
		DrawSetPosition("ranking/ranking1.png", CENTER_POS_X + 230, 140);
		DrawSetActive("ranking/ranking2.png", false);
		DrawSetActive("ranking/ranking3.png", false);
		break;
	case RANKING2:
		DrawSetPosition("ranking/ranking2.png", CENTER_POS_X + 230, 140);
		DrawSetActive("ranking/ranking1.png", false);
		DrawSetActive("ranking/ranking3.png", false);
		break;
	case RANKING3:
		DrawSetPosition("ranking/ranking3.png", CENTER_POS_X + 230, 140);
		DrawSetActive("ranking/ranking2.png", false);
		DrawSetActive("ranking/ranking1.png", false);
		break;
	default:
		DrawSetPosition("ranking/ranking1.png", RANKING_POS_X, CENTER_POS_Y - RANKING_H);
		DrawSetPosition("ranking/ranking2.png", RANKING_POS_X, CENTER_POS_Y);
		DrawSetPosition("ranking/ranking3.png", RANKING_POS_X, CENTER_POS_Y + RANKING_H);
		DrawSetActive("ranking/ranking1.png", true);
		DrawSetActive("ranking/ranking2.png", true);
		DrawSetActive("ranking/ranking3.png", true);
		break;
	}
}
//---------------------------------------------
// ranking�t�@�C���ǂݍ���
//---------------------------------------------
void RankingScene::RankingFileRead()
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
		int ranking_, hp_, defeat_count_, score_;
		sstr >> ranking_ >> hp_ >> defeat_count_ >> score_;
		// �����L���O��ۑ�
		ranking.push_back(ranking_);
		hp.push_back(hp_);
		defeat_count.push_back(defeat_count_);
		score.push_back(score_);
	}

	int count = RANKING_SIZE;
	// �t�@�C���ɉ����������܂�Ă��Ȃ��ă����L���O�ɕۑ�����Ȃ���������
	// ���ׂā@0�@�ŕۑ�
	for (int i = (int)ranking.size(); i < count; i++) {
		ranking.push_back(0);
		hp.push_back(0);
		defeat_count.push_back(0);
		score.push_back(0);
	}

	// �t�@�C�������
	file.close();
}
