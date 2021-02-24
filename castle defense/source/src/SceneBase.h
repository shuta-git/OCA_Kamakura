#pragma once

//---------------------------------------------
// SceneBase�N���X
//---------------------------------------------
class SceneBase
{
public:
	SceneBase();
	~SceneBase();

	virtual void Update() = 0;
	virtual void Draw() = 0;

	// �V�[���̎�ނ��擾
	int GetSceneType() const;
	// �V�[���̎�ނ�ݒ�
	void SetSceneType(int scene_type_);
	// �t�H���g�n���h��������
	void DeleteFontHandle();

protected:

	int arrow_pos_x;	// ����X���W
	int arrow_pos_y;	// ����Y���W
	int scene_type;		// �V�[���̎��
	int old_scene;		// ��O�̃V�[��
	int x, y;			// ���W
	float size;			// �T�C�Y
	Vector3 pos;		// Vector�N���X
	Vector3 scale;		// �T�C�Y
	std::vector<Effect*>effect;		// �G�t�F�N�g
	std::vector<int>font_handle;	// �t�H���g�f�[�^

	// �摜�̃p�X��ݒ�
	void SetGraphHandle(string str,int x_ = 0,int y_ = 0, float size_ = 1.0f);
	// �摜�̍��W��ݒ�
	void DrawSetPosition(string str,int x_ = 0,int y_ = 0);
	// �����x�̐ݒ�
	void SetTrans(string str, bool trans);
	// �摜��`�悷�邩�w��
	void DrawSetActive(string str,bool active);
	// �O���t�B�b�N�n���h���̕`��
	void DrawGraphHandle();
	// �G�t�F�N�g������
	void DeleteEffect(int i);
	// �G�t�F�N�g���~�߂�
	void StopEffect(int i);
	// ���������ŕ`��
	// �ǂ̈ʒu���璆���������邩�@Y���W�@
	// �o�͂��������� �t�H���g�n���h���F
	void DrawCenterString(int x_, int y_,string str_, unsigned int color_,int font_handle_);
	// ���������Ńt�H�[�}�b�g�w��`��
	// �ǂ̈ʒu���璆���������邩�@Y���W�@
    // �F �t�H���g�n���h�� �o�͂��������� �ϐ�
	void DrawCenterFormatString(int x_,int y_, unsigned int color_,int font_handle_,string str_,int num);
	// �E�l�߂ŕ`��
	// Y���W�@�o�͂���������
    // �t�H���g�n���h�� �F
	void DrawRightString(int y_,string str_,unsigned int color_, int font_handle_);
	// �E�l�߂ăt�H�[�}�b�g�w��`��
	// Y���W�@�F �t�H���g�n���h��
	// �o�͂��������� �ϐ�
	void DrawRightFormatString(int y_, unsigned int color_, int font_handle_, string str_, int num);
	// �����̐擪��X���W���擾
	int GetStringPosX(int x_,string str_,int font_handle_);
	

private:
	std::vector<string>handle_path;	// �n���h���̃p�X
	std::vector<int>graph_handle;	// �摜�n���h��
	std::vector<int>v_x;			// x���W
	std::vector<int>v_y;			// y���W
	std::vector<float>v_size;		// �T�C�Y
	std::vector<bool>v_active;		// �`�悷�邩
	std::vector<bool>v_trans;		// �����x��L���ɂ��邩

	// effect�̃T�C�Y���k�߂�
	void EraseEffect();
};

