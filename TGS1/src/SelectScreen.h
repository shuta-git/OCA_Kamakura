#pragma once
#include "Main.h"

class SelectScreen
{
private:
	bool operation;
	int map_num;
	int select_num;

public:
	SelectScreen();
	~SelectScreen();

	void Init();
	void Update();
	void Draw(Font font,Font font1);
	void Exit();

	bool GetIsOperation() const { return operation; }
	int GetMapNum()const { return map_num; }

private:

	RectF GetEasy() const;
	RectF GetNormal() const;
	RectF GetHard() const;
	RectF GetOperation() const;

	void SelectChange();
	void KeySelect();
	void ConSelect();

	void DrawMouse(Font font, Font font1);
	void DrawCon(Font font, Font font1);
};

