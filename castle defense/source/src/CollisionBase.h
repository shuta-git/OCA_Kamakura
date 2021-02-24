#pragma once

//---------------------------------------------
// CollisionBase�N���X�@�p�� Base
//---------------------------------------------
class CollisionBase : public Base
{
public:
	CollisionBase();
	~CollisionBase();

	//=======================================
	// Get�֐�
	//=======================================
	// ���f���n���h�����擾
	int GetModel() const { return model; }
	// �_���[�W���擾
	int GetDamage() const { return damage; }
	// HP���擾
	int GetHp()const { return hp; }
	// ��O��HP���擾
	int GetOldHp()const { return old_hp; }
	// �X�R�A���擾
	int GetScore()const { return score; }
	// ���a���擾
	float GetRadius() const { return radius; }
	// ���񂾂��ǂ���
	bool GetDeath()const { return death; }					
	// �͈͂ɓ����Ă��邩
	bool GetRange()const { return range; }
	// �U���͈͂ɓ����Ă��邩�ǂ���
	bool GetAttackRange()const { return attack_range; }
	// ���G���ǂ���
	bool GetInvincible()const { return invincible;}
	// �U���t���O���擾
	bool GetAttack() const { return attack; }

	//=======================================
	// Set�֐�
	//=======================================
	// death �t���O�𗧂Ă�
	void SetIsHitDeath() { death = true; }
	// range �t���O�𗧂Ă�
	void SetIsHitRange() { range = true; }
	// attack_range�t���O�𗧂Ă�
	void SetIs_HitAttackRange() { attack_range = true; }
	// invincible �t���O�𗧂Ă�(���G)
	void SetInvincible() { invincible = true; }	
	// range �� false�ɂ���
	void SetFalseRange() { range = false; }
	// attack_range �� false �ɂ���
	void SetFalseAttackRange() { attack_range = false; }
	// HP �̏���
	void HpProcessing(int hp_);
	// ���W�ɑ��
	void SetPos(Vector3 v) { pos = v; }
	// attack �t���O�� false�ɂ���
	void SetFalseAttack() { attack = false; }
	// HP��0�ɂ���
	void SetHPInit() { hp = 0; }

protected:
	//=======================================
	// �ϐ�
	//=======================================
	float  length;			// ����
	float  anim_time;		// �A�j���[�V�����̎���
	float  play_anim;		// �A�j���[�V�����̍Đ�
	float  play_anim_speed; // �A�j���[�V�������x

	int    hp;				// HP
	int    old_hp;			// ��O��HP
	int	   damage;			// �_���[�W
	int	   old_anim_num;	// �O�̃A�j���[�V�����ԍ�
	int	   anim_num;		// �A�j���[�V�����ԍ�
	int    attach_anim;		// �A�b�^�b�`�����A�j���f�[�^
	int	   model;			// ���f���n���h��
	int	   score;			// �X�R�A

	bool   death;			// ���S�t���O
	bool   range;			// �͈̓t���O
	bool   attack_range;	// �U���͈̓t���O
	bool   invincible;		// ���G�t���O
	bool   attack;			// �U���p�̃t���O

	//=======================================
	// Get�֐�
	//=======================================
	// ���G���Ԃ��擾
	int GetInvincibleTimer()const { return invincible_timer; }	
	// ���G����
	void Invincible();											

private:
	//=======================================
	// �萔
	//=======================================
	const int MAX_INVUNCIBLE_TIME = 60;		// �ő喳�G����(60)
	int invincible_timer;					// ���G���Ԃ��͂���p

};

