//---------------------------------------------
// �萔�̐錾
//---------------------------------------------
const int GAUGE_W = (WINDOW_W - 40);		// �Q�[�W�̉���
const int GAUGE_H = 24;						// �Q�[�W�̏c��
const int GAUGE_X = 20;						// �Q�[�W��X���W
const int GAUGE_Y = WINDOW_H - 60;			// �Q�[�W��Y���W
const int MAX_LOAD_TIME = 10;				// �ő働�[�h�^�C��


//---------------------------------------------
// �R���X�g���N�^
//---------------------------------------------
LoadScene::LoadScene()
{
	// ������
	handle_count = 0;
	load_count = 0;
	load_time = 0;
	load_str_num = 0;
	font_handle.push_back(CreateFontToHandle(NULL, 20, -1));

	// ���̃V�[���^�C�v���w��
	scene_type = LOAD;
	// �ŏ��ɓǂݍ��ގ�ނ��w��
	load_type = MODEL;
	// �t�@�C����ǂݍ���
	ReadModelFilePath();
	ReadGraphFilePath();
	ReadEffectFilePath();
	// �����������
	load_str.push_back("Now Loading .");
	load_str.push_back("Now Loading ..");
	load_str.push_back("Now Loading ...");
	//  �摜�̃t�@�C���p�X��ݒ�
	SetGraphHandle("road/road1_1.png", (WINDOW_W / 2), (WINDOW_H / 2));
}
//---------------------------------------------
// �f�X�g���N�^
//---------------------------------------------
LoadScene::~LoadScene()
{

}
//---------------------------------------------
// �X�V����
//---------------------------------------------
void LoadScene::Update()
{

	LoadUpdate(0);

	switch (load_type) {
		// ���f��
	case MODEL:
		// �ǂݍ��ރ��f���̐��ƃ��[�h�������������Ȃ�
		if(model_path.size() == load_count)
		{
			// �J�E���g������
			load_count = 0;
			// ���[�h�̎�ނ��摜�ɕύX
			load_type = GRAPH;
		}
		else {
			// ���f�����[�h�֐�
			LoadModelHandle(model_path[load_count].c_str());
		}
		break;
		// �摜
	case GRAPH:
		// �ǂݍ��މ摜�̐��ƃ��[�h�������������Ȃ�
		if (graph_path.size() == load_count)
		{
			// �J�E���g������
			load_count = 0;
			// ���[�h�̎�ނ��G�t�F�N�g�ɕύX
			load_type = EFFECT;
		}
		else {
			// �摜���[�h�֐�
			LoadGraphHandle(graph_path[load_count].c_str());
		}
		break;
		// �G�t�F�N�g
	case EFFECT:
		// �ǂݍ��ރG�t�F�N�g�̐��ƃ��[�h�������������Ȃ�
		if (effect_path.size() == load_count)
		{
			// �J�E���g������
			load_count = 0;
			// ���[�h�̎�ނ��t�B�j�b�V���ɕύX
			load_type = FINISH;
		}
		else {
			// �G�t�F�N�g���[�h�֐�
			LoadEffectHandle(effect_path[load_count].c_str());
		}
		break;
	}

#if(NDEBUG)
	// ���[�h���I�������
	if (load_type == FINISH && CheckHitKey(KEY_INPUT_SPACE)) {
		// �V�[�����Q�[�����C���ɐ؂�ւ�
		scene_type = GAME_MAIN;
	}
#else
	// ���[�h���I�������
	if (load_type == FINISH) {
		// �V�[�����Q�[�����C���ɐ؂�ւ�
		scene_type = GAME_MAIN;
	}
#endif
}
//---------------------------------------------
// �`�揈��
//---------------------------------------------
void LoadScene::Draw()
{
	// �摜�̕`��
	DrawGraphHandle();
	// �ǂݍ��݉�ʕ`��֐�
	DrawLoadingScreen();

	//DrawStringToHandle(WINDOW_W,WINDOW_H );

#if (NDEBUG)
	printfDx("���[�h \n");
	for (int i = 0; i < model_path.size(); i ++) {
		printfDx(model_path[i].c_str());
		printfDx("\n");
	}
	for (int i = 0; i < graph_path.size(); i++) {
		printfDx(graph_path[i].c_str());
		printfDx("\n");
	}
	for (int i = 0; i < effect_path.size(); i++) {
		printfDx(effect_path[i].c_str());
		printfDx("\n");
	}
	printfDx("%d \n",handle_count);
	printfDx("%d \n", gauge_count);
#endif
}
//---------------------------------------------
// �ǂݍ��݉�ʕ`��֐�
//---------------------------------------------
void LoadScene::DrawLoadingScreen()
{
	// ���[�h�̕����ƃQ�[�W�̍��W�p
	int loading_x, loading_y;
	
	loading_x = GAUGE_X;
	loading_y = GAUGE_Y - 20;
	// ���[�h�̕���
	DrawStringToHandle(loading_x, loading_y, load_str[load_str_num].c_str(), GetColor(255, 255, 255), font_handle[0]);

	loading_x = GAUGE_X;
	loading_y = GAUGE_Y;
	// �Q�[�W�̑S�̂��D�F�ŕ`��
	DrawBox(loading_x, loading_y, loading_x + GAUGE_W, loading_y + GAUGE_H, GetColor(125, 125, 125), TRUE);

	// �����_�œǂݍ��ݏI����Ă��镪�𔒐F�ŕ`��
	DrawBox(loading_x, loading_y,loading_x +( gauge_count * GAUGE_W / handle_count), loading_y + GAUGE_H, SKYBLUE, TRUE);
}
//---------------------------------------------
// ���f���p�X�t�@�C����ǂݍ���
//---------------------------------------------
void LoadScene::ReadModelFilePath()
{
	file.open("data/txt/LoadModelPath.txt", ios::in);

	if (!file.is_open()) {
		// �t�@�C�����J�����ƂɎ��s���܂���
		return;
	}

	string str;
	while (getline(file, str))
	{
		model_path.push_back(str);
		handle_count++;
	}

	// �t�@�C�������
	file.close();
}
//---------------------------------------------
// �摜�p�X�t�@�C����ǂݍ���
//---------------------------------------------
void LoadScene::ReadGraphFilePath()
{
	file.open("data/txt/LoadGraphPath.txt", ios::in);

	if (!file.is_open()) {
		// �t�@�C�����J�����ƂɎ��s���܂���
		return;
	}

	string str;
	while (getline(file, str))
	{
		graph_path.push_back(str);
		handle_count++;
	}

	// �t�@�C�������
	file.close();
}
//---------------------------------------------
//  �G�t�F�N�g�p�X�t�@�C����ǂݍ���
//---------------------------------------------
void LoadScene::ReadEffectFilePath()
{
	file.open("data/txt/LoadEffectPath.txt", ios::in);

	if (!file.is_open()) {
		// �t�@�C�����J�����ƂɎ��s���܂���
		return;
	}

	string str;
	while (getline(file, str))
	{
		effect_path.push_back(str);
		handle_count++;
	}

	// �t�@�C�������
	file.close();
}
//---------------------------------------------
// ���[�h�^�C���̍X�V����
// load_str_num ���T�C�Y���������Ƃ��ɉ��Ԃɒ������邩
//---------------------------------------------
void LoadScene::LoadUpdate(int num)
{
	load_time++;
	if (load_time % MAX_LOAD_TIME == 0) {
		load_time = 0;
		load_str_num++;
	}
	if (load_str_num == load_str.size()) {
		load_str_num = num;
	}
}
//---------------------------------------------
// ���[�h���f���n���h��
//---------------------------------------------
void LoadScene::LoadModelHandle(string str)
{
	// �p�X��ݒ�
	string data = "data/";
	string path = data + str;
	resource::LoadModel(path.c_str());
	load_count++;
	gauge_count++;
}
//---------------------------------------------
// ���[�h�O���t�B�b�N�n���h��
//---------------------------------------------
void LoadScene::LoadGraphHandle(string str)
{
	// �p�X��ݒ�
	string data = "data/";
	string path = data + str;
	resource::LoadGraph(path.c_str());
	load_count++;
	gauge_count++;
}
//---------------------------------------------
// ���[�h�G�t�F�N�g�n���h��
//---------------------------------------------
void LoadScene::LoadEffectHandle(string str)
{
	// �p�X��ݒ�
	string data = "data/effect/";
	string path = data + str;
	resource::LoadEffekseerEffect(path.c_str());
	load_count++;
	gauge_count++;
}
