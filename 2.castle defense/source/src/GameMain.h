#pragma once


//---------------------------------------------
// GameMain�N���X  �p��SceneBase
//---------------------------------------------
class GameMain : public SceneBase
{
public:
	GameMain();
	~GameMain();

	void Update() override;
	void Draw() override;

private:
	
	int skydome;	 // �X�J�C�h�[��
	int play_timer;	 // �v���C����
	int old_time;	 // �O�̎��Ԃ�ۑ�
	int count_time;	 // �X�^�[�g���Ă���̌o�ߎ���
	int start_time;  // �X�^�[�g����
	int result_timer; // �N���A���Ɏg���^�C�}�[

	bool clear;		 // �Q�[���N���A�p�̃t���O
	bool fail;		 // �Q�[���I�[�o�[�p�̃t���O
	bool result;	 // �N���A�����s��
	int score;		 // �X�R�A
	int defeat_count;// ���j�J�E���g

	bool pause;		// �|�[�Y

	MapManager* map_m;			// �}�b�v�}�l�W���[
	EnemyManager* enemy_m;		// �G�l�~�[�}�l�W���[
	PlayerManager* player_m;	// �v���C���[�}�l�[�W���[

	// �t�@�C���ǂݍ��ݗp
	fstream file;
	
	//=======================================
	//�@Debug�p
	//=======================================
	bool isUpdata;						// �X�V���������Ă��邩
	bool isOneUpdate;					// �X�V��������x�s��
	// Debug�̍X�V����
	void DebugUpdata();	

	//=======================================
	// �����蔻��
	//======================================= 
	// �����蔻��܂Ƃ�
	void CheckHit();											
	// �v���C���[�ƃG�l�~�[�͈̔͂̓����蔻��
	void CheckHitPlayer_EnemyRange(Player* p, EnemyBase* e);
	// �v���C���[�ƃG�l�~�[�̍U���͈͂̓����蔻��
	void CheckHitPlayer_EnemyAttackRange(Player* p, EnemyBase* e);
	// �v���C���[�ƃG�l�~�[�̍U���̓����蔻��
	void CheckHitPlayer_EnemyAttack(Player* p, EnemyBase* e);
	// ���ƃG�l�~�[�̂�����͈�
	void CheckHitSword_Enemy(Player* p);
	// ����U���ƃG�l�~�[�̓����蔻��
	void CheckHitSpacial_Enemy(Player* p);
	// �G�l�~�[�̉���U���Ə�̓����蔻��
	void CheckHitEnemyAttack_Castle(EnemyBase*e, Castle* c);
	// �}�b�v�̓����蔻��
	void CheckHitMap(Castle* m, CollisionBase* c);

	//=======================================
	// �֐�
	//=======================================
	// �Q�[���N���A�`�F�b�N
	void CheckGameClear();
	// �Q�[���I�[�o�[�`�F�b�N
	void CheckGameOver();
	// �Q�[���N���A
	void GameClear();
	// �Q�[���I�[�o�[
	void GameOver();
	// �c��HP���`�F�b�N
	void HPCheck(CollisionBase* c);
	// Castle��HP�Q�[�W�̕`��
	void DrawCastleHPGauge(Castle* m);

	//=======================================
	// �t�@�C���֌W
	//=======================================
	// pos1�t�@�C���ǂݍ���
	void Pos1FileRead();
	// pos2�t�@�C���ǂݍ���
	void Pos2FileRead();
	// radius�t�@�C���ǂݍ���
	void RadiusFileRead();
	// result�t�@�C����������
	void ResultFileWrite();

	Vector3 pos;
	Vector3 rot;
	Vector3 scale;

	Vector3 enemy_pos;

	std::vector<Vector3>pos1;
	std::vector<Vector3>pos2;
	std::vector<float>radius;


	enum FontType {
		FONT_HP = 0,
		FONT_TIME,
		FONT_GUIDE,
		FONT_CLEAR,
		FONT_PAUSE,
		

		FONT_MAX,
	};

};
