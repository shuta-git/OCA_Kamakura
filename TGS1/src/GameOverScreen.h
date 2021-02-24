#pragma once
#include"Main.h"

class GameOverScreen
{
private:
	float time;
	bool death;
	int count;
public:
	GameOverScreen();
	~GameOverScreen();

	void Init();
	void Update();
	void Draw(Font font, Font font1);
	void Exit();

	bool GetDeath() const { return death; }

private:
};

