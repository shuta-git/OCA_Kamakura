#pragma once

//---------------------------------------------
// Effect�N���X�@�p��Base
//---------------------------------------------
class Effect
{
public:
	Effect();
	Effect(string str, Vector3 pos_ , bool d);
	~Effect();

	// �X�V����
	void Update();

	
	// ���W�̐ݒ�
	void SetPos(Vector3 pos_);
	// �Đ����x�ɐݒ�
	void SetSpeed(float speed_);
	// �T�C�Y�̐ݒ�
	void SetScale(Vector3 scale_);
	// �����̐ݒ�
	void SetRot(Vector3 rot_);

	// �`��
	void Draw();				

	// �p�X���m�F
	bool CheckPath(string str);
	// �G�t�F�N�g���Đ������擾
	// 0�ōĐ���   -1�ōĐ�����Ă��Ȃ� : �Đ��I��
	int IsEffectPlaying(string str);
	//�@�Đ����̃G�t�F�N�g�̃p�X���擾
	int GetPlayingEffect() const { return playingEffectHandle; }
	// ���������擾
	bool GetD()const { return D; }

private:

	Vector3 pos;
	Vector3 rot;
	Vector3 scale;

	float speed;

	string save_path;			// �p�X��ۑ�

	bool D;


	int handle;					// �摜�n���h��
	int playingEffectHandle;	// �Đ����̃G�t�F�N�g�̃n���h��������������B

};

