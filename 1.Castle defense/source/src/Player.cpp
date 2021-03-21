#include "Player.h"
//---------------------------------------------
// �萔�̐錾
//---------------------------------------------
const int PLAYER_GAUGE_X = WINDOW_W / 2;	// �v���C���[�̃Q�[�W�̍��WX�i�Q�[�W�̒��S�j
const int PLAYER_GAUGE_Y = WINDOW_H - 100;	// �v���C���[�̃Q�[�W�̍��WY�i�Q�[�W�̒��S�j
const int PLAYER_GAUGE_W = 200;				// �v���C���[�̃Q�[�W�̉���
const int PLAYER_GAUGE_H = 13;				// �v���C���[�̃Q�[�W�̏c��
const int PLAYER_MPGAUGE_H = 7;				// �v���C���[��MP�Q�[�W�̏c��
const int PLAYER_STAMINA_H = 7;				// �v���C���[�̃X�^�~�i�̏c��
// �v���C���[�̃X�^�~�i�Q�[�W�̍��WY
const int PLAYER_STAMINA_Y = PLAYER_GAUGE_Y + PLAYER_GAUGE_H + PLAYER_STAMINA_H;
// �v���C���[��MP�Q�[�W�̍��WY
const int PLAYER_MPGAUGE_Y = PLAYER_STAMINA_Y + PLAYER_STAMINA_H + PLAYER_MPGAUGE_H;
const int BACK_GAUGE = 2;					//�@�ӂ�
const float DEC_RATE = 0.71f;				// ���������Ƃ��̌W��
const float P_ROT_RANGE = (DX_PI_F / 18);	// PLAYER�̉�]�͈�	
const float P_SPEED = 0.4f;					// PLAYER�̑��x
const float P_DASH__SPEED = 0.6f;			// PLAYER�̃_�b�V���X�s�[�h
const float P_ROT_SPEED = 18.0f;			// PLAYER�̉�]���x
const float P_ROT_PI_F = 90.0f;				// float �^�̊p�x
const int	MAX_HP = 100;					// �v���C���[�̍ő�HP
const int	MAX_MP = 100;					// MP�̍ő�		
const int   MAX_STAMINA = 100;				// �X�^�~�i�̍ő�
const int   STAMIN_RED_GAUGE = 40;			// �X�^�~�i���Ԃ��Ȃ�^�C�~���O
const int   SUB_MP = 40;					// MP�̏����
const int   SP_MAX_TIME = 130;				// ����U���ő厞��

