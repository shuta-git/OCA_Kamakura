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

	TaskCall    b_Update;   //更新設定
	TaskCall    b_Draw;     //描画設定

	PhysicsWorld world;

	PhysicsBody ground;

	TaskReceive bt_Receive; //ブロックとトラップとの受信

	TaskGet m_Get;

	TaskSend    m_Send; //送信設定

	int type;
	// プレイヤーが範囲に入った時用
	bool is_droptrap;
	// ドロップトラップがブロックに当たった時用
	bool is_droptrap_block;
//	// プレイヤーがドロップトラップにのているとき用
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
		// 物理演算用のグラウンドを作成
		// 計4ケ所変更 Trap.h Trap.cpp GameMain.cpp 2ヶ所 
		switch (num)
		{
										//反時計回り								上							左					右			
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
		// 送信元の設定
		m_Send.Register(this);
	}
	~Trap();

	//----------------
	// Get関数
	//----------------
	// トラップすべての当たり判定を取得する
	Triangle GetTriangle() const;
	// 飛び出るトラップの範囲の当たり判定を取得
	Circle GetRangeTRIANGLE_OUT() const;
	// 落ちるトラップの範囲の当たり判定を取得
	Triangle GetRangeTRIANGLE_DROP() const;
	// ドロップトラップの頂点の当たり判定を取得
	Triangle GetTriangle_Drop()const;
	// ドロップトラップの上の辺の当たり判定を取得
	Triangle GetOnTriagle_Drop()const;

	// ドロップトラップのフラグを取得
	bool GetIs_DropTrap_Block() const { return is_droptrap_block; }
	// ドロップトラップのX座標を取得
	double GetDropTrapPosX() const { return t_drop_pos.x; }
	// ドロップトラップのY座標を取得
	double GetDropTrapPosY() const { return t_drop_pos.y; }
	// ドロップトラップの座標を取得
	Vec2 GetDropTrapPos() const { return t_drop_pos; }
	// ブロックとトラップの当たり判定チェック
	bool IsDropTrapBlock()const { return is_droptrap_block; }
	// トラップとプレイヤーの当たり判定チェック
	bool IsDropTrapPlayer()const { return is_droptrap; }

	// このタスクを消す
	void SetDestroy();
	// TRIANGLE_DROPのt_pos.yをマイナスに下げていく
	void DropTriangle();
	// playerと当たっているか
	void Is_DropChange();
	// ブロックとトラップの当たり判定
	void HitCheckBT(Block & block);

private:
	//----------------
	// 規定関数
	//----------------
	void Update();
	void Draw();

public:
};

