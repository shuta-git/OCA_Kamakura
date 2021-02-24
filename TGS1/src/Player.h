#pragma once
#include "Main.h"

class Player 
{
private:
	Vec2 m_pos;		// 移動用の変数
	Vec2 p_pos;	    // 座標用の変数
	int p_rot;		// 半径

	PhysicsWorld world;		// 物理演算設定
	PhysicsBody player;		// 物理演算用の型を作成

	// ジャンプフラグ
	bool jump_f;
public:
	Player(PhysicsWorld w);
	~Player();

	//----------------
	// Get関数
	//----------------
	double GetPosX() const { return p_pos.x; }			// X座標を取得
	double GetPosY() const { return p_pos.y; }			// Y座標を取得
	double GetMPosX()const { return m_pos.x; }
	Vec2 GetPos() const { return p_pos; }				// Vec2で座標を取得
	int GetRot() const { return p_rot; }				// 半径をを取得
	PhysicsBody GetBody() const { return player; }		// PhysicsBodyを取得
	
	// ジャンプフラグを変える
	void CahegFlag();

	//----------------
	// 規定関数
	//----------------
	void Init();
	void Update();
	void Draw();

	void InitM_Pos();

private:

	//----------------
	// コントローラー
	//----------------
	void JumpCon();			// ジャンプ
	void JumpKey();			// ジャンプ
	void MoveCon();			// 移動
	void MoveKey();			// 移動
	void Gravity();			// 重力
};

