#include "WinMain.h"
#include "Screen.h"
#include "StartScreen.h"
#include "ClearScreen.h"
#include "GameOver.h"
#include "OperationScreen.h"
#include "PauseScreen.h"
#include "CharaOperation.h"
#include "RankingScreen.h"
#include "ScoreScreen.h"


//-----------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------
Screen::Screen()
{
}
//-----------------------------------------------------------
// 引数付きコンストラクタ
//-----------------------------------------------------------
Screen::Screen(int num_, int chara_num_)
{
	if (screenbase != nullptr) {
		screenbase = nullptr;
	}
	chara_num = chara_num_;
	number = num_;
	old_number = number;
}

//-----------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------
Screen::~Screen()
{
	Exit();
}
//-----------------------------------------------------------
// 終了処理
//-----------------------------------------------------------
void Screen::Exit()
{
	if (screenbase != nullptr) {
		delete screenbase;
		screenbase = nullptr;
	}
}
//-----------------------------------------------------------
// 更新処理
//-----------------------------------------------------------
void Screen::Update(int num_, int chara_num_)
{
	number = num_;
	chara_num = chara_num_;
	Screen_Delete();
	old_number = number;
	Screen_Set();
	if (screenbase != nullptr) {
		screenbase->Update(chara_num);
		screenbase->Update();
	}
}
//-----------------------------------------------------------
// 描画処理
//-----------------------------------------------------------
void Screen::Draw()
{
	if (screenbase != nullptr) {
		screenbase->Draw();
	}
}
//-----------------------------------------------------------
//-----------------------------------------------------------
void Screen::ReloadGraph()
{
	if (screenbase != nullptr) {
		screenbase->ReloadGraph();
	}
}
//-----------------------------------------------------------
// playerの値をセット
//-----------------------------------------------------------
void Screen::SetPlayer(Player * player, double timer_)
{
	score = player->GetScore();
	timer = timer_;
}
//-----------------------------------------------------------
//-----------------------------------------------------------
void Screen::Screen_Set()
{
	switch (number)
	{
	case START:
		if (screenbase == nullptr) {
			screenbase = new StartScreen();
		}
		break;
	case CLEAR:
		if (screenbase == nullptr) {
			screenbase = new ClearScreen();
		}
		break;
	case GAMEOVER:
		if (screenbase == nullptr) {
			screenbase = new GameOver();
		}
		break;
	case OPERATION:
		if (screenbase == nullptr) {
			screenbase = new OperationScreen();
		}
		break;
	case PAUSE:
		if (screenbase == nullptr) {
			screenbase = new PauseScreen();
		}
		break;
	case CHARA_OPERATION:
		if (screenbase == nullptr) {
			screenbase = new CharaOperation(chara_num);
		}
		break;
	case RANKING:
		if (screenbase == nullptr) {
			screenbase = new RankingScreen();
		}
		break;
	case SCORE:
		if (screenbase == nullptr) {
			screenbase = new ScoreScreen(score,timer);
		}
		break;
	default:
		break;
	}
}
//-----------------------------------------------------------
//-----------------------------------------------------------
void Screen::Screen_Delete()
{
	if (number != old_number) {
		Exit();
	}
}