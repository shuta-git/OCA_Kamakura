#pragma once
#include "BaseObj.h"

//-----------------------------------------------------------
// ƒNƒ‰ƒX
//-----------------------------------------------------------
class Enemy : public BaseObj
{
public:
	Enemy();
	~Enemy();
public:
	void Init();
	void Exit();
	void Update();
	void Draw();
private:

};

