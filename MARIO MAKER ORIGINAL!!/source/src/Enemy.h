#pragma once
#include "BaseObj.h"

//-----------------------------------------------------------
// �N���X
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

