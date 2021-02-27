#pragma once

//---------------------------------------------
// Player�N���X�@�p��Base
//---------------------------------------------
class Player : public CollisionBase
{
public:
	Player();
	~Player();

	// �X�V
	void Update() override;
	// �`��
	void Draw() override;				

	//=======================================
	// Get�֐�
	//=======================================
	// ���̐�[�̍��W���擾
	VECTOR GetSwordTipPos()const;
	// ���̎�����̍��W���擾
	VECTOR GetSwordRootPos()const;
	// ����U���̍��W
	VECTOR GetSpacialPos() const { return sp_pos.GetV(); }
	// �U���t���O���擾
	bool GetAttack() const { return attack; }	
	// ����U���t���O���擾
	bool GetSpacialAttack()const { return spacial_attack; }
	// ����U�����Ԃ̎擾
	int GetSpacialTimer() const { return sp_timer; }
	// ����U���̍ő厞�Ԃ̎擾
	int GetSpacialMaxTimer() const;
	
	//=======================================
	// Set�֐�
	//=======================================
	// ����U���t���O��false�ɂ���
	void SetFalseSpacialAttack() { spacial_attack = false; }
	// �J������]�p��ݒ�
	void SetCameraRot(Vector3 rot_);				
	// MP���v���X
	void AddMp(int num);
	// MP�����炷
	void SubMp(int num);

private:

	// �J�����̑S�̍s��
	MATRIX camera_mat;

	//=======================================
	// �ϐ�
	//=======================================
	int old_dir;		// ��O�̕���
	float old_rot_y;	// rot.y ��ۑ����Ă���
	int dir;			// �����p
	int attack_time;	// �U������
	float rot_speed;	// ��]���x
	int mp;				// ���@�����p
	int old_mp;			// mp�ۑ��p
	bool now_attack;	// �U������
	bool spacial_attack;// ����U���p
	int stamina;		// �X�^�~�i
	bool stamina_out;	// �X�^�~�i�؂�
	int sp_timer;		// ����U�����ԗp

	Vector3 sp_pos;					// ����U���p�̍��W
	Vector3 camera_rot;				// �J������]�p
	std::vector<int>texture_handle;	// �e�N�X�`���[�n���h��

	//  �|�C���g���C�g�p
	float Range, Atten0, Atten1, Atten2;

	//=======================================
	// �֐�
	//=======================================
	void Init();			// ������
	void SetMatRot();		// ��]�s��ɒl��ݒ�
	void Move();			// �ړ�
	void MoveRot();			// ��]
	void AttackAction();	// �U���A�N�V����

	// �J�����̉�]�͈͂̏���
	void CameraDirRotRange(float p_rot_range);	
	// �J�����̌����͈͂�ݒ�
	void DirSetRotRange();										
	// �����͈͂�ݒ�
	void SetRotRange(float d1,float d2,float d3,float d4);	
	// �}�C�i�X�ɉ�邩�v���X�ɉ�邩����		
	void SetRotRange(float d1, float d2, float d3, float d4,	
						float d5,float d6,float d7,float d8);	
	// ��]�X�s�[�h
	void RotSpeed(int dir_,int old_,float DIR);		
	// �Q�[�W
	void SetDrawGeuge(int size_x,int size_y,int x,int y, int type, int old_type , unsigned int color);
	// �U�����[�V�������Ԃ̍X�V
	void AttackAnimTimeUpdate(int attack_type,int finish_time);
	// �Q�[�W�̕`��
	void DrawGeuge();
	// �Q�[�W�̍X�V����
	void GeugeUpdate();

	// �|�C���g���C�g�𓮂����X�V����
	void PointLightUpdate();

	//=======================================
	// �e�N�X�`���[
	//=======================================
	// �e�N�X�`���[�̃p�X���擾
	void SetTextureHandle(string str);
	// �e�N�X�`���[��K�p
	void ApplyTexture();

	//=======================================
	// �񋓑�
	//=======================================

	// �����̎��
	enum Dir {								
		RIGHT = 0,
		LEFT,
		UP,
		DOWN,
		RIGHT_UP,
		RIGHT_DOWN,
		LEFT_UP,
		LEFT_DOWN,

		DIR_MAX,
	};

	enum Anim {
		ATTACK = 0,
		MAGIC,

		ANIM_MAX,
	};
};

