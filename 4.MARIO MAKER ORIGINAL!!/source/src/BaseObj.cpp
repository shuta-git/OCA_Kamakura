#include "WinMain.h"
#include "BaseObj.h"


BaseObj::BaseObj()
{
	dir = false;
	ptr = 0;
	speed = 0.0f;
	count = 0;

	old_x = 0.0f;
	old_y = 0.0f;
	move_x = 0.0f;
	move_y = 0.0f;
	draw_old_x = 0.0f;
	draw_x = 0.0f;
	draw_y = 0.0f;
}


BaseObj::~BaseObj()
{
}

void BaseObj::SetOld()
{
	old_x = x;
	old_y = y;
}





