#pragma once
#include "Main.h"

class PauseScreen
{
private:
	bool start;
	bool operation;
	bool game;
	bool spawn;
	int pause_num;

public:
	PauseScreen();
	~PauseScreen();

	void Init();
	void Update();
	void Draw(Font font, Font font1);
	void Exit();

	bool GetIsStart() const { return start; }
	bool GetIsOperation() const { return operation; }
	bool GetIsGame() const { return game; }
	bool GetIsSpawn() const { return spawn; }

private:

	RectF GetGame() const;
	RectF GetOperation() const;
	RectF GetSpawn() const;
	RectF GetStart() const;

	void KeySelect();
	void ConSelect();
	void SelectChange();

	void DrawMouse(Font font, Font font1);
	void DrawCon(Font font, Font font1);

};

