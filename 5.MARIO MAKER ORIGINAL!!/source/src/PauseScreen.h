#pragma once
#include "ScreenBase.h"
#include "Cursor.h"


class PauseScreen :	public ScreenBase
{
public:
	PauseScreen();
	~PauseScreen();

	void Update();
	void Draw();
	void ReloadGraph();

private:
	Cursor * cur = nullptr;
	
};

