#include "GameMain.h"
//---------------------------------------------
// �萔�̐錾
//---------------------------------------------
const int CASTLE_LIFEGAUGE_X = WINDOW_W / 2;	// ��̃��C�t�Q�[�W�̍��WX�i���C�t�Q�[�W�ɒ��S�j
const int CASTLE_LIFEGAUGE_Y = 80;				// ��̃��C�t�Q�[�W�̍��WY�i���C�t�Q�[�W�ɒ��S�j
const int CASTLE_LIFEGAUGE_W = 225;				// ��̃��[�t�Q�[�W�̉���
const int CASTLE_LIFEGAUGE_H = 30;				// ��̃��C�t�Q�[�W�̏c��
const int MAX_TIME = 150;						// �ő厞�� 150
const int SCORE = 100;							// �X�R�A
const int CLEAR_MAX_TIME = 300;					// �N���A���̍ő厞��
const int FAIL_MAX_TIME = 60;					// �Q�[���I�[�o�[���̍ő厞��
const int ADD_MP = 3;							// MP�񕜗�

//---------------------------------------------
// �R���X�g���N�^
//---------------------------------------------
GameMain::GameMain()
{
	// �t�H�O��L���ɂ���
	SetFogEnable(TRUE);
	// �t�H�O�̐F�𔒐F�ɂ���
	SetFogColor(255, 255, 255);
	// �t�H�O�̊J�n�������O�A�I��������130�ɂ���
	SetFogStartEnd(0.0f, 130.0f);

	// ���C�t�Q�[�W�̓ǂݍ���
	SetGraphHandle("UI/LifeGauge.png", CASTLE_LIFEGAUGE_X, CASTLE_LIFEGAUGE_Y, 0.4f);
	SetGraphHandle("UI/pause.png", WINDOW_W - 70,60,0.07f);
	// �X�J�C�h�[��
	skydome = resource::LoadModel("data/scene/skydome/dome321.X");

	if (map_m == nullptr)	 { map_m = new MapManager(); }
	if (player_m == nullptr) { player_m = new PlayerManager(); }
	if (enemy_m == nullptr)  { enemy_m = new EnemyManager(); }

	isUpdata = true;
	isOneUpdate = false;
	pause = false;
	// �t�@�C���ǂݍ���
	Pos1FileRead();
	Pos2FileRead();
	RadiusFileRead();

	scene_type = GAME_MAIN;
	enemy_pos = { 0.0f,0.0f,0.0f };
	
	play_timer = MAX_TIME;
	start_time = GetNowCount();
	old_time = start_time;
	count_time = 0;
	defeat_count = 0;
	result = false;

	clear = false;

	// HP�̕���
	font_handle.push_back(CreateFontToHandle(NULL, 30, -1));
	// �^�C���̕���
	font_handle.push_back(CreateFontToHandle(NULL, 50, -1));
	// �K�C�h�̕���
	font_handle.push_back(CreateFontToHandle(NULL, 40, -1));
	// �N���A����
	font_handle.push_back(CreateFontToHandle(NULL, 90, -1));
	// �|�[�Y
	font_handle.push_back(CreateFontToHandle(NULL, 20, -1));
}
//---------------------------------------------
// �f�X�g���N�^
//---------------------------------------------
GameMain::~GameMain()
{
	if (map_m != nullptr) {
		delete map_m;
		map_m = nullptr;
	}
	if (player_m != nullptr) { 
		delete player_m;
		player_m = nullptr;
	}
	if (enemy_m != nullptr) {
		delete enemy_m;
		enemy_m = nullptr;
	}
	pos1.clear();
	pos2.clear();
	radius.clear();
}
//---------------------------------------------
//		�X�V����
//---------------------------------------------
void GameMain::Update()
{
#if(NDEBUG)
	// �f�o�b�O�̍X�V
	DebugUpdata();
#endif
	// �f�o�b�O�̍X�V
	DebugUpdata();

	if (IsKeyOn(KEY_INPUT_F) || IsPadOn(PAD_START)) {
		scene_type = PAUSE;
	}

	if (isUpdata ) {

		// �N���A������
		if (clear) {
			GameClear();
		}
		// �Q�[���I�[�o�[
		if (fail) {
			GameOver();
		}
		// �Q�[����
		if(clear == false && fail == false){
			// �O�̌o�ߎ��Ԃ�ۑ�
			old_time = count_time;
			// �o�ߎ��� = ���̃~���b���� - �X�^�[�g�������� / �b�ɒ���
			count_time = (GetNowCount() - start_time) / 1000;
			// �o�ߎ��Ԃ��Ⴄ�Ƃ�
			if (old_time != count_time) {
				// �^�C�}�[�����炷
				play_timer--;
				// �X�R�A�𑝂₷
				score += SCORE;
			}

			// �Q�[���I�[�o�[�`�F�b�N
			CheckGameOver();
			// �Q�[���N���A�`�F�b�N
			CheckGameClear();
		}
		
		// �X�V
		if (player_m != nullptr) {
			player_m->Update();
		}
		if (enemy_m != nullptr) {
			enemy_m->Update(play_timer);
		}
		if (map_m != nullptr) {
			map_m->Update();
		}

		// �����蔻��
		CheckHit();

		// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
		UpdateEffekseer3D();

		// isOneUdate��true�̎�
		if (isOneUpdate) {
			// �X�V���~�߂�
			isOneUpdate = false;
			isUpdata = false;
		}
	}

	// �G�t�F�N�g�̏���
	for (int i = 0; i < effect.size(); i++) {
		effect[i]->Update();
		DeleteEffect(i);
	}
}
//---------------------------------------------
//		�`�揈��
//---------------------------------------------
void GameMain::Draw()
{
	// �X�J�C�h�[��
	VECTOR a_ = { 0.0f,0.0f,0.0f };
	VECTOR b_ = { 0.3f,0.3f,0.3f };
	VECTOR c_ = { 1.0f,1.0f,1.0f };
	VECTOR d_ = { 3.0f,3.0f,3.0f };
	VECTOR e_ = { 5.0f,5.0f,5.0f };
	MV1SetScale(skydome, c_);
	MV1SetPosition(skydome, a_);
	MV1DrawModel(skydome);

	// �}�b�v�̕`��
	if (map_m != nullptr) {
		map_m->Draw();
	}
	// �G�l�~�[�̕`��
	if (enemy_m != nullptr) {
		enemy_m->Draw();
	}
	// �v���C���[�̕`��
	if (player_m != nullptr) {
		player_m->Draw();
	}
	// �G�t�F�N�g�̕`��
	for (int i = 0; i < effect.size(); i++) {
		effect[i]->Draw();
	}
	// ��̃��C�t�Q�[�W�̕`��
	if (map_m != nullptr) {
		DrawCastleHPGauge(map_m->GetCastle());
	}

	// �摜�̕`��
	DrawGraphHandle();

	// �N���A
	if (clear) {
		if (result_timer < 150) {
			DrawCenterString(WINDOW_W / 2, 400, "�h�q���� !!", RED, font_handle[FONT_CLEAR]);
		}
		else {
			DrawCenterString(WINDOW_W / 2, 400, "Congratulation!", YELLOW, font_handle[FONT_CLEAR]);
		}
		DrawStringToHandle(0, 0, "�c�莞�� : FINISH ", BLACK, font_handle[FONT_TIME]);
	}
	// �Q�[���I�[�o�[
	if (fail) {
		DrawCenterString(WINDOW_W / 2, 400, "�h�q���s !!",BLUE, font_handle[FONT_CLEAR]);
	}
	if(clear == false && fail == false)
	{
		// �o�ߎ��Ԃ̕\��
		DrawFormatStringToHandle(0, 0, BLACK, font_handle[FONT_TIME], "�c�莞�� : %d", play_timer);
		// �����̕`��
		DrawRightString(150, "�������Ԃ܂ŏ�����I", RED, font_handle[FONT_GUIDE]);
		// �|�[�Y�ɕ���
		DrawRightString(0,"START", BLACK,font_handle[FONT_PAUSE]);
	}

#if(NDEBUG)
	VECTOR a = {100.0f,0.0f,0.0f};
	VECTOR b = {-100.0f,0.0f,0.0f };
	DrawLine3D(a,b,RED);
	a = { 0.0f,100.0f,0.0f };
	b = { 0.0f,-100.0f,0.0f };
	DrawLine3D(a,b,GREEN);
	a = { 0.0f,0.0f,100.0f };
	b = { 0.0f,0.0f,-100.0f };
	DrawLine3D(a,b,BLUE);
	for (int i = 0; i < pos2.size();i++) {
		DrawLine3D(pos1[i].GetV(), pos2[i].GetV(), SKYBLUE);
	}
	for (int i = 0; i < radius.size(); i++) {
		DrawSphere3D(pos1[i + pos2.size()].GetV(),radius[i], 20, SKYBLUE, SKYBLUE,FALSE);
	}
#endif
}
//---------------------------------------------
// �f�o�b�O�̍X�V
//---------------------------------------------
void GameMain::DebugUpdata()
{
	// F1�������ꂽ��X�V��؂�ւ�
	if (IsKeyOn(KEY_INPUT_F1)) isUpdata = !isUpdata;
	// F2��������Ă��čX�V���~�܂��Ă�����
	if (IsKeyOn(KEY_INPUT_F2) && !isUpdata) {
		// �X�V�J�n
		isUpdata = true;
		isOneUpdate = true;
	}
}
//---------------------------------------------
// �����蔻����܂Ƃ߂�
//---------------------------------------------
void GameMain::CheckHit()
{
	// �ǂ��炩�� nullptr �̎� return
	if (enemy_m == nullptr || player_m == nullptr)return;

	// �v���C���[�ƃG�l�~�[�̓����蔻��
	for (int i = 0; i < enemy_m->GetSize(); i++) 
	{
		CheckHitPlayer_EnemyRange(player_m->GetPlayer(), enemy_m->GetEnemyBase(i));
		CheckHitPlayer_EnemyAttackRange(player_m->GetPlayer(), enemy_m->GetEnemyBase(i));
	}

	// ���ƃG�l�~�[
	CheckHitSword_Enemy(player_m->GetPlayer());
	// ����U���ƃG�l�~�[
	CheckHitSpacial_Enemy(player_m->GetPlayer());

	// nullptr �̎� return
	if (map_m == nullptr) return;
	for (int i = 0; i < enemy_m->GetSize(); i++) {
		CheckHitEnemyAttack_Castle(enemy_m->GetEnemyBase(i),map_m->GetCastle());
	}
	
	// �}�b�v�̓����蔻��
	CheckHitMap(map_m->GetCastle(),player_m->GetPlayer());
	for (int i = 0; i < enemy_m->GetSize(); i++)
	{
		CheckHitMap(map_m->GetCastle(), enemy_m->GetEnemyBase(i));
	}
}
//---------------------------------------------
// �v���C���[�ƃG�l�~�[�͈̔͂̓����蔻��
//---------------------------------------------
void GameMain::CheckHitPlayer_EnemyRange(Player * p, EnemyBase * e)
{
	// �G�l�~�[�͈̔͂ɓ����Ă��Ȃ��ꍇ���������Ȃ��܂��́A
	// ���G�̎�
	if (!HitCheck_Sphere_Sphere(p->GetVPos(), p->GetRadius(),
		e->GetVPos(), e->GetRadius() + e->GetRangeRadius())
		|| p->GetInvincible()) 
	{
		// �G�l�~�[�� range �� false �ɂ���
		e->SetFalseRange();
		return;
	}
	else { // �����Ă�����
		// �G�l�~�[�Ƀq�b�g�錾
		e->SetIsHitRange();
		e->SetPlayerPos(p->GetVPos());
	}
}
//---------------------------------------------
// �v���C���[�ƃG�l�~�[�̍U���͈͂̓����蔻��
//---------------------------------------------
void GameMain::CheckHitPlayer_EnemyAttackRange(Player * p, EnemyBase * e)
{
	// �G�l�~�[�̍U���͈͂ɓ����Ă��Ȃ��ꍇ���������Ȃ�
	if (!HitCheck_Sphere_Sphere(p->GetVPos(), p->GetRadius(),
		e->GetVPos(), e->GetRadius() + e->GetAttackRangeRadius()))
	{
		// �G�l�~�[�� attackrange �� false �ɂ���
		e->SetFalseAttackRange();
		return;
	}
	else { // �����Ă�����
		// �G�l�~�[�Ƀq�b�g�錾
		e->SetIs_HitAttackRange();
		CheckHitPlayer_EnemyAttack(p, e);
	}
}
//---------------------------------------------
// �v���C���[�ƃG�l�~�[�̍U���̓����蔻��
//---------------------------------------------
void GameMain::CheckHitPlayer_EnemyAttack(Player * p, EnemyBase * e)
{
	// �������Ă�����
	// �G�l�~�[������ł��Ȃ�������
	if (e->GetAttack() && !e->GetGoal()) {
		if (HitCheck_Sphere_Capsule(e->GetVPos(), e->GetRadius() + e->GetAttackRangeRadius(),
			p->GetCapsuleUp(p->GetVPos(), 8.0f), p->GetCapsuleDown(p->GetVPos()), p->GetRadius())
			&& !e->GetDeath() && !p->GetInvincible())
		{
			// HP ���_���[�W�����炷
			p->HpProcessing(e->GetDamage());
			// ���G�t���O�𗧂Ă�
			p->SetInvincible();
		}
	}
}
//---------------------------------------------
// ���ƃG�l�~�[�̓����蔻��
//---------------------------------------------
void GameMain::CheckHitSword_Enemy(Player * p)
{
	// �U�����Ă��邩�m�F
	// ���Ă��Ȃ��ꍇ�������I��
	if (!p->GetAttack())return;

	for (int i = 0; i < enemy_m->GetSize(); i++)
	{
		EnemyBase * e = enemy_m->GetEnemyBase(i);
		// �G�l�~�[������ł����薳�G�������ꍇ
		if (e->GetDeath() || e->GetInvincible()) {
			continue;
		}
		// �ł�����A���ƃG�l�~�[���������Ă��邩�m�F
		if (HitCheck_Sphere_Capsule(e->GetVPos(), e->GetRadius(),
			p->GetCapsuleTip(p->GetSwordTipPos()), p->GetCapsuleRoot(p->GetSwordRootPos()), p->GetRadius()))
		{
			// HP�����炷
			e->HpProcessing(1);
			if (e->GetHp() == 0) {
				// �������Ă�����G�l�~�[���f���[�g
				e->SetIsHitDeath();
				// MP����
				p->AddMp(ADD_MP);
			}
			else {
				// �G�l�~�[�̖��G��
				e->SetInvincible();
			}
			// �G�l�~�[�̍��W���擾
			enemy_pos = e->GetPos();
			enemy_pos.y += 3.0f;
			// �X�R�A�����Z
			score += e->GetScore();
			// ���j�J�E���g���v���X����
			defeat_count++;
			// �G�l�~�[�̍��W�Ɏa���̃G�t�F�N�g�𐶐�
			effect.push_back(new Effect("hit_eff.efk", enemy_pos, true));
		}
	}
	// �U���t���O��������
	p->SetFalseAttack();
}
//---------------------------------------------
// ����U���ƃG�l�~�[�̓����蔻��
//---------------------------------------------
void GameMain::CheckHitSpacial_Enemy(Player * p)
{
	// ����U�����Ă��邩�m�F
	// ���Ă��Ȃ��ꍇ�������I��
	if (!p->GetSpacialAttack())return;

	for (int i = 0; i < enemy_m->GetSize(); i++)
	{
		EnemyBase *e = enemy_m->GetEnemyBase(i);

		// �G�l�~�[������ł����薳�G�������ꍇ
		if (e->GetDeath() || e->GetInvincible()) {
			continue;
		}

		// �G�l�~�[�Ɠ���U���͈̔�
		// ����U�����Ԃ�����U���̍ő厞�ԂɒB���Ă��Ȃ���
		if(HitCheck_Sphere_Sphere(e->GetVPos(), e->GetRadius(), p->GetSpacialPos(),10.0f) 
			&& p->GetSpacialTimer() < p->GetSpacialMaxTimer())
		{
			// �������Ă�����G�l�~�[���f���[�g
			e->SetIsHitDeath();
			// �X�R�A�����Z
			score += e->GetScore();
			// ���j�J�E���g���v���X����
			defeat_count++;
		}
	}
}
//---------------------------------------------
// �G�l�~�[�̍U���Ə�̓����蔻��
//---------------------------------------------
void GameMain::CheckHitEnemyAttack_Castle(EnemyBase * e, Castle * c)
{
	if (e->GetAttack() && e->GetGoal()) {
		if (HitCheck_Line_Sphere(c->GetCastlePos1(), c->GetCastlePos2(), 
			e->GetVPos(), e->GetRadius() + e->GetAttackRangeRadius()) 
			&& !e->GetDeath())
		{
			// HP ���_���[�W�����炷
			c->HpProcessing(e->GetDamage());
			// �G�l�~�[�̍U���t���O��false�ɂ���
			e->SetFalseAttack();
			// ���HP���O�ȉ��̎�
			if (c->GetHp() <=  0) {
				c->SetHPInit();
			}
		}
	}
}
//---------------------------------------------
// �}�b�v�̓����蔻��
//---------------------------------------------
void GameMain::CheckHitMap(Castle * m, CollisionBase * c)
{
	// ���i�ǁj�ƃJ�v�Z���i�v���C���[�j�̓����蔻��
	for (int i = 0; i < pos2.size(); i++) {
		if (HitCheck_Line_Sphere(pos1[i].GetV(),pos2[i].GetV(),c->GetVPos(),c->GetRadius())) 
		{
			//------------------------------------
			// �_�Ɛ���̂P�ԋ߂����������߂鏈��
			//------------------------------------
			// ���C����̃x�N�g��
			Vector3 line_v = pos1[i] - pos2[i];
			// ���C���̍��Wpos1�ƃv���C���[�̍��W�̃x�N�g��
			Vector3 pos_v = c->GetPos() - pos1[i];

			Vector3 dot_f;		// ���ς����߂�p

			// ���C���̍��Wpos1����_�Ɛ���̓_��1�ԋ߂����W�Ƃ̋���
			float line_lenght = dot_f.dot(line_v.normalize(),pos_v);
			// �_�Ɛ���̂P�ԋ߂��_���W
			Vector3 min_point = pos1[i] + (line_v.normalize() * line_lenght);

			//------------------------------------
			// �����߂�
			//------------------------------------
			// �Q�_�Ԃ̃x�N�g�� = �v���C���[�̍��W - ����1�ԋ߂��_���W
			Vector3 v = c->GetPos() - min_point;

			float lenght;	// �����������������߂�p

			// ������������ = �v���C���[�̔��a - �_�Ɛ��̂P�ԋ߂�����
			lenght = c->GetRadius() - Segment_Point_MinLength(pos1[i].GetV(), pos2[i].GetV(), c->GetVPos());
			// �����߂������W�@=�@�v���C���[�̍��W + �Q�_�Ԃ̒P�ʃx�N�g�� * ������������
			Vector3 result = c->GetPos() + v.normalize() * lenght;
			// �v���C���[�̍��W�ɑ��
			c->SetPos(result);
		}
	}
	// �~�i�����A�^���[�j�ƃJ�v�Z���i�v���C���[�j�̓����蔻��
	for (int i = 0; i < radius.size(); i++) 
	{
		// �~�ƃJ�v�Z���̓����蔻��
		if (HitCheck_Sphere_Capsule(pos1[i + pos2.size()].GetV(), radius[i], 
			c->GetCapsuleUp(c->GetVPos(), 8.0f), c->GetCapsuleDown(c->GetVPos()), c->GetRadius())) 
		{
			// �Q�_�Ԃ̃x�N�g�� = �v���C���[�̍��W - �~�̍��W
			Vector3 v = c->GetPos() - pos1[i + pos2.size()];

			Vector3 sprt;	// �Q�_�Ԃ̋��������߂�p
			float lenght;	// �����������������߂�p

			// ������������ = �v���C���[�̔��a + �~�̔��a - �Q�_�Ԃ̋���
			lenght = c->GetRadius() + radius[i] - sprt.GetSqrtf(c->GetPos(), pos1[i + pos2.size()]);
			// �����߂������W = �v���C���[�̍��W + �Q�_�Ԃ̒P�ʃx�N�g�� * ������������
			Vector3 result = c->GetPos() + v.normalize() * lenght;
			// �v���C���[�̑��
			c->SetPos(result);
		}
	}
}
//---------------------------------------------
// �Q�[���I�[�o�[�`�F�b�N
//---------------------------------------------
void GameMain::CheckGameOver()
{
	if (map_m == nullptr) { return; }

	HPCheck(map_m->GetCastle());

	if (player_m == nullptr) { return; }

	HPCheck(player_m->GetPlayer());
}
//---------------------------------------------
// �Q�[���N���A�`�F�b�N
//---------------------------------------------
void GameMain::CheckGameClear()
{
	if (play_timer == 0) {
		clear = true;
	}
}
//---------------------------------------------
// �Q�[���N���A
//---------------------------------------------
void GameMain::GameClear()
{
	result_timer++;
	for (int i = 0; i < enemy_m->GetSize(); i++)
	{
		EnemyBase *e = enemy_m->GetEnemyBase(i);
		// �������Ă�����G�l�~�[���f���[�g
		e->SetIsHitDeath();
	}

	// ���U���g�^�C�}�[���N���A�̍ő厞�Ԃ𒴂�����
	if (result_timer > CLEAR_MAX_TIME) {
		scene_type = RESULT;
		result = true;
		// ���U���g�t�@�C���ɏ�������
		ResultFileWrite();
	}
}
//---------------------------------------------
// �Q�[���I�[�o�[
//---------------------------------------------
void GameMain::GameOver()
{
	result_timer++;

	// ���U���g�^�C�}�[���Q�[���I�[�o�[�̍ő厞�Ԃ𒴂�����
	if (result_timer > FAIL_MAX_TIME) {
		scene_type = RESULT;
		result = false;
		// ���U���g�t�@�C���ɏ�������
		ResultFileWrite();
	}
}
//---------------------------------------------
// ��̎c��HP���`�F�b�N
//---------------------------------------------
void GameMain::HPCheck(CollisionBase * c)
{
	if (c->GetHp() <= 0) {
		fail = true;
	}
}
//---------------------------------------------
// pos1�t�@�C����ǂݍ���
//---------------------------------------------
void GameMain::Pos1FileRead()
{
	file.open("data/txt/LoadObjectPos1.txt", ios::in);

	if (!file.is_open()) {
		// �t�@�C�����J�����ƂɎ��s���܂���
		return;
	}

	string str;
	while (getline(file, str))
	{
		while (true) {
			string::size_type pos = str.find(',', 0);
			if (pos == string::npos) break;
			str.replace(pos, 1, " ");	// replace=�u������/ replace(�ꏊ,������,�Ȃ�ĕ�����)
		}

		stringstream sstr(str);
		float x,y,z;
		sstr >> x >> y >> z;
		pos1.push_back(VECTOR{x,y,z});
	}

	// �t�@�C�������
	file.close();
}
//---------------------------------------------
// pos2�t�@�C����ǂݍ���
//---------------------------------------------
void GameMain::Pos2FileRead()
{
	file.open("data/txt/LoadObjectPos2.txt", ios::in);

	if (!file.is_open()) {
		// �t�@�C�����J�����ƂɎ��s���܂���
		return;
	}

	string str;
	while (getline(file, str))
	{
		while (true) {
			string::size_type pos = str.find(',', 0);
			if (pos == string::npos) break;
			str.replace(pos, 1, " ");	// replace=�u������/ replace(�ꏊ,������,�Ȃ�ĕ�����)
		}

		stringstream sstr(str);
		float x, y, z;
		sstr >> x >> y >> z;
		pos2.push_back(VECTOR{ x,y,z });
	}

	// �t�@�C�������
	file.close();
}
//---------------------------------------------
// radius�t�@�C����ǂݍ���
//---------------------------------------------
void GameMain::RadiusFileRead()
{
	file.open("data/txt/LoadObjectRadius.txt", ios::in);

	if (!file.is_open()) {
		// �t�@�C�����J�����ƂɎ��s���܂���
		return;
	}

	string str;
	while (getline(file, str))
	{
		stringstream sstr(str);
		float r;
		sstr >>r;
		radius.push_back(r);
	}

	// �t�@�C�������
	file.close();
}
//---------------------------------------------
// result�t�@�C����������
//---------------------------------------------
void GameMain::ResultFileWrite()
{
	file.open("data/txt/Result.txt", ios::out);

	if (!file.is_open()) {
		// �t�@�C�����J�����ƂɎ��s���܂���
		return;
	}

	if (map_m == nullptr) { return; }
	Castle *c = map_m->GetCastle();
	float rate = (float)c->GetHp() / (float)c->GetMaxHp();
	float life = rate * 100.0f;

	// �N���A�����s��
	file << result << ' ';
	// �X�R�A
	file << score << ' ';
	// ��̎c��HP
	file << (int)life << ' ';
	// ���j��
	file << defeat_count << endl;

	// �t�@�C�������
	file.close();
}
//---------------------------------------------
// Castle��HP�Q�[�W�̕`��
//---------------------------------------------
void GameMain::DrawCastleHPGauge(Castle * m)
{
	// ����450�@�c�� 60
	// �_���[�W
	// 45
	// 90
	// 135
	int life_size_x = CASTLE_LIFEGAUGE_W;
	int life_size_y = CASTLE_LIFEGAUGE_H;
	int life_x = CASTLE_LIFEGAUGE_X - life_size_x;
	int life_y = CASTLE_LIFEGAUGE_Y - life_size_y;
	// �D�F�Q�[�W
	DrawBox(life_x, life_y, life_x + life_size_x * 2, life_y + life_size_y * 2, BLACKGRAY, TRUE);
	// old_HP�Q�[�W
	DrawBox(life_x, life_y, life_x + (m->GetOldHp()* life_size_x * 2 / m->GetMaxHp()), life_y + life_size_y * 2, WHITE, WHITE);
	// HP�Q�[�W
	DrawBox(life_x, life_y, 
		life_x + (m->GetHp() * life_size_x * 2 / m->GetMaxHp()) , life_y + life_size_y * 2, GREEN, TRUE);

	float rate = (float)m->GetHp() / (float)m->GetMaxHp();
	float life = rate * 100.0f;
	DrawFormatStringToHandle(CASTLE_LIFEGAUGE_X - 40, CASTLE_LIFEGAUGE_Y - 10, GetColor(255, 255, 255), font_handle[FONT_HP],"%.0f ��", life);
}
