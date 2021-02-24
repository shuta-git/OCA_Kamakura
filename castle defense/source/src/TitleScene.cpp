//---------------------------------------------
// �萔�̐錾
//---------------------------------------------
const int STRING_X = WINDOW_W / 2 - 200;	// �����̍��WX
const int STRING_Y = WINDOW_H - 100;		// �����̍��WY

//---------------------------------------------
// �R���X�g���N�^
//---------------------------------------------
TitleScene::TitleScene()
{
	// ���̃V�[���^�C�v���w��
	scene_type = TITLE;
	// �摜�̃t�@�C���p�X��ݒ�
	SetGraphHandle("title/title2_1.png",(WINDOW_W /2),(WINDOW_H / 2));
	// ���W���w��
	pos = { WINDOW_W / 2,WINDOW_H / 2,0.0f };
	// �T�C�Y��錾
	scale = { 130.0f,130.0f,130.0f };
	// �G�t�F�N�g�̐���
	effect.push_back(new Effect("Atmosphere.efk", pos, false));
	effect.push_back(new Effect("Atmosphere.efk", pos, false));
	effect.push_back(new Effect("Atmosphere.efk", pos, false));
	effect.push_back(new Effect("Atmosphere.efk", pos, false));
	effect.push_back(new Effect("Atmosphere.efk", pos, false));

	flush = 0;

	// �t�H���g��ݒ�
	font_handle.push_back( CreateFontToHandle(NULL, 70, -1));
}
//---------------------------------------------
// �f�X�g���N�^
//---------------------------------------------
TitleScene::~TitleScene()
{
	// �t�H���g�n���h��������
	DeleteFontHandle();
}
//---------------------------------------------
// �X�V����
//---------------------------------------------
void TitleScene::Update()
{
	// �_�ŗp�̃J�E���g����
	flush++;

	if (IsKeyOn(KEY_INPUT_SPACE) || IsPadOn(PAD_B)) {
		scene_type = SELECT;
	}

	// �G�t�F�N�g�̏���
	for (int i = 0; i < effect.size(); i++) {
		// �G�t�F�N�g�̃T�C�Y���w��
		effect[i]->SetScale(scale);
		// �G�t�F�N�g�̍X�V����
		effect[i]->Update();
		// �f���[�g����
		DeleteEffect(i);
	}
}
//---------------------------------------------
// �`�揈��
//---------------------------------------------
void TitleScene::Draw()
{
	// �摜��`��
	DrawGraphHandle();

	// �G�t�F�N�g�̕`��
	for (int i = 0; i < effect.size(); i++) {
		effect[i]->Draw();
	}
	// �J�E���g����^�C�~���O
	if (flush % 40 <= 20) {
		DrawStringToHandle(STRING_X, STRING_Y, "Push B Button", WHITE, font_handle[0]);
	}

	printfDx("�^�C�g�� \n");
}
