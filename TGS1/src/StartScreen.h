#pragma once
#include "Main.h"

class StartScreen
{
private:
	bool start;
	int time;
public:
	StartScreen();
	~StartScreen();

	void Init();
	void Update();
	void Draw(Font font, Font font1);
	void Exit();

	bool GetStart() const { return start; }

private:



};

