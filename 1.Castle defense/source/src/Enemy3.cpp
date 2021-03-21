//---------------------------------------------
// �萔�̐錾
//---------------------------------------------
const float MAX_RANGE_RADIUS = 10.0f;	// �ő�͈�
const float MAX_SPEED = 0.1f;			// �ő�ړ����x
const int DAMAGE = 135;					// �_���[�W��

//---------------------------------------------
// �R���X�g���N�^
//---------------------------------------------
Enemy3::Enemy3( Vector3 pos_, Vector3 rot_, Vector3 scale_)
{
	pos = pos_;
	rot = rot_;
	scale = scale_;
	radius = 0.5f;
	range_radius = MAX_RANGE_RADIUS;
	speed = MAX_SPEED;
	damage = DAMAGE;
	SetModelHandle("npc.x");
}
//---------------------------------------------
// �f�X�g���N�^
//---------------------------------------------
Enemy3::~Enemy3()
{
	MV1DeleteModel(model);
}
//---------------------------------------------
// �X�V
//---------------------------------------------
void Enemy3::Update()
{
	old_pos = pos;
	if (range) {
		// player �Ɍ�����
		Approach(GetPlayerPos());
	}
	if (!range && !GetGoal()) {
		// ��Ɍ�����
		Approach(goal_pos);
	}

}
//---------------------------------------------
// �`��
//---------------------------------------------
void Enemy3::Draw()
{// model�̃|�W�V������ݒ�
	MV1SetPosition(model, pos.GetV());
	// model�̌�����ݒ�
	MV1SetRotationXYZ(model, rot.GetV());
	// model�̃T�C�Y��ݒ�
	MV1SetScale(model, scale.GetV());
	// noc.x���f���̕`��
	MV1DrawModel(model);

#if(NDEBUG)
	DrawSphere3D(pos.GetV(), radius, 20, YELLOW, YELLOW, false);
#endif
}
