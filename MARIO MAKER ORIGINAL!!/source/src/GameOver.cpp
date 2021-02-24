#include "WinMain.h"
#include "GameOver.h"


GameOver::GameOver()
{
	image[0] = LoadGraph("data/gameover/gameover.png");
}

GameOver::~GameOver()
{
}

void GameOver::Update()
{
}

void GameOver::Draw()
{
	DrawGraph(-10,-20,image[0],TRUE);
	SetFontSize(25);
	DrawString(340,450,"TABでスタート画面に戻る",GetColor(255,0,0),TRUE);
}

void GameOver::ReloadGraph()
{
	image[0] = LoadGraph("data/gameover/ganeover.png");
}









