#pragma once

class Cursor
{
	int m_x, m_y;
	int m_size;

	int cursor_x;
	int cursor_y;

	int move_x = 0;
	int move_y = 0;

public:
	Cursor(int x_ = 320,int y_ = 320);
	virtual ~Cursor();

	void Move();
	void Draw();
	int GetX() const { return m_x; }
	int GetY() const { return m_y; }
};

