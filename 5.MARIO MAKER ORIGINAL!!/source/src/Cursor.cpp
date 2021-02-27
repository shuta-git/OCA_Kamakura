#include "Cursor.h"
#include "WinMain.h"

static const int MOVE_SPEED = MAP_SIZE;

//-----------------------------------------------------------
// 引数付きコンストラクタ
//-----------------------------------------------------------
Cursor::Cursor(int x_, int y_) :
	m_x(x_), m_y(y_), m_size(MAP_SIZE)
{

}
//-----------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------
Cursor::~Cursor()
{

}
//-----------------------------------------------------------
// 移動
//-----------------------------------------------------------
void Cursor::Move()
{
	
	cursor_x = GetMouseX() ;
	cursor_y = GetMouseY();

	m_x = cursor_x ;
	m_y = cursor_y;

	move_x = cursor_x / MAP_SIZE * MAP_SIZE;
	move_y = cursor_y / MAP_SIZE * MAP_SIZE;

	if (m_x < 0)					m_x = 0;
	if (m_x > MAX_WINDOW_W - MAP_SIZE)	m_x = MAX_WINDOW_W - MAP_SIZE;
	if (m_y < 0)					m_y = 0;
	if (m_y > MAX_WINDOW_H - MAP_SIZE)	m_y = MAX_WINDOW_H - MAP_SIZE;
}
//-----------------------------------------------------------
// 描画
//-----------------------------------------------------------
void Cursor::Draw()
{
	DrawBox(move_x, move_y, move_x + MAP_SIZE, move_y + MAP_SIZE, RED, FALSE);
}
