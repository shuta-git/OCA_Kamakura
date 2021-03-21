#pragma once
#include "Hit.h"
#include "Enemy.h"
#include "Map.h"
#include "Player.h"
#include "Screen.h"

//-----------------------------------------------------------
// ƒNƒ‰ƒX
//-----------------------------------------------------------
class GameMain {
public:
	GameMain();

	~GameMain();
		
	MAP* map;
	Player* player;
	Screen* screen;
	Cursor* cur = nullptr;

	void Init();
	void Update(int number_,int mode_num_,int old_num_, int chara_num_);
	void Draw();
	void Exit();
	void ReloadGraph();

	bool GetAlive() const { return alive; }
	bool GetClear() const { return clear; }

private:
	int number;
	int old_num;
	int mode_num;
	int chara_num;

	double timer;
	
	bool pause;
	bool alive;
	bool clear;

	void Pause();
	void ResetTimer();
};

