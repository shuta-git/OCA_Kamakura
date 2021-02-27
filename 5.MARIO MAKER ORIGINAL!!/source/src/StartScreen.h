#pragma once
#include "ScreenBase.h"
#include "Cursor.h"


//-----------------------------------------------------------
// �萔
//-----------------------------------------------------------
#define MAX_SIZE_X 100
#define MAX_SIZE_Y 200

//-----------------------------------------------------------
// �N���X
//-----------------------------------------------------------
class StartScreen : public ScreenBase
{
public:
	StartScreen();
	~StartScreen();

	void Update();
	void Draw();
	void ReloadGraph();

private:
	Cursor * cur = nullptr;
};


