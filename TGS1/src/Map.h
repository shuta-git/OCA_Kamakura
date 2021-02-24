#pragma once
#include "Main.h"
#include "Player.h"
#include "Block.h"
#include "Trap.h"


class Map : public Task
{
private:
	Vec2 m_pos;		// 座標
	Vec2 move_pos;
	Vec2 t_drop_pos;

	TaskCall    m_Update;   //更新設定
	TaskCall    m_Draw;     //描画設定

	PhysicsWorld world;		// 物理演算設定

	Player* player;			// プレイヤーをポインターで設定

	TaskReceive b_Receive;  //ブロックとの受信設定
	TaskReceive t_Receive;  //トラップとの受信設定

	bool is_tdrop_range;// TRIANGLE_DROP用
	bool is_tout_range;	// TRIANGLE_OUT用
	bool p_death;		// プレイヤーが死亡したかの判定
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
		// 受信先の設定
		// ブロック
		b_Receive.Register<Block>(this, &Map::HitCheckPB);		
		// トラップ
		t_Receive.Register<Trap>(this, &Map::HitCheckPT);
	}
	~Map();

	//----------------
	// Get関数
	//----------------
	// プレイヤーのX座標取得
	double GetPlayerPosX() const { return player->GetPosX(); }	
	// プレイヤーのY座標を取得
	double GetPlayerPosY() const { return player->GetPosY(); }	
	// 物理演算用の本体を取得
	PhysicsWorld GetWorld() const { return world; }		
	// Circleプレイヤーを取得
	Circle GetPlayer() const {return  Circle(player->GetPosX(), player->GetPosY(), player->GetRot() + 1); }
	//----------------
	// Trap
	//----------------
	// is_tout_rangeを取得(TRIANGLE_OUT)
	bool GetTOutRange()const { return is_tout_range; }
	// is_tdrop_rangeを取得(TRIANGLE_DROP)
	bool GetTDropRange()const { return is_tdrop_range; }
	// プレイヤーが死んでいるか取得
	bool GetPDeath()const { return p_death; }
	// ブロックとトラップの当たり判定チェック
	bool Is_DropTrap_Block() const { return is_droptrap_block; }
	// トラップとプレイヤーの当たり判定チェック
	bool IsDropTrapPlayer()const { return is_droptrap; }
	// ドロップトラップのポジションXを取得
	double GetDropTrapPosX() const {  return t_drop_pos.x; }
	// ドロップトラップのポジションYを取得
	double GetDropTrapPosY() const { return t_drop_pos.y; }
	

	// プレイヤーとブロックの判定
	void HitCheckPB(Block & block);
	// プレイヤーとトラップの当たり判定
	void  HitCheckPT(Trap & trap);
	// is_tout_rangeを設定(TRIANGLE_OUT)
	void SetTOutRange();
private:
	//----------------
	// 規定関数
	//----------------
	void Update();
	void Draw();

	
};

