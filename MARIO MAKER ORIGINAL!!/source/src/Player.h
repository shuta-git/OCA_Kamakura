#pragma once
#include "PlayerBase.h"

//-----------------------------------------------------------
// 定数
//-----------------------------------------------------------
#define PLAYER_MAX 3
#define ADD_SCORE 5.00

//-----------------------------------------------------------
// クラス
//-----------------------------------------------------------
class Player : public BaseObj
{
public:
	Player();
	~Player();
	void Init();
	void Exit();
	void Update();
	void Draw();
	void ReloadGraph();

	void ChangeJflag();
	void ResetPosX();
	void ResetDrawX();
	void ResetPosY();

	void AddScore();
	void TimerScore(double timer_);

	double GetScore() const { return score; }
	int GetNum() const { return number; }
	bool GetJflag()const;

	bool Check_Map_Range();

private:
	int number;
	int old_number;
	double score;
	void Character_Set();			//	キャラクターセット
	void Character_Delete();		//  セットしたキャラクターでない時デリート処理
	void Character_Change();		//  キャラクターチェンジ
	void Pos_Storage();				//  座標を保存
	void Pos_Old_Storage();			//  old座標を保存
	void Dir_Storage();				//  向きを保存
	void Speed_Storage();			//  スピードを保存
	void Draw_Storage();			//  draw座標を保存
	

	PlayerBase* playerbase;

};



















