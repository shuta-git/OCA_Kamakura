#pragma once

class LoadScene : public SceneBase
{
public:
	LoadScene();
	~LoadScene();

	void Update() override;
	void Draw() override;

private:

	//=======================================
	// �ϐ�
	//=======================================
	int handle_count;	// �n���h���J�E���g
	int load_count;		// ���[�h�J�E���g
	int gauge_count;	// �Q�[�W�p�̃J�E���g
	int load_type;		// ���[�h�^�C�v
	int load_time;		// ���[�h����
	int load_str_num;	// ���[�h�����̔ԍ�


	// �t�@�C���ǂݍ��ݗp
	fstream file;
	//�@���f���p�X��ۑ��p
	std::vector<string>model_path;
	// �摜�p�X��ۑ��p
	std::vector<string>graph_path;
	// �G�t�F�N�g�p�X�ۑ��p
	std::vector<string>effect_path;
	// ���[�h�̕����p��
	std::vector<string>load_str;

	// �ǂݍ��݉�ʕ`��֐�
	void DrawLoadingScreen();
	// ���[�h���f���n���h��
	void LoadModelHandle(string str);
	// ���[�h�O���t�B�b�N�n���h��
	void LoadGraphHandle(string str);
	// ���[�h�G�t�F�N�g�n���h��
	void LoadEffectHandle(string str);
	// ���f���p�X�t�@�C����ǂݍ���
	void ReadModelFilePath();
	// �摜�p�X�t�@�C����ǂݍ���
	void ReadGraphFilePath();
	// �G�t�F�N�g�p�X�t�@�C����ǂݍ���
	void ReadEffectFilePath();
	// ���[�h�^�C���̍X�V����
	// load_str_num ���T�C�Y���������Ƃ��ɉ��Ԃɒ������邩
	void LoadUpdate(int num);

	enum LOAD_TYPE {
		MODEL = 0,
		GRAPH,
		EFFECT,

		FINISH,
	};
};

