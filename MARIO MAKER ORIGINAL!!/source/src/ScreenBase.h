#pragma once
#include "BaseObj.h"

//-----------------------------------------------------------
// ƒNƒ‰ƒX
//-----------------------------------------------------------
class ScreenBase :public BaseObj
{
public:
	ScreenBase();
	~ScreenBase();

	void virtual Update() = 0;
	void virtual Update(int chara_num_);
	void virtual Draw();
	void virtual Exit();
	void virtual ReloadGraph() = 0;
};

