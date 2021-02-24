#include "WinMain.h"

//-----------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------
GameMain::GameMain()
{
	pause = false;
	alive = false;
	clear = false;
	timer = 60.00;
	map = nullptr;
	player = nullptr;
	screen = nullptr;
}
//-----------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------
GameMain::~GameMain()
{
	Exit();
	if (map != nullptr) {
		delete map;
		map = nullptr;
	}
	if (player != nullptr) {
		delete player;
		player = nullptr;
	}
	if (screen != nullptr) {
		delete screen;
		screen = nullptr;
	}
	if (cur != nullptr) {
		delete cur;
		cur = nullptr;
	}
}

//-----------------------------------------------------------
//	初期化
//-----------------------------------------------------------
void GameMain::Init()
{
	if (cur == nullptr){
		cur = new Cursor();
	}
}
//-----------------------------------------------------------
//	更新
//-----------------------------------------------------------
void GameMain::Update(int number_, int mode_num_, int old_num_, int chara_num_)
{
	number = number_;
	old_num = old_num_;
	mode_num = mode_num_;
	chara_num = chara_num_;

	// pause;
	Pause();

	// スクリーン
	if (screen == nullptr) {
		screen = new Screen(number, chara_num);
	}
	if (screen != nullptr) {
		screen->Update(number, chara_num_);
	}


	if (number == GAMEOVER || number == CLEAR || old_num == TUTORIAL && mode_num == NOT_MODE) {
		ResetTimer();
		if (map != nullptr) {
			delete map;
			map = nullptr;
		}
	}
	//	ポーズ
	if (pause) {
		return;
	}

	if (old_num == PAUSE) {
		if (cur != nullptr) {
			cur->Move();
			if (CheckMouseInput()) {
				// リスポーン
				if (cur->GetX() >= 255 && cur->GetY() >= 300 &&
					cur->GetX() <= 388 && cur->GetY() <= 325) {
					if (player != nullptr) {
						delete player;
						player = nullptr;
					}
					ResetTimer();
				}
			}
		}
		// マップビルダー
		if (number == START || number == NOT_SCREEN && mode_num == MAP_MODE ) {
			if (map != nullptr) {
				delete map;
				map = nullptr;
			}
			ResetTimer();
		}
	}

	// マップ
	if (map == nullptr) {
		map = new MAP();
	}
	if (map != nullptr) {
		map->Update(number, mode_num, old_num);
	}

	 
	// ゲーム中  チュートリアル
	if (mode_num == GAME_MODE || mode_num == TUTORIAL_MODE) {
		// ゲーム中のタイマー
		if (timer <= 0.00) {
			alive = true;
		}
		if (player == nullptr) {
			player = new Player();
			map->Init();
		}
		if (number != TUTORIAL) {
			timer -= 0.02;
			if (player->GetDrawX() >= MAP_WIDTH_MAX * MAP_SIZE - 255) {
				clear = true;
				player->TimerScore(timer);
				if (screen != nullptr) {
					screen->SetPlayer(player, timer);
				}
			}
		}
		if (player != nullptr) {
			player->Update();
		}
		if (player->GetPosY() >= MAX_WINDOW_H) {
			alive = true;
		}
		if (map != nullptr) {
			if (player != nullptr) {
				map->SetPlayer(player);
				if (player->Check_Map_Range()) {
					map->ResetPosX();
					map->ResetMoveX();
				}
			}
		}
	}
	else{
		if (player != nullptr) {
			delete player;
			player = nullptr;
			alive = false;
			clear = false;
		}
	}
}
//-----------------------------------------------------------
//	描画
//-----------------------------------------------------------
void GameMain::Draw()
{
	if (map != nullptr) {
		map->Draw();
	}
	if (mode_num == GAME_MODE || mode_num == TUTORIAL_MODE) {
		if (player != nullptr) {
			player->Draw();
		}
		if (number != TUTORIAL) {
			SetFontSize(30);
			DrawFormatString(280, 30, GetColor(255, 255, 255), "%0.2f", timer);
			SetFontSize(20);
			DrawFormatString(15, 15, GetColor(255, 255, 255), "スコア ： %0.2f", player->GetScore());
		}
	}
	if (screen != nullptr) {
		screen->Draw();
	}
}
//-----------------------------------------------------------
//	終了
//-----------------------------------------------------------
void GameMain::Exit()
{
	if (map != nullptr) {
		map->Exit();
	}
	if (player != nullptr) {
		player->Exit();
	}
	if (screen != nullptr) {
		screen->Exit();
	}
}
//-----------------------------------------------------------
// 画像の読み直し関数
//-----------------------------------------------------------
void GameMain::ReloadGraph()
{
	if (map != nullptr) {
		map->ReloadGraph();
	}
	if (player != nullptr) {
		player->ReloadGraph();
	}
}

//-----------------------------------------------------------
// pause
//-----------------------------------------------------------
void GameMain::Pause()
{
	// pause条件
	if (number == PAUSE || mode_num == GAME_MODE && number == OPERATION || number == GAMEOVER || number == CLEAR) {
		pause = true;
	}
	else {
		pause = false;
	}
}
//-----------------------------------------------------------
// リセットタイム
//-----------------------------------------------------------
void GameMain::ResetTimer()
{
	timer = 60.00;
}