//---------------------------------------------
// �R���X�g���N�^
//---------------------------------------------
Player::Player()
{
	MatrixInit();
	
	// �f�[�^�ǂݍ���
	model = resource::LoadModel("data/player/player2.mv1");
	SetTextureHandle("Texture/kami01_tekusutya.jpg");
	SetTextureHandle("Texture/kao_tekusutya1.jpg");
	SetTextureHandle("Texture/nunoyoroi.jpg");
	ApplyTexture();

	// �ϐ��̏�����
	Init();

	// �e�p�����[�^��������
	Range = 2000.0f;
	Atten0 = 0.0f;
	Atten1 = 0.0006f;
	Atten2 = 0.0f;
}
//---------------------------------------------
// �f�X�g���N�^
//---------------------------------------------
Player::~Player()
{
}
//---------------------------------------------
// ������
//---------------------------------------------
void Player::Init()
{
	scale = { 0.5f,0.5f,0.5f };

	old_dir = 0;
	dir = 0;
	attack_time = 0;
	hp = MAX_HP;
	mp = MAX_MP;
	stamina = MAX_STAMINA;
	old_mp = MAX_MP;
	old_hp = MAX_HP;

	sp_timer = 0;

	speed = P_SPEED;
	rot_speed = (DX_PI_F / 360) * P_ROT_SPEED;
	radius = 1.0f;

	attack = false;
	now_attack = false;
	stamina_out = false;

	sp_pos = pos;
	camera_rot = Vector3(0.0f, 0.0f, 0.0f);
}
//---------------------------------------------
// �X�V
//---------------------------------------------
void Player::Update()
{
	camera_mat = MGetIdent();
	// �s��̏�����
	MatrixInit();
	// ��]�s��ɒl��ݒ�
	SetMatRot();

	old_pos = pos;
	old_rot_y = rot.y;

	// ��]
	MoveRot();
	// �ړ�
	Move();
	
	// �v���C���[�ƃJ�����̉�]�l�̍����~�����𒴂���
	// �v���C���[�ɃJ�����̒l����
	if (rot.y - camera_rot.y > (DX_PI_F * 2) || rot.y - camera_rot.y < -(DX_PI_F * 2)) {
		rot.y = camera_rot.y;
	}
	// ���G�t���O�������Ă�����
	if (GetInvincible()) {
		// ���G����
		Invincible();
	}

	// �U���A�N�V����
	AttackAction();

	// �G�t�F�N�g�̏���
	for (int i = 0; i < effect.size();i++) {
		// ����U���̃G�t�F�N�g�̎�
		if (effect[i]->CheckPath("Tornade.efk")) {
			Vector3 scale_ = {2.0f,2.0f,2.0f};
			effect[i]->SetScale(scale_);
		}
		effect[i]->Update();
		DeleteEffect(i);
	}

	// �Q�[�W�̍X�V����
	GeugeUpdate();

	// �|�C���g���C�g�̒���
	//PointLightUpdate();
	// ���f���̏��Ƀ|�C���g���C�g��ݒ�
	ChangeLightTypePoint(
		VGet(pos.x, 1000.0f, pos.z - 30.0f),
		Range,
		Atten0,
		Atten1,
		Atten2);
}
//---------------------------------------------
// �`��
//---------------------------------------------
void Player::Draw()
{
	// model�̃|�W�V������ݒ�
	MV1SetPosition(model, pos.GetV());
	// model�̌�����ݒ�
	MV1SetRotationXYZ(model, rot.GetV());
	// model�̃T�C�Y��ݒ�
	MV1SetScale(model,scale.GetV());

	if (!GetInvincible()) {
		// player.x���f���̕`��
		MV1DrawModel(model);
	}
	if (GetInvincible() && GetInvincibleTimer() % 10 != 0) {
		// player.x���f���̕`��
		MV1DrawModel(model);
	}
	// �G�t�F�N�g�̕`��
	for (int i = 0; i < effect.size(); i++) {
		effect[i]->Draw();
	}
	
	// �Q�[�W�̕`��
	DrawGeuge();

	// �p�����[�^�̓��e����ʂɕ\��
	//SetFontSize(20);
	//DrawFormatString(0, 0, GetColor(255, 255, 255), "Key:D.C  Range  %f", Range);
	//DrawFormatString(0, 16, GetColor(255, 255, 255), "Key:F.V  Atten0 %f", Atten0);
	//DrawFormatString(0, 32, GetColor(255, 255, 255), "Key:G.B  Atten1 %f", Atten1);
	//DrawFormatString(0, 48, GetColor(255, 255, 255), "Key:H.N  Atten2 %f / 100.0f", Atten2 * 100.0f);

#if(NDEBUG)
	// player �{�̂̓����蔻��
	DrawCapsule3D(GetCapsuleUp(pos.GetV(),8.0f), GetCapsuleDown(pos.GetV()), GetRadius(), 20, RED, RED, FALSE);
	// �U���̂�����
	Vector3 sword_tip = pos;
	sword_tip.x += sinf(rot.y);
	sword_tip.z += cosf(rot.y);
	Vector3 sword_root = pos;
	sword_root.x += sin(rot.y) * 5.0f;
	sword_root.z += cos(rot.y) * 5.0f;
	DrawCapsule3D(GetCapsuleTip(sword_root.GetV()), GetCapsuleRoot(sword_tip.GetV()), GetRadius(), 20, RED, RED, FALSE);
	// ����U���͈�
	for (int i = 0; i < (int)effect.size(); i++) {
		if (effect[i]->IsEffectPlaying("Tornade.efk") == 0 && sp_timer < SP_MAX_TIME) {
			DrawSphere3D(sp_pos.GetV(), 8.0f, 20, SKYBLUE, SKYBLUE, FALSE);
		}
	}
#endif
}
//---------------------------------------------
// ���̐�[�̍��W���擾
//---------------------------------------------
VECTOR Player::GetSwordTipPos() const
{
	Vector3 sword_tip = pos;
	sword_tip.x += sinf(rot.y);
	sword_tip.z += cosf(rot.y);
	return sword_tip.GetV();
}
//---------------------------------------------
// ���̎�����̍��W���擾
//---------------------------------------------
VECTOR Player::GetSwordRootPos() const
{
	Vector3 sword_root = pos;
	sword_root.x += sin(rot.y) * 5.0f;
	sword_root.z += cos(rot.y) * 5.0f;
	return sword_root.GetV();
}
//---------------------------------------------
// �U���A�N�V����
//---------------------------------------------
void Player::AttackAction()
{
	// ���N���b�N ���� ���U�����Ă��Ȃ���
	if (now_attack == false) {
		if (IsMouseOn(MOUSE_INPUT_LEFT) || IsPadOn(PAD_Y)) {
			attack = true;
			now_attack = true;
			anim_num = ATTACK;
		}
	}

	for (int i = 0; i < (int)effect.size(); i++) {
		// �Đ�����Ă��鎞
		if (effect[i]->IsEffectPlaying("Tornade.efk") == 0) {
			sp_timer++;
		}
		// �Đ����I����Ă���
		if (sp_timer > SP_MAX_TIME){
			spacial_attack = false;
			sp_timer = 0;
		}
	}

	if (now_attack == false && mp >= SUB_MP && spacial_attack == false) {
		// �E�N���b�N ���� ���U�����Ă��Ȃ��� ���� mp��20�ȏ�̎�
		if (IsMouseOn(MOUSE_INPUT_RIGHT) || IsPadOn(PAD_B)) {
			sp_pos = pos;
			sp_pos.x += sinf(rot.y) * 10;
			sp_pos.z += cosf(rot.y) * 10;
			effect.push_back(new Effect("Tornade.efk", sp_pos, true));
			spacial_attack = true;
			now_attack = true;
			anim_num = MAGIC;
			SubMp(SUB_MP);
		}
	}

	// �U�����Ԃ̍X�V����
	AttackAnimTimeUpdate(ATTACK, 10);
	AttackAnimTimeUpdate(MAGIC, 30);
}
//---------------------------------------------
// ����U���̍ő厞�Ԃ̎擾
//---------------------------------------------
int Player::GetSpacialMaxTimer() const
{
	return SP_MAX_TIME;
}
//---------------------------------------------
// �J������]�p��ݒ�
//---------------------------------------------
void Player::SetCameraRot(Vector3 rot_)
{
	// �J�����̒l����
	camera_rot = rot_;
}
//---------------------------------------------
// MP���v���X
//---------------------------------------------
void Player::AddMp(int num)
{
	// ��O��mp��ۑ�
	old_mp = mp;
	// mp���w�肳�ꂽ���^�X
	mp+=num;
	// mp���ő�𒴂�����
	if (mp > MAX_MP) {
		// mp���ő�ɂ���
		mp = MAX_MP;
	}
}
//---------------------------------------------
// MP�����炷
//---------------------------------------------
void Player::SubMp(int num)
{
	// ��O��mp��ۑ�
	old_mp = mp;
	// mp���w�肳�ꂽ�����炷
	mp -= num;
	// mp��0�����������
	if (mp < 0) {
		//mp��0�ɂ���
		mp = 0;
	}
}
//---------------------------------------------
//  ��]�s��ɒl��ݒ�
//---------------------------------------------
void Player::SetMatRot()
{
	// �S�̍s��  * ��]�s��
	camera_mat = MMult(camera_mat, MGetRotY(camera_rot.y));
}
//---------------------------------------------
// �ړ�
//---------------------------------------------
void Player::Move()
{
	// �W���p
	float rate = 1.0f;

	if (CheckHitKey(KEY_INPUT_D)  || IsPadRepeat(PAD_RIGHT) || CheckHitKey(KEY_INPUT_A) || IsPadRepeat(PAD_LEFT)) {
		if (CheckHitKey(KEY_INPUT_W) || IsPadRepeat(PAD_UP) || CheckHitKey(KEY_INPUT_S) || IsPadRepeat(PAD_DOWN)) {
			rate = DEC_RATE;
		}
	}

	// �_�b�V��
	speed = P_SPEED;
	if (stamina_out == false) {
		if (CheckHitKey(KEY_INPUT_LSHIFT) || IsPadRepeat(PAD_R)){
			speed = P_DASH__SPEED;
			stamina -= 2;
		}
	}
	// �X�^�~�i��0�ȉ��ɂȂ�Ȃ��悤�ɂ���
	if (stamina <= 0) {
		stamina = 0;
		stamina_out = true;
	}

	// �ړ���
	float move = speed * rate;

	if (CheckHitKey(KEY_INPUT_D) || IsPadRepeat(PAD_RIGHT)) {
		pos.x += camera_mat.m[0][0] * move;
		pos.z += camera_mat.m[0][2] * move;
	}
	if (CheckHitKey(KEY_INPUT_A) || IsPadRepeat(PAD_LEFT)) {
		pos.x -= camera_mat.m[0][0] * move;
		pos.z -= camera_mat.m[0][2] * move;
	}
	if (CheckHitKey(KEY_INPUT_S) || IsPadRepeat(PAD_DOWN)) {
		pos.x -= camera_mat.m[2][0] * move;
		pos.z -= camera_mat.m[2][2] * move;
	}
	if (CheckHitKey(KEY_INPUT_W) || IsPadRepeat(PAD_UP)) {
		pos.x += camera_mat.m[2][0] * move;
		pos.z += camera_mat.m[2][2] * move;
	}
}
//---------------------------------------------
// ��]
//---------------------------------------------
void Player::MoveRot()
{
	// ��O�̌�����ۑ�
	old_dir = dir;

	// �E��
	if (CheckHitKey(KEY_INPUT_D) && CheckHitKey(KEY_INPUT_W) || IsPadRepeat(PAD_RIGHT) && IsPadRepeat(PAD_UP)) {
		dir = RIGHT_UP;
		CameraDirRotRange(DIR_RIGHT_UP);
	}
	// �E��
	else if (CheckHitKey(KEY_INPUT_D) && CheckHitKey(KEY_INPUT_S) || IsPadRepeat(PAD_RIGHT) && IsPadRepeat(PAD_DOWN)) {
		dir = RIGHT_DOWN;
		CameraDirRotRange(DIR_RIGHT_DOWN);
	}
	// ����
	else if (CheckHitKey(KEY_INPUT_A) && CheckHitKey(KEY_INPUT_W) || IsPadRepeat(PAD_LEFT) && IsPadRepeat(PAD_UP)) {
		dir = LEFT_UP;
		CameraDirRotRange(DIR_LEFT_UP);
	}
	// ����
	else if (CheckHitKey(KEY_INPUT_A) && CheckHitKey(KEY_INPUT_S) || IsPadRepeat(PAD_LEFT) && IsPadRepeat(PAD_DOWN)) {
		dir = LEFT_DOWN;
		CameraDirRotRange(DIR_LEFT_DOWN);
	}
	// �E
	else if (CheckHitKey(KEY_INPUT_D) || IsPadRepeat(PAD_RIGHT)) {
		RotSpeed(RIGHT, LEFT, DIR_RIGHT);
	}
	// ��
	else if (CheckHitKey(KEY_INPUT_A) || IsPadRepeat(PAD_LEFT)) {
		RotSpeed(LEFT, RIGHT, DIR_LEFT);
	}
	// ��
	else if (CheckHitKey(KEY_INPUT_W) || IsPadRepeat(PAD_UP)) {
		RotSpeed(UP, DOWN, DIR_UP1);
	}
	// ��
	else if (CheckHitKey(KEY_INPUT_S) || IsPadRepeat(PAD_DOWN)) {
		RotSpeed(DOWN, UP, DIR_DOWN);
	}

	// ������
	rot_speed = (DX_PI_F / 360) * P_ROT_SPEED;
}
//---------------------------------------------
// ��]�X�s�[�h
//---------------------------------------------
void Player::RotSpeed(int dir_, int old_, float DIR)
{
	dir = dir_;
	if (old_dir == old_) {
		rot_speed = (DX_PI_F / 360) * P_ROT_PI_F;
	}
	CameraDirRotRange(DIR);
}
//---------------------------------------------
// �J�����̉�]�͈͂̏���
// ������camera.y��rot.y�̍���������
//---------------------------------------------
void Player::CameraDirRotRange(float p_rot_range)
{
	// ����������

	float range = 0.0f;

	if (p_rot_range > DX_PI_F || rot.y < -DX_PI_F || camera_rot.y >(DX_PI_F / 2)) {
		range = -((DX_PI_F * 2) - p_rot_range);

		if (camera_rot.y - P_ROT_RANGE + range < rot.y &&
			camera_rot.y + P_ROT_RANGE + range > rot.y) {
			return;
		}

	}

	if (camera_rot.y - P_ROT_RANGE + p_rot_range < rot.y &&
		camera_rot.y + P_ROT_RANGE + p_rot_range > rot.y ) 
	{
		return;
	}

	DirSetRotRange();
}
//---------------------------------------------
// �J�����̌����͈͂�ݒ� 
//---------------------------------------------
void Player::DirSetRotRange()
{
	switch (dir)
	{
	case RIGHT:
		SetRotRange(DIR_RIGHT, DIR_LEFT, DIR_LEFT, DIR_RIGHT);
		break;
	case LEFT:
		SetRotRange(DIR_UP1, DIR_RIGHT, DIR_LEFT, DIR_UP2, DIR_RIGHT, DIR_UP1, DIR_UP2, DIR_LEFT);
		break;
	case UP:
		SetRotRange(DIR_UP1, DIR_DOWN, DIR_UP2, DIR_DOWN);
		break;
	case DOWN:
		SetRotRange(DIR_DOWN, DIR_UP2, DIR_DOWN, DIR_UP1);
		break;
	case RIGHT_UP:
		SetRotRange(DIR_RIGHT_UP, DIR_LEFT_DOWN, DIR_LEFT_UP, DIR_RIGHT_DOWN);
		break;
	case RIGHT_DOWN:
		SetRotRange(DIR_RIGHT_DOWN, DIR_LEFT_UP, DIR_LEFT_DOWN, DIR_RIGHT_UP);
		break;
	case LEFT_UP:
		SetRotRange(DIR_UP1, DIR_RIGHT_DOWN, DIR_LEFT_UP, DIR_UP2, DIR_RIGHT_UP, DIR_UP1, DIR_UP2, DIR_LEFT_DOWN);
		break;
	case LEFT_DOWN:
		SetRotRange(DIR_UP1, DIR_RIGHT_UP, DIR_LEFT_DOWN, DIR_UP2, DIR_RIGHT_DOWN, DIR_UP1, DIR_UP2, DIR_LEFT_UP);
		break;
	default:
		break;
	}
}
//---------------------------------------------
// �����͈̔͂�ݒ�
// �}�C�i�X�ɉ�邩�v���X�ɉ�邩����
//---------------------------------------------
void Player::SetRotRange(float d1, float d2, float d3, float d4)
{
	if (rot.y > d1 + camera_rot.y &&
		rot.y <  d2 + camera_rot.y ||
		rot.y > -d3 + camera_rot.y &&
		rot.y < -d4 + camera_rot.y)
	{
		rot.y -= rot_speed;
	}
	else {
		rot.y += rot_speed;
	}
}
//---------------------------------------------
// �����͈̔͂�ݒ�
// �}�C�i�X�ɉ�邩�v���X�ɉ�邩����
//---------------------------------------------
void Player::SetRotRange(float d1, float d2, float d3, float d4, float d5, float d6, float d7, float d8)
{
	if (rot.y >= d1 + camera_rot.y &&
		rot.y <  d2 + camera_rot.y ||
		rot.y >  d3 + camera_rot.y &&
		rot.y <  d4 + camera_rot.y ||

		rot.y >  -d5 + camera_rot.y &&
		rot.y <= d6 + camera_rot.y ||
		rot.y > -d7 + camera_rot.y &&
		rot.y < -d8 + camera_rot.y)
	{
		rot.y -= rot_speed;
	}
	else {
		rot.y += rot_speed;
	}
}
//---------------------------------------------
// �e�N�X�`���[�̃p�X���擾
//---------------------------------------------
void Player::SetTextureHandle(string str)
{
	string data = "data/player/";
	string path = data + str;
	texture_handle.push_back(resource::LoadGraph(path.c_str()));
}
//---------------------------------------------
// �e�N�X�`���[��K�p
//---------------------------------------------
void Player::ApplyTexture()
{
	for (int i = 0; i < texture_handle.size(); i++) {
		int num = MV1GetMaterialDifMapTexture(model, i);
		MV1SetTextureGraphHandle(model, num, texture_handle[i], FALSE);
	}

	texture_handle.clear();
}
//---------------------------------------------
// �|�C���g���C�g����
//---------------------------------------------
void Player::PointLightUpdate()
{
	// �c�b�L�[�Ń��C�g�̉e���͈͂�ύX
	if (CheckHitKey(KEY_INPUT_D) == 1)
	{
		Range += 20.0f;
	}
	if (CheckHitKey(KEY_INPUT_C) == 1)
	{
		Range -= 20.0f;
	}

	// �e�u�L�[�Ń��C�g�̋��������p�����[�^�O�̒l��ύX
	if (CheckHitKey(KEY_INPUT_F) == 1)
	{
		Atten0 += 0.001f;
	}
	if (CheckHitKey(KEY_INPUT_V) == 1)
	{
		Atten0 -= 0.001f;
	}

	// �f�a�L�[�Ń��C�g�̋��������p�����[�^�P�̒l��ύX
	if (CheckHitKey(KEY_INPUT_G) == 1)
	{
		Atten1 += 0.00001f;
	}
	if (CheckHitKey(KEY_INPUT_B) == 1)
	{
		Atten1 -= 0.00001f;
	}

	// �g�m�L�[�Ń��C�g�̋��������p�����[�^�Q�̒l��ύX
	if (CheckHitKey(KEY_INPUT_H) == 1)
	{
		Atten2 += 0.0000001f;
	}
	if (CheckHitKey(KEY_INPUT_N) == 1)
	{
		Atten2 -= 0.0000001f;
	}

	// �e�������̒l��␳
	if (Range < 0.0f) Range = 0.0f;

	// ���������p�����[�^�̒l��␳
	if (Atten0 < 0.0f) Atten0 = 0.0f;
	if (Atten1 < 0.0f) Atten1 = 0.0f;
	if (Atten2 < 0.0f) Atten2 = 0.0f;
}
//---------------------------------------------
// �Q�[�W�̒l��ݒ肵�ĕ`��
// �����@�c���@���WX�@���WY�@
// �Ȃ�̃Q�[�W���@���̃Q�[�W�̈�O�̒l �F
//---------------------------------------------
void Player::SetDrawGeuge(int size_x, int size_y, int x, int y,int type, int old_type, unsigned int color)
{
	// �O�g
	DrawBox(x - BACK_GAUGE, y - BACK_GAUGE, x + size_x * 2 + BACK_GAUGE, y + size_y * 2 + BACK_GAUGE, GetColor(50, 50, 50), TRUE);
	// �D�F�Q�[�W
	DrawBox(x , y , x + size_x * 2 , y + size_y * 2 , GetColor(90, 90, 90), TRUE);
	// ��O�̃Q�[�W
	DrawBox(x, y, x + (old_type *  size_x * 2 / MAX_MP), y + size_y * 2, GRAY, TRUE);
	// �Q�[�W
	DrawBox(x, y,x + (type *  size_x * 2 / MAX_MP), y + size_y * 2,color, TRUE);
}
//---------------------------------------------
// �U�����[�V�������Ԃ̍X�V
// �U���̎�ށ@�I������
//---------------------------------------------
void Player::AttackAnimTimeUpdate(int attack_type, int finish_time)
{
	// �A�j���[�V�������U���̎�
	if (anim_num == attack_type) {
		// ���U�����Ă�����
		if (now_attack) {
			// ���Ԃ��X�V
			attack_time++;
		}
		// �U�����Ԃ��I��������
		if (attack_time == finish_time) {
			// �U�����I��
			now_attack = false;
			// �U�����Ԃ�������
			attack_time = 0;
		}
	}
}
//---------------------------------------------
// �Q�[�W�̕`��
//---------------------------------------------
void Player::DrawGeuge()
{
	// HP�Q�[�W
	int life_size_x = PLAYER_GAUGE_W;
	int life_size_y = PLAYER_GAUGE_H;
	int life_x = PLAYER_GAUGE_X - life_size_x;
	int life_y = PLAYER_GAUGE_Y - life_size_y;
	// �X�^�~�i�Q�[�W
	int st_size_x = PLAYER_GAUGE_W;
	int st_size_y = PLAYER_STAMINA_H;
	int st_x = PLAYER_GAUGE_X - st_size_x;
	int st_y = PLAYER_STAMINA_Y - st_size_y;
	unsigned int st_color = 0;
	if (stamina > STAMIN_RED_GAUGE) {
		st_color = GetColor(253, 126, 0);
	}
	else {
		st_color = GetColor(253, 0, 0);
	}
	// MP�Q�[�W
	int mp_size_x = PLAYER_GAUGE_W;
	int mp_size_y = PLAYER_MPGAUGE_H;
	int mp_x = PLAYER_GAUGE_X - mp_size_x;
	int mp_y = PLAYER_MPGAUGE_Y - mp_size_y;
	// HP�Q�[�W
	SetDrawGeuge(life_size_x, life_size_y, life_x, life_y, hp, old_hp, WHITE);
	// MP�Q�[�W
	SetDrawGeuge(mp_size_x, mp_size_y, mp_x, mp_y, mp, old_mp, SKYBLUE);
	// �X�^�~�i�Q�[�W
	SetDrawGeuge(st_size_x, st_size_y, st_x, st_y, stamina, 0, st_color);

	// MP�Q�[�W�̊i�q
	int lattice_size_x = PLAYER_GAUGE_W / 5;
	int lattice_size_y = PLAYER_MPGAUGE_H;
	int lattice_x[5];
	int lattice_y = mp_y;
	for (int i = 0; i < 5; i++) {
		lattice_x[i] = mp_x + (lattice_size_x * i * 2);
	}
	// MP�Q�[�W�̊i�q
	for (int i = 0; i < 5; i++) {
		DrawBox(lattice_x[i] - 1, lattice_y - 1, lattice_x[i] + lattice_size_x * 2 + 1, lattice_y + lattice_size_y * 2 + 1, GetColor(50, 50, 50), FALSE);
	}
}
//---------------------------------------------
// �Q�[�W�̍X�V����
//---------------------------------------------
void Player::GeugeUpdate()
{
	// old_hp�̏���
	if (old_hp != hp) {
		old_hp--;
	}
	// old_mp�̏���
	if (old_mp > mp) {
		old_mp--;
	}
	if (old_mp < mp) {
		old_mp++;
	}

	// �X�^�~�i�؂�̎�
	if (stamina_out) {
		// �X�^�~�i�����^���łȂ����
		if (stamina != MAX_STAMINA) {
			// �X�^�~�i����
			stamina++;
		}
		if (stamina > STAMIN_RED_GAUGE) {
			stamina_out = false;
		}
	}
	else {
		// �_�b�V�����ł͂Ȃ��Ƃ�
		if (!CheckHitKey(KEY_INPUT_LSHIFT) && !IsPadRepeat(PAD_R)) {
			// �X�^�~�i�����^���łȂ����
			if (stamina != MAX_STAMINA) {
				// �X�^�~�i����
				stamina++;
			}
		}
	}
}
