#include "WinMain.h"
#include "Player.h"
#include "Fiona.h"
#include "Musya.h"
#include "Ninin.h"

//-----------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------
Player::Player()
{
	Init();
	if (playerbase != nullptr) {
		playerbase = nullptr;
	}
	jflag = false;
	gflag = true;
}
//-----------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------
Player::~Player()
{
	Exit();
}

//-----------------------------------------------------------
//�@���������� 
//-----------------------------------------------------------
void Player::Init()
{
	score = 0.00;
	x = 150;
	y = 0;
	number = FIONA;
	old_number = number;
}
//-----------------------------------------------------------
// �I������
//-----------------------------------------------------------
void Player::Exit()
{
	if (playerbase != nullptr) {
		delete playerbase;
		playerbase = nullptr;
	}
}
//-----------------------------------------------------------
// �X�V����
//-----------------------------------------------------------
void Player::Update()
{
	SetOld();
	old_number = number;
	// �L�����N�^�[�`�F���W
	Character_Change();
	// �Z�b�g�����L�����N�^�[�o�Ȃ��Ƃ�����
	Character_Delete();
	// �ǂ̃L�����N�^�[�ɂ��邩�ݒ�
	Character_Set();

	// �L�����N�^�[�̍X�V����
	if (playerbase != nullptr) {
		playerbase->Update();
	}
	Draw_Storage();
	Pos_Old_Storage();
	Pos_Storage();
	Dir_Storage();
	Speed_Storage();
}
//-----------------------------------------------------------
// �`�揈��
//-----------------------------------------------------------
void Player::Draw(){
	if (playerbase != nullptr) {
		playerbase->Draw();
	}
}
//-----------------------------------------------------------
// �摜�̓ǂݒ����p�̊֐�
//-----------------------------------------------------------
void Player::ReloadGraph()
{
	if (playerbase != nullptr) {
		playerbase->ReloadGraph();
	}
}
//-----------------------------------------------------------
// �W�����v�t���O�؂�ւ�
//-----------------------------------------------------------
void Player::ChangeJflag()
{
	if (jflag) {
		jflag = false;
	}
	move_y = 0;
	if (playerbase != nullptr) {
		playerbase->ChangeJflag();
	}
}
//-----------------------------------------------------------
// old_x�ɖ߂�
//-----------------------------------------------------------
void Player::ResetPosX() 
{
	x = old_x;
	if (playerbase != nullptr) {
		playerbase->ResetPosX();
	}
}
//-----------------------------------------------------------
// draw_oldx�ɖ߂�
//-----------------------------------------------------------
void Player::ResetDrawX()
{
	if (playerbase != nullptr) {
		playerbase->ResetDrawX();
	}
}
//-----------------------------------------------------------
// old_y�ɖ߂�
//-----------------------------------------------------------
void Player::ResetPosY()
{
	y = old_y;
	if (playerbase != nullptr) {
		playerbase->ResetPosY();
	}
}
//-----------------------------------------------------------
// �L�����N�^�[�`�F���W
//-----------------------------------------------------------
void Player::Character_Change()
{
	if (CheckHitKey(KEY_INPUT_1)) {
		number = FIONA;
	}
	if (CheckHitKey(KEY_INPUT_2)) {
		number = MUSYA;
	}
	if (CheckHitKey(KEY_INPUT_3)) {
		number = NININ;
	}
}
//-----------------------------------------------------------
// �Z�b�g�����L�����N�^�[�łȂ����f���[�g����
//-----------------------------------------------------------
void Player::Character_Delete()
{
	if (number != old_number) {
		Exit();
	}
}
//-----------------------------------------------------------
// ���W��ۑ�
//-----------------------------------------------------------
void Player::Pos_Storage()
{
	if (playerbase != nullptr) {
		x = playerbase->GetPosX();
		y = playerbase->GetPosY();
		move_y = playerbase->GetMoveY();
	}
}
//-----------------------------------------------------------
// old���W��ۑ�
//-----------------------------------------------------------
void Player::Pos_Old_Storage()
{
	if (playerbase != nullptr) {
		old_x = playerbase->GetOldX();
		old_y = playerbase->GetOldY();
	}
}
//-----------------------------------------------------------
// ������ۑ�
//-----------------------------------------------------------
void Player::Dir_Storage()
{
	if (playerbase != nullptr) {
		dir = playerbase->GetDir();
	}
}
//-----------------------------------------------------------
// �X�s�[�h��ۑ�
//-----------------------------------------------------------
void Player::Speed_Storage()
{
	if (playerbase != nullptr) {
		speed = playerbase->GetSpeed();
	}
}
//-----------------------------------------------------------
// �X�s�[�h��ۑ�
//-----------------------------------------------------------
void Player::Draw_Storage()
{
	if (playerbase != nullptr) {
		draw_x = playerbase->GetDrawX();
		draw_y = playerbase->GetDrawY();
	}
}
//-----------------------------------------------------------
// �W�����v�t���O�擾
//-----------------------------------------------------------
bool Player::GetJflag() const
{
	return playerbase->GetJflag();
}
//-----------------------------------------------------------
// �}�b�v�͈͓̔�������
//-----------------------------------------------------------
bool Player::Check_Map_Range()
{
	if (playerbase != nullptr) {
		if (playerbase->GetPosX() <= -12 || playerbase->GetDrawX() >= MAP_WIDTH_MAX * MAP_SIZE - 185) {
			playerbase->ResetPosX();
			playerbase->ResetDrawX();
			return true;
		}
	}
	return false;
}
//-----------------------------------------------------------
// �L�����N�^�[�ݒ�
//-----------------------------------------------------------
void Player::Character_Set()
{
	switch (number)
	{
	case FIONA:
		if (playerbase == nullptr) {
			playerbase = new Fiona(x,y,dir,draw_x);
		}
		break;
	case MUSYA:
		if (playerbase == nullptr) {
			playerbase = new Musya(x,y,dir,draw_x);
		}
		break;
	case NININ:
		if (playerbase == nullptr) {
			playerbase = new Ninin(x, y, dir,draw_x);
		}
		break;
	default:
		break;
	}
}
//-----------------------------------------------------------
// �X�R�A�𑫂�
//-----------------------------------------------------------
void Player::AddScore()
{
	score += ADD_SCORE;
}
//-----------------------------------------------------------
// �^�C�}�[���i�ނ��тɃX�R�A�𑫂�
//-----------------------------------------------------------
void Player::TimerScore(double timer_)
{
	score += timer_;
}


