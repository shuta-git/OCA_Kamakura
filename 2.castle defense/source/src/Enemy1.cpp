//---------------------------------------------
// �萔�̐錾
//---------------------------------------------
const float MAX_RANGE_RADIUS = 5.0f;	// �ő�͈�
const float ATTACK_RANGE_RADIUS = 2.0f;	// �U���͈�
const float MAX_SPEED = 0.1f;			// �ő�ړ����x
const int DAMAGE = 13;					// �_���[�W��
const int SCORE = 300;					// �X�R�A
const int MAX_HP = 2;					// HP

//---------------------------------------------
// �R���X�g���N�^
//---------------------------------------------
Enemy1::Enemy1(Vector3 pos_ ,Vector3 rot_ ,Vector3 scale_ )
{
	pos = pos_;
	rot = rot_;
	scale = scale_;
	radius = MAX_RANGE_RADIUS;
	range_radius = MAX_RANGE_RADIUS;
	speed = MAX_SPEED;
	damage = DAMAGE;
	score = SCORE;
	hp = MAX_HP;

	SetTextureHandle("haar_detail_NRM.jpg");
	SetTextureHandle("Spinnen_Bein_tex_COLOR_.jpg");
	SetTextureHandle("Spinnen_Bein_tex_2.jpg");
	SetTextureHandle("SH3.png");
	
	SetModelHandle("Spider_3.mv1");

	ApplyTexture();		// �e�N�X�`���[�K�p
	SetAnimNum(POSE);	// �A�j���[�V�����ԍ��w��
	SetAnim();			// �A�j���[�V�����֌W�̐ݒ�
}
//---------------------------------------------
// �f�X�g���N�^
//---------------------------------------------
Enemy1::~Enemy1()
{
	MV1DeleteModel(model);
}
//---------------------------------------------
// �X�V
//---------------------------------------------
void Enemy1::Update()
{
	SetOld();

	// ���S��
	if (GetDeath()) {
		SetAnimNum(DIE);
		play_anim_speed = 0.3f;
		// �A�j���[�V�����֌W�̐ݒ�
		SetAnim();
		// �A�j���[�V�������X�V
		UpdateAnim();
		if (anim_num == DIE && play_anim == 0.0f) {
			death_anim = true;
		}
		return;
	}

	// ���G�t���O�������Ă�����
	if (GetInvincible()) {
		// ���G����
		Invincible();
	}

	// �S�[���Ɍ�����
	if (!GetGoal()) {
		Approach(goal_pos);
		SetAnimNum(WAIK_VOR);
	}
	// �S�[���`�F�b�N
	CheckGoal();

	// �A�C�h��
	if (anim_num == POSE) {
		recast_time++;
		recast_time %= 90;
	}

	// �S�[��������
	if (GetGoal()) {
		if (recast_time == 0) {
			SetAnimNum(ATTACK);
		}
	}

	// �U�����[�V������
	if (anim_num == ATTACK) {
		// �A�j���[�V������13�܂ł�������
		if ((int)play_anim == 13) {
			attack = true;
		}
		// �A�j���[�V�������I��������
		if (play_anim == 0.0f) {
			// �A�C�h��
			SetAnimNum(POSE);
			pos.y = 0.0f;
		}
	}

	// �A�j���[�V�����֌W�̐ݒ�
	SetAnim();
	// �A�j���[�V�������X�V
	UpdateAnim();

}
//---------------------------------------------
// �`��
//---------------------------------------------
void Enemy1::Draw()
{
	// model�̃|�W�V������ݒ�
	MV1SetPosition(model, pos.GetV());
	// model�̌�����ݒ�
	MV1SetRotationXYZ(model, rot.GetV());
	// model�̃T�C�Y��ݒ�
	MV1SetScale(model , scale.GetV());
	// �A�j���[�V����
	MV1SetAttachAnimTime(model,attach_anim,play_anim);

	if (!GetInvincible()) {
		// npc.x���f���̕`��
		MV1DrawModel(model);
	}
	if (GetInvincible() && GetInvincibleTimer() % 10 != 0) {
		// npc.x���f���̕`��
		MV1DrawModel(model);
	}
	

#if(NDEBUG)
	//DrawSphere3D(pos.GetV(), radius +  attack_range_radius, 20, SKYBLUE, SKYBLUE, false);

	DrawSphere3D(pos.GetV(), radius, 20, BLUE, BLUE, false);
#endif
}
