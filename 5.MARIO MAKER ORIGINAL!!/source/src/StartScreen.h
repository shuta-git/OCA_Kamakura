#pragma once
#include "ScreenBase.h"
#include "Cursor.h"


//-----------------------------------------------------------
// íËêî
//-----------------------------------------------------------
#define MAX_SIZE_X 100
#define MAX_SIZE_Y 200

//-----------------------------------------------------------
// ÉNÉâÉX
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


