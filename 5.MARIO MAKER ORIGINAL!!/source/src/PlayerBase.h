#pragma once
#include "BaseObj.h"

#define JUMP_UP 1.3F

class PlayerBase : public BaseObj
{

private:

	

public:
	PlayerBase();
	~PlayerBase();

	virtual void Init() = 0;
	virtual void Exit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void ReloadGraph() = 0;
	void SetDrawX() { draw_old_x = draw_x; }
	void ResetDrawX() { draw_x = draw_old_x; }					// リセットdraw_X
	void ResetPosX() { x = old_x; }					// リセットX
	void ResetPosY() { y = old_y, move_y = 0; }		// リセットY
	void ChangeJflag();								// フラグチェンジ
	bool MoveCheckRange();					// 動ける範囲内かチェック

protected:

	float gspd;
	float jspd;

	virtual void Dir_Change();				// 向きのチェック
	virtual void Animation();				// アニメーション
	virtual void Move();					// 移動
	void Jump();							// ジャンプ
	void Gravity();							// 重力

	virtual void DashDraw() = 0;
	virtual void NormalDraw() = 0;
	virtual int SetDraw() = 0;

	void MoveDrawX();


	

};

