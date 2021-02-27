#pragma once

class GameMain;
//---------------------------------------------
// EnemyManager�N���X
//---------------------------------------------
class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();

	void Update(int time);
	void Draw();

	//=======================================
	// Get�֐�
	//=======================================

	// EnemyBase�^�Ŏ擾
	EnemyBase* GetEnemyBase(int i)const;

	// �G�l�~�[�̃T�C�Y���擾
	size_t GetSize() const;							 

private:
	//=======================================
	// �֐�
	//=======================================
	// �G�l�~�[�̃f���[�g����
	void DeleteEnemy(int i);	
	// �G�l�~�[�̃C���[�X����
	void EraseEnemy();
	// �G�l�~�[�̃X�|�[���ݒ�
	void SetSpawn(int time);
	// �G�l�~�[�̃X�|�[��
	void Spawn();

	std::vector<EnemyBase*>enemies;	// �G�l�~�[

	int spawn_timer;		// �������Ԃ��͂���
	int num;				// �G�l�~�[�̔ԍ�
	int enemy_count;		// �G�l�~�[�̐��𐔂���

	Vector3 pos;
	Vector3 rot;
	Vector3 scale;

	VECTOR spawn_pos;

	// �G�l�~�[�̎��
	enum Type
	{
		ENEMY1 = 0,
		ENEMY2,
		ENEMY3,

		ENEMY_MAX,
	};

};

