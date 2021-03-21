#pragma once

//---------------------------------------------
// EnemyBase�N���X �p��Base
//---------------------------------------------
class EnemyBase : public CollisionBase
{
public:
	EnemyBase();
	~EnemyBase();

	// ���S�A�j���[�V�����t���O���擾
	bool GetDeathAnim() { return death_anim; }
	// �ڕW�n�_�ɂ������̏���
	bool GetGoal();
	// �͈͗p�̔��a���擾
	float GetRangeRadius() const { return range_radius; }
	// �U���͈͂̔��a���擾
	float GetAttackRangeRadius() const { return attack_range_radius; }
	// �v���C���[���W�擾
	void SetPlayerPos(VECTOR player_pos_);

protected:

	// �v���C���[���W���擾
	VECTOR GetPlayerPos() { return player_pos; }
	// old�ϐ��ݒ�
	void SetOld();	
	// �߂Â�
	void Approach(VECTOR goal_);	
	// �S�[���`�F�b�N
	void CheckGoal();
	// ���f���n���h����ݒ�
	void SetModelHandle(string str);

	//=======================================
	// �A�j���[�V����
	//=======================================

	// �A�j���[�V�����̔ԍ���ݒ�
	void SetAnimNum(int num) { anim_num = num; } 
	// �A�j���[�V������ݒ�
	void SetAnim();			
	// �A�j���[�V�����̍X�V
	void UpdateAnim();							 

	//=======================================
	// �e�N�X�`���[
	//=======================================

	 // �e�N�X�`���[�̃p�X���擾
	void SetTextureHandle(string str);		
	// �e�N�X�`���[��K�p
	void ApplyTexture();									

	//=======================================
	// �ϐ�
	//=======================================
	int	   recast_time;			// ���L���X�g�^�C��
	float  range_radius;		// �͈͗p�̔��a
	float  attack_range_radius; // �U���͈͂̔��a

	VECTOR player_pos;
	VECTOR goal_pos;					// �ڕW�n�_�̍��W
	std::vector<int>texture_handle;		// �e�N�X�`���[�n���h��

	bool death_anim;	// ���S�A�j���[�V����
	bool goal;			// �S�[��


};

