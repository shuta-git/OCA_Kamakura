#pragma once
#include"Main.h"
#include"Block.h"


class Trap : public Task
{
private:
	Vec2 t_pos;
	Vec2 t_speed;
	Vec2 t_drop_pos;
	//Vec2 up_pos;
	//Vec2 left_pos;
	//Vec2 right_pos;

	TaskCall    b_Update;   //�X�V�ݒ�
	TaskCall    b_Draw;     //�`��ݒ�

	PhysicsWorld world;

	PhysicsBody ground;

	TaskReceive bt_Receive; //�u���b�N�ƃg���b�v�Ƃ̎�M

	TaskGet m_Get;

	TaskSend    m_Send; //���M�ݒ�

	int type;
	// �v���C���[���͈͂ɓ��������p
	bool is_droptrap;
	// �h���b�v�g���b�v���u���b�N�ɓ����������p
	bool is_droptrap_block;
//	// �v���C���[���h���b�v�g���b�v�ɂ̂Ă���Ƃ��p
//	bool is_on_droptrap;

	const Font m_font;

public:
	Trap(PhysicsWorld w, double x, double y,int num) : Task()
		, t_pos(x, y)
		, b_Update(this, &Trap::Update, CallGroup_Update)
		, b_Draw(this, &Trap::Draw, CallGroup_Draw, CallPriority_Trap)
		, m_Get(this)
	{
		bt_Receive.Register<Block>(this, &Trap::HitCheckBT);
		type = num;
		world = w;
		t_speed.x = 0;
		t_speed.y = 0;
		t_drop_pos.x = 0;
		t_drop_pos.y = 0;
		is_droptrap = false;
		is_droptrap_block = false;
//		is_on_droptrap = false;
		// �������Z�p�̃O���E���h���쐬
		// �v4�P���ύX Trap.h Trap.cpp GameMain.cpp 2���� 
		switch (num)
		{
										//�����v���								��							��					�E			
		case TRIANGLE_UP:
			ground = world.createTriangle(Vec2(0, 0), Triangle(t_pos.x + MAP_SIZE / 2, t_pos.y + MAP_SIZE, t_pos.x, t_pos.y, t_pos.x + MAP_SIZE, t_pos.y), none, none, PhysicsBodyType::Static);
			break;
		case TRIANGLE_DOWN:
			ground = world.createTriangle(Vec2(0, 0), Triangle(t_pos.x + MAP_SIZE / 2, t_pos.y, t_pos.x + MAP_SIZE, t_pos.y + MAP_SIZE, t_pos.x, t_pos.y + MAP_SIZE), none, none, PhysicsBodyType::Static);
			break;
		case TRIANGLE_RIGHT:
			ground = world.createTriangle(Vec2(0, 0), Triangle(t_pos.x + MAP_SIZE, t_pos.y + MAP_SIZE / 2, t_pos.x, t_pos.y + MAP_SIZE, t_pos.x, t_pos.y), none, none, PhysicsBodyType::Static);
			break;
		case TRIANGLE_LEFT:
			ground = world.createTriangle(Vec2(0, 0), Triangle(t_pos.x, t_pos.y + MAP_SIZE / 2, t_pos.x + MAP_SIZE, t_pos.y, t_pos.x + MAP_SIZE, t_pos.y + MAP_SIZE), none, none, PhysicsBodyType::Static);
			break;
		case TRIANGLE_OUT:
			ground = world.createTriangle(Vec2(0, 0), Triangle(t_pos.x + MAP_SIZE / 2, t_pos.y + MAP_SIZE / 2, t_pos.x, t_pos.y, t_pos.x + MAP_SIZE, t_pos.y), none, none, PhysicsBodyType::Static);
			break;
		case TRIANGLE_DROP:
			ground = world.createTriangle(Vec2(0, 0), Triangle(t_pos.x + MAP_SIZE / 2, t_pos.y - MAP_SIZE / 2  , t_pos.x + MAP_SIZE, t_pos.y + MAP_SIZE, t_pos.x, t_pos.y + MAP_SIZE ), none, none, PhysicsBodyType::Static);
			break;
		case TRIANGLE_OUT_1:
			ground = world.createTriangle(Vec2(0, 0), Triangle(t_pos.x + MAP_SIZE / 2, t_pos.y + MAP_SIZE + MAP_SIZE / 2, t_pos.x, t_pos.y, t_pos.x + MAP_SIZE, t_pos.y), none, none, PhysicsBodyType::Static);
			break;
		default:
			break;
		}
		// ���M���̐ݒ�
		m_Send.Register(this);
	}
	~Trap();

	//----------------
	// Get�֐�
	//----------------
	// �g���b�v���ׂĂ̓����蔻����擾����
	Triangle GetTriangle() const;
	// ��яo��g���b�v�͈̔͂̓����蔻����擾
	Circle GetRangeTRIANGLE_OUT() const;
	// ������g���b�v�͈̔͂̓����蔻����擾
	Triangle GetRangeTRIANGLE_DROP() const;
	// �h���b�v�g���b�v�̒��_�̓����蔻����擾
	Triangle GetTriangle_Drop()const;
	// �h���b�v�g���b�v�̏�̕ӂ̓����蔻����擾
	Triangle GetOnTriagle_Drop()const;

	// �h���b�v�g���b�v�̃t���O���擾
	bool GetIs_DropTrap_Block() const { return is_droptrap_block; }
	// �h���b�v�g���b�v��X���W���擾
	double GetDropTrapPosX() const { return t_drop_pos.x; }
	// �h���b�v�g���b�v��Y���W���擾
	double GetDropTrapPosY() const { return t_drop_pos.y; }
	// �h���b�v�g���b�v�̍��W���擾
	Vec2 GetDropTrapPos() const { return t_drop_pos; }
	// �u���b�N�ƃg���b�v�̓����蔻��`�F�b�N
	bool IsDropTrapBlock()const { return is_droptrap_block; }
	// �g���b�v�ƃv���C���[�̓����蔻��`�F�b�N
	bool IsDropTrapPlayer()const { return is_droptrap; }

	// ���̃^�X�N������
	void SetDestroy();
	// TRIANGLE_DROP��t_pos.y���}�C�i�X�ɉ����Ă���
	void DropTriangle();
	// player�Ɠ������Ă��邩
	void Is_DropChange();
	// �u���b�N�ƃg���b�v�̓����蔻��
	void HitCheckBT(Block & block);

private:
	//----------------
	// �K��֐�
	//----------------
	void Update();
	void Draw();

public:
};

