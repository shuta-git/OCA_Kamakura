//---------------------------------------------
// �萔�̐錾
//---------------------------------------------
const float MAX_RANGE_RADIUS = 20.0f;	// �ő�͈�
const float DEF_ATTACK_RANGE = 8.0f;	// �f�t�H���g�U���͈�
const float DIVE_ATTACK_RANGE = 12.0f;	// �_�C�u�U���͈�
const float FRONT_ATTACK_RANGE = 1.5f;	// ����U���͈�
const float MAX_SPEED = 0.1f;			// �ő�ړ����x
const float DEF_RADIUS = 1.5f;			// �f�t�H���g���a
const int DAMAGE = 23;					// �_���[�W��
const int SCORE = 500;					// �X�R�A
const int MAX_HP = 2;					// HP

//---------------------------------------------
// �R���X�g���N�^
//---------------------------------------------
Enemy2::Enemy2( Vector3 pos_, Vector3 rot_, Vector3 scale_)
{
	pos = pos_;
	pos.y = 0.0f;
	rot = rot_;
	scale = {0.01f,0.01f,0.01f};
	radius = DEF_RADIUS;

	range_radius = MAX_RANGE_RADIUS;
	attack_range_radius = DEF_ATTACK_RANGE;
	speed = MAX_SPEED;
	damage = DAMAGE;
	score = SCORE;
	hp = MAX_HP;

	SetModelHandle("idle.mv1");
	SetModelHandle("walk.mv1");
	SetModelHandle("attack_front.mv1");
	SetModelHandle("attack_divebomb.mv1");
	SetModelHandle("attack_fireball.mv1");
	SetModelHandle("intro.mv1");
	SetModelHandle("death.mv1");

	SetTextureHandle("gargoyle_low_gargoyle_body_mat_BaseColor.png");

	type = INTRO;

	ApplyTexture();
	SetAnimNum(0);		// �A�j���[�V�����ԍ��w��
	SetAnim();			// �A�j���[�V�����֌W�̐ݒ�
}
//---------------------------------------------
// �f�X�g���N�^
//---------------------------------------------
Enemy2::~Enemy2()
{
}
//---------------------------------------------
// �X�V
//---------------------------------------------
void Enemy2::Update()
{
	old_pos = pos;
	old_type = type;

	// ���S��
	if (GetDeath()) {
		// ���S���[�V����
		type = DEATH;
		// �A�j���[�V�������x�̐ݒ�
		play_anim_speed = 1.0f;
		SetAnim();
		// �A�j���[�V�������X�V
		UpdateAnim();
		// �G�t�F�N�g�̏���
		for (int i = 0; i < effect.size(); i++) {
			// �G�t�F�N�g���~�߂�
			StopEffect(i);
			// �G�t�F�N�g�̏���
			DeleteEffect(i);
		}
		// ���[�V�������I��������
		if (type == DEATH && play_anim == 0.0f) {
			// �}�l�[�W���[�ɓ`���邽�߂Ƀt���O�𗧂Ă�
			death_anim = true;
		}
		return;
	}

	// ���G�t���O�������Ă�����
	if (GetInvincible()) {
		// ���G����
		Invincible();
	}


	// �C���g���ł͂Ȃ������U���͈͂ɓ����Ă�����
	if (type != INTRO && attack_range && !GetGoal()) {
		// �_�C�u�U�����[�V����
		type = ATTACK_DIVEBOMB;
		// �A�j���[�V�����֌W�̐ݒ�
		SetAnim();
		if (type != old_type) {
			effect.push_back(new Effect("Flame.efk", pos,true));
			effect[effect.size() - 1]->SetScale(Vector3(1.2f,1.2f,1.2f));
			effect[effect.size() - 1]->SetSpeed(1.2f);
		}
		
	}

	// �C���g���ƍU�����[�V�����ł͂Ȃ��Ƃ�
	// �ړ�����
	if (type != INTRO && type != ATTACK_DIVEBOMB && !GetGoal()) {
		if (range) {
			// player�Ɍ�����
			Approach(GetPlayerPos());
		}
		if (!range) {
			// ��Ɍ�����
			Approach(goal_pos);
		}
		// �S�[���`�F�b�N
		CheckGoal();
	}

	// �A�C�h��
	if (type == IDLE) {
		recast_time++;
		recast_time %= 90;
	}

	// ��ɒ�������
	if (GetGoal()) {
		if (recast_time == 0) {
			type = ATTACK_FRONT;
			// �A�j���[�V�����֌W�̐ݒ�
			SetAnim();
		}
	}

	// ����
	if (type == WALK) {
		play_anim_speed = 3.0f;
	}
	else {
		play_anim_speed = 1.0f;
	}

	// �A�j���[�V�����֌W�̐ݒ�
	SetAnim();
	// �A�j���[�V�������X�V
	UpdateAnim();

	//	�_�C�u�U��
	if (type == ATTACK_DIVEBOMB ) {
		// �U���͈͂̐ݒ�
		attack_range_radius = DIVE_ATTACK_RANGE;
		if (play_anim > 35.0f && play_anim < 100.0f) {
			invincible = true;
		}
		// �A�j���[�V������100.0f�𒴂�����
		// �����蔻��N��
		if (play_anim > 100.0f) {
			invincible = false;
			attack = true;
			
		}
		// �A�j���[�V������150.0f�𒴂�����
		// �����蔻��I��
		if (play_anim > 150.0f) {
			// �����蔻��I��
			attack = false;
			// �U���͈͂�������
			attack_range_radius = DEF_ATTACK_RANGE;
		}
		// �A�j���[�V�������I��������
		if (play_anim == 0.0f) {
			// �����A�j���[�V�����ɕύX
			type = WALK;
			// �A�j���[�V�����֌W�̐ݒ�
			SetAnim();
		}
	}

	// ����U��
	if (type == ATTACK_FRONT) {
		attack_range_radius = FRONT_ATTACK_RANGE;
		if ((int)play_anim == 70) {
			attack = true;
		}
		// �A�j���[�V�������I��������
		if (play_anim == 0.0f) {
			// �A�C�h��
			type = IDLE;
			// �U���͈͂�������
			attack_range_radius = DEF_ATTACK_RANGE;
			// �A�j���[�V�����֌W�̐ݒ�
			SetAnim();
		}
	}

	// �C���g���I��
	if (type == INTRO && play_anim == 0.0f) {
		type = WALK;
		// �A�j���[�V�����֌W�̐ݒ�
		SetAnim();
	}

	// �G�t�F�N�g�X�V
	for (int i = 0; i < effect.size(); i++) {
		effect[i]->Update();
		DeleteEffect(i);
	}
	
	
}
//---------------------------------------------
// �`��
//---------------------------------------------
void Enemy2::Draw()
{
	// ���f���`��
	DrawModel();

	// �G�t�F�N�g�`��
	for (int i = 0; i < effect.size(); i++) {
		effect[i]->Draw();
	}

#if(NDEBUG)
	
	DrawSphere3D(pos.GetV(), radius + attack_range_radius, 20, SKYBLUE, SKYBLUE, false);
	
	DrawSphere3D(pos.GetV(), radius + range_radius, 20, YELLOW, YELLOW, false);

	DrawCapsule3D(GetCapsuleUp(pos.GetV(), 5.0f), GetCapsuleDown(pos.GetV()), GetRadius(), 20, GREEN, GREEN, FALSE);
#endif
}
//---------------------------------------------
// ���f���n���h����ݒ�
//---------------------------------------------
void Enemy2::SetModelHandle(string str)
{
	string data = "data/enemys/";
	string path = data + str;
	model_type.push_back(resource::LoadModel(path.c_str()));
}
//---------------------------------------------
// �e�N�X�`���[��K�p
//---------------------------------------------
void Enemy2::ApplyTexture()
{
	for (int i = 0; i < texture_handle.size(); i++) {
		for (int j = 0; j < model_type.size();j++) {
			int num = MV1GetMaterialDifMapTexture(model_type[j], i);
			MV1SetTextureGraphHandle(model_type[j], num, texture_handle[i], FALSE);
		}
	}
	texture_handle.clear();
}
//---------------------------------------------
// �A�j���[�V������ݒ�
//---------------------------------------------
void Enemy2::SetAnim()
{
	if (type == old_type) { return; }
	play_anim = 0.0f;
	// ���܂ŃA�^�b�`���Ă����A�j���[�V�����̃f�^�b�`
	MV1DetachAnim(model_type[type], anim_num);
	// ���f���� anim_num �Ԗڂ̃A�j���[�V�������A�^�b�`����
	attach_anim = MV1AttachAnim(model_type[type], anim_num, model_type[type], TRUE);
	// �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
	anim_time = MV1GetAttachAnimTotalTime(model_type[type], attach_anim);
}
//---------------------------------------------
// ���f���̕`��
//---------------------------------------------
void Enemy2::DrawModel()
{
	// model�̃|�W�V������ݒ�
	MV1SetPosition(model_type[type], pos.GetV());
	// model�̌�����ݒ�
	MV1SetRotationXYZ(model_type[type], rot.GetV());
	// model�̃T�C�Y��ݒ�
	MV1SetScale(model_type[type], scale.GetV());
	// �A�j���[�V����
	MV1SetAttachAnimTime(model_type[type], attach_anim, play_anim);
	// npc.x���f���̕`��
	MV1DrawModel(model_type[type]);
}
