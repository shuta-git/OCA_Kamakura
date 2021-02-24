#pragma once
#include"Main.h"

class Block : public Task
{
private:
	Vec2 b_pos;		// ���W
	Vec2 t_pos;

	TaskCall    b_Update;   //�X�V�ݒ�
	TaskCall    b_Draw;     //�`��ݒ�

	PhysicsWorld world;

	PhysicsBody ground;

	TaskSend    m_Send; //���M�ݒ�

public:
	Block(PhysicsWorld w,double x , double y) : Task()
		, b_pos(0, 0)
		,t_pos(x,y)
		, b_Update(this, &Block::Update, CallGroup_Update)
		, b_Draw(this, &Block::Draw, CallGroup_Draw, CallPriority_Map)
	{
		world = w;
		// �������Z�p�̃O���E���h���쐬
		ground = world.createRect(Vec2(0, 0), RectF({ b_pos.x + (t_pos.x * MAP_SIZE),b_pos .y + (t_pos.y * MAP_SIZE) }, { MAP_SIZE,MAP_SIZE }),none,none, PhysicsBodyType::Static);
		// ���M���̐ݒ�
		m_Send.Register(this);
	}
	~Block();
 
	//----------------
	// Get�֐�
	//----------------
	RectF GetRect() const { return  RectF(b_pos.x + (t_pos.x * MAP_SIZE), b_pos.y + (t_pos.y * MAP_SIZE), { MAP_SIZE,MAP_SIZE }); }
	
	// ���̃^�X�N������
	void SetDestroy();

private:

	void Update();
	void Draw();
};

