#include "Player.h"

//----------------------------------------------------------------------
// �R���X�g���N�^
//----------------------------------------------------------------------
Player::Player(PhysicsWorld w)
{
	p_pos = Vec2(PLAYER_POS_X, PLAYER_PO_Y);
	m_pos = Vec2(0, 0);
	p_rot = PLAYER_ROT;
	world = w;
	player = world.createCircle(Vec2(p_pos.x, p_pos.y), Circle(p_rot)),none,none, PhysicsBodyType::Static;
	
	jump_f = true;
}

//----------------------------------------------------------------------
// �f�X�g���N�^
//----------------------------------------------------------------------
Player::~Player()
{
}
//----------------------------------------------------------------------
// ����������
//----------------------------------------------------------------------
void Player::Init()
{
	p_pos = Vec2(PLAYER_POS_X, PLAYER_PO_Y);
	m_pos = Vec2(0, 0);
	player = world.createCircle(Vec2(p_pos.x, p_pos.y), Circle(p_rot)), none, none, PhysicsBodyType::Static;
	jump_f = true;
}
//----------------------------------------------------------------------
// �X�V����
//----------------------------------------------------------------------
void Player::Update()
{
#if ! defined(NDEBUG)
#else
	
#endif

	// �v���C���[�̃|�W�V�����ɍ��W��ݒ�
	p_pos = player.getPos();

	// �W�����v
	JumpCon();
	JumpKey();
	// �ړ�
	MoveCon();
	MoveKey();
	// �d��
	Gravity();

	// ��Ɉړ������v���C���[�̍��W�ɑ���
	p_pos.y += m_pos.y;
	p_pos.x += m_pos.x;
	
	// �{�[���̍��W�ɐݒ�
	player.setPos(p_pos);
}

//----------------------------------------------------------------------
// �`�揈��
//----------------------------------------------------------------------
void Player::Draw()
{
	// �v���C���[�{�̂�`��
	player.draw();

	//Println(p_pos.x);
}
//----------------------------------------------------------------------
// m_pos.y��������
//----------------------------------------------------------------------
void Player::InitM_Pos()
{
	m_pos.y = 0;
}
//----------------------------------------------------------------------
// �W�����v�t���O��ς���
//----------------------------------------------------------------------
void Player::CahegFlag()
{
	if (jump_f == true) {
		jump_f = false;
	}
}
//----------------------------------------------------------------------
// �R���g���[���[
//----------------------------------------------------------------------
//-----------------------------------
// �ړ�
//-----------------------------------
void Player::MoveCon()
{
	m_pos.x = controller.leftThumbX * 7.0;

	if (controller.buttonRight.pressed) {
		m_pos.x = 6;
	}
	else if(controller.buttonLeft.pressed){
		m_pos.x = -6;
	}
}
//-----------------------------------
// �W�����v
//-----------------------------------
void Player::JumpCon()
{
	if (controller.buttonA.clicked && jump_f == false) {
		m_pos.y += 10;
		jump_f = true;
	}
}
//----------------------------------------------------------------------
//�@�L�[�{�[�h
//----------------------------------------------------------------------
//-----------------------------------
// �ړ�
//-----------------------------------
void Player::MoveKey()
{
	if (Input::KeyShift.pressed) {
		if (Input::KeyD.pressed) {
			m_pos.x = 6;
		}
		else if (Input::KeyA.pressed) {
			m_pos.x = -6;
		}
		
		

	}
	else {
		if (Input::KeyD.pressed) {
			m_pos.x = 3;
		}
		else if (Input::KeyA.pressed) {
			m_pos.x = -3;
		}
	}
}
//-----------------------------------
// �W�����v
//-----------------------------------
void Player::JumpKey()
{
	if (Input::KeySpace.clicked && jump_f == false) {
		m_pos.y = 0;
		m_pos.y += 10;
		jump_f = true;
	}

}
//-----------------------------------
// �d��
//-----------------------------------
void Player::Gravity()
{
	m_pos.y -= 0.3;
}


