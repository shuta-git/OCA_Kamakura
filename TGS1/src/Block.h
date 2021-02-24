#pragma once
#include"Main.h"

class Block : public Task
{
private:
	Vec2 b_pos;		// 座標
	Vec2 t_pos;

	TaskCall    b_Update;   //更新設定
	TaskCall    b_Draw;     //描画設定

	PhysicsWorld world;

	PhysicsBody ground;

	TaskSend    m_Send; //送信設定

public:
	Block(PhysicsWorld w,double x , double y) : Task()
		, b_pos(0, 0)
		,t_pos(x,y)
		, b_Update(this, &Block::Update, CallGroup_Update)
		, b_Draw(this, &Block::Draw, CallGroup_Draw, CallPriority_Map)
	{
		world = w;
		// 物理演算用のグラウンドを作成
		ground = world.createRect(Vec2(0, 0), RectF({ b_pos.x + (t_pos.x * MAP_SIZE),b_pos .y + (t_pos.y * MAP_SIZE) }, { MAP_SIZE,MAP_SIZE }),none,none, PhysicsBodyType::Static);
		// 送信元の設定
		m_Send.Register(this);
	}
	~Block();
 
	//----------------
	// Get関数
	//----------------
	RectF GetRect() const { return  RectF(b_pos.x + (t_pos.x * MAP_SIZE), b_pos.y + (t_pos.y * MAP_SIZE), { MAP_SIZE,MAP_SIZE }); }
	
	// このタスクを消す
	void SetDestroy();

private:

	void Update();
	void Draw();
};

