#pragma once
#include "Main.h"
#include "Player.h"
#include "Block.h"
#include "Trap.h"


class Map : public Task
{
private:
	Vec2 m_pos;		// ���W
	Vec2 move_pos;
	Vec2 t_drop_pos;

	TaskCall    m_Update;   //�X�V�ݒ�
	TaskCall    m_Draw;     //�`��ݒ�

	PhysicsWorld world;		// �������Z�ݒ�

	Player* player;			// �v���C���[���|�C���^�[�Őݒ�

	TaskReceive b_Receive;  //�u���b�N�Ƃ̎�M�ݒ�
	TaskReceive t_Receive;  //�g���b�v�Ƃ̎�M�ݒ�

	bool is_tdrop_range;// TRIANGLE_DROP�p
	bool is_tout_range;	// TRIANGLE_OUT�p
	bool p_death;		// �v���C���[�����S�������̔���
	bool is_droptrap_block;
	bool is_droptrap;

	const Font m_font;
	

public:
	Map() : Task()
		, m_pos(0, -320)
		, m_Update(this, &Map::Update, CallGroup_Update)
		, m_Draw(this, &Map::Draw, CallGroup_Draw, CallPriority_Map)
	{
		player = new Player(world);
		p_death = false;
		is_tout_range = false;
		is_droptrap = false;
		is_droptrap_block = false;
		// ��M��̐ݒ�
		// �u���b�N
		b_Receive.Register<Block>(this, &Map::HitCheckPB);		
		// �g���b�v
		t_Receive.Register<Trap>(this, &Map::HitCheckPT);
	}
	~Map();

	//----------------
	// Get�֐�
	//----------------
	// �v���C���[��X���W�擾
	double GetPlayerPosX() const { return player->GetPosX(); }	
	// �v���C���[��Y���W���擾
	double GetPlayerPosY() const { return player->GetPosY(); }	
	// �������Z�p�̖{�̂��擾
	PhysicsWorld GetWorld() const { return world; }		
	// Circle�v���C���[���擾
	Circle GetPlayer() const {return  Circle(player->GetPosX(), player->GetPosY(), player->GetRot() + 1); }
	//----------------
	// Trap
	//----------------
	// is_tout_range���擾(TRIANGLE_OUT)
	bool GetTOutRange()const { return is_tout_range; }
	// is_tdrop_range���擾(TRIANGLE_DROP)
	bool GetTDropRange()const { return is_tdrop_range; }
	// �v���C���[������ł��邩�擾
	bool GetPDeath()const { return p_death; }
	// �u���b�N�ƃg���b�v�̓����蔻��`�F�b�N
	bool Is_DropTrap_Block() const { return is_droptrap_block; }
	// �g���b�v�ƃv���C���[�̓����蔻��`�F�b�N
	bool IsDropTrapPlayer()const { return is_droptrap; }
	// �h���b�v�g���b�v�̃|�W�V����X���擾
	double GetDropTrapPosX() const {  return t_drop_pos.x; }
	// �h���b�v�g���b�v�̃|�W�V����Y���擾
	double GetDropTrapPosY() const { return t_drop_pos.y; }
	

	// �v���C���[�ƃu���b�N�̔���
	void HitCheckPB(Block & block);
	// �v���C���[�ƃg���b�v�̓����蔻��
	void  HitCheckPT(Trap & trap);
	// is_tout_range��ݒ�(TRIANGLE_OUT)
	void SetTOutRange();
private:
	//----------------
	// �K��֐�
	//----------------
	void Update();
	void Draw();

	
};

