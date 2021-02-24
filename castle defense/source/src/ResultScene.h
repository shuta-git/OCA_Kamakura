#pragma once

class ResultScene : public SceneBase
{
public:
	ResultScene();
	~ResultScene();

	void Update() override;
	void Draw() override;

private:

	// �t�@�C���ǂݍ��ݗp
	fstream file;

	bool result;		// �N���A�����s��
	int score;			// �X�R�A
	int defeat_count;	// ���j�J�E���g
	int hp;				// ��̎c��HP
	int total;			// ���v
	
	std::vector<int>ranking;		// �����L���O
	std::vector<int>score_v;		// �X�R�A
	std::vector<int>defeat_count_v;	// ���j��
	std::vector<int>hp_v;			// HP

	// result�t�@�C���ǂݍ���
	void ResultFileRead();
	// ranking�t�@�C���ǂݍ���
	void RankingFileRead();
	// ranking�t�@�C����������
	void RankingFileWrite();
	// �����L���O�̌v�Z
	void RankingCalculation();
	// �g�[�^���̌v�Z
	void TotalCalculation();
};

