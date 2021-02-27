#pragma once

class RankingScene : public SceneBase
{
public:
	RankingScene();
	~RankingScene();

	void Update() override;
	void Draw() override;

private:

	// �t�@�C���ǂݍ��ݗp
	fstream file;

	std::vector<int>ranking;		// �����L���O
	std::vector<int>score;			// �X�R�A
	std::vector<int>defeat_count;	// ���j��
	std::vector<int>hp;				// HP

	int flush;				// �t���b�V��
	int select_type;		// �I�����Ă�����
	int letter_type;		// �����̎��

	// �Z���N�g��ς���
	void ChangeSelect();
	// ����
	void Decision();
	// �����̃J�[�\����`��
	void LineCursor(int select_type_);

	// �����L���O�̕`��
	void DrawRanking();
	// �ڍׂ̕`��
	void DrawRankingDetails();
	// �ڍׂ̐ݒ�
	void SetRankingDetails();
	// ranking�t�@�C���ǂݍ���
	void RankingFileRead();

	enum RankingLetter {
		RANKING1 = 0,
		RANKING2,
		RANKING3,

		RANKING_MAX,
	};
};

