#include "WinMain.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{	

	SetOutApplicationLogValidFlag(FALSE);
	ChangeWindowMode(TRUE);

	SetGraphMode(WINDOW_W, WINDOW_H, 32);

	SetBackgroundColor(125, 125, 125);

	SetMainWindowText("MARIO MAKER ORIGINAL!!");
	if (DxLib_Init() == -1) { return -1; }

	SetDrawScreen(DX_SCREEN_BACK);

	// ---------------
	// 初期化処理
	// ---------------
	KeyInputInit();

	int number = START;
	int mode_num = NOT_MODE;
	int chara_num = NOT_OPERATION;


	GameMain* gamemain = new GameMain();
	Cursor * cur = new Cursor();
	

	gamemain->Init();

	while (ProcessMessage() == 0 && !IsKeyOn(KEY_INPUT_ESCAPE) && KeyInputUpdate() == 0)
	{
		// ---------------
		// 更新処理
		// ---------------
		int old_num = number;


		// ---------------
		// 生存確認
		// ---------------
		if (gamemain->GetAlive()) {
			number = GAMEOVER;
		}
		if (gamemain->GetClear()) {
			number = CLEAR;
		}
		// ---------------
		// スタート
		// ---------------
		if (number == START) {
			if (cur != nullptr) {
				cur->Move();
				if (CheckMouseInput()) {
					// ゲーム
					if (cur->GetX() >= 60 && cur->GetY() >= 150 &&
						cur->GetX() <= 260 && cur->GetY() <= 250) {
						number = NOT_SCREEN;
					}
					// マップビルダー
					if (cur->GetX() >= 380 && cur->GetY() >= 150 &&
						cur->GetX() <= 580 && cur->GetY() <= 250) {
						number = NOT_SCREEN;
					}
					// 操作説明
					if (cur->GetX() >= 60 && cur->GetY() >= 290 &&
						cur->GetX() <= 260 && cur->GetY() <= 350) {
						number = OPERATION;
					}
					// キャラクター説明
					if (cur->GetX() >= 60 && cur->GetY() >= 380 &&
						cur->GetX() <= 260 && cur->GetY() <= 440) {
						number = CHARA_OPERATION;
					}
					// ランキング
					if (cur->GetX() >= 380 && cur->GetY() >= 290 &&
						cur->GetX() <= 580 && cur->GetY() <= 350) {
						number = RANKING;
					}
					// チュートリアル
					if (cur->GetX() >= 380 && cur->GetY() >= 380 &&
						cur->GetX() <= 580 && cur->GetY() <= 440) {
						number = NOT_SCREEN;
					}
				}
			}
		}
		// ---------------
		// スコア
		// ---------------
		if (number == SCORE) {
			if (IsKeyOn(KEY_INPUT_TAB)) {
				number = START;
			}
		}
		// ---------------
		// クリア
		// ---------------
		if (number == CLEAR) {
			if (IsKeyOn(KEY_INPUT_TAB)) {
				number = SCORE;
				mode_num = NOT_MODE;
			}
		}
		// ---------------
		// ゲームオーバー
		// ---------------
		if (number == GAMEOVER) {
			if (IsKeyOn(KEY_INPUT_TAB)) {
				number = START;
				mode_num = NOT_MODE;
			}
		}
		// ---------------
		// 操作説明
		// ---------------
		if (number == OPERATION) {
			if (mode_num == NOT_MODE) {
				if (IsKeyOn(KEY_INPUT_TAB)) {
					number = START;
				}
			}
			if (mode_num == GAME_MODE) {
				if (IsKeyOn(KEY_INPUT_TAB)) {
					number = PAUSE;
				}
			}
		}
		// ---------------
		// キャラクター説明
		// ---------------
		if (number == CHARA_OPERATION && chara_num == NOT_OPERATION) {
			if (cur != nullptr) {
				cur->Move();
				if (CheckMouseInput()) {
					// fiona
					if (cur->GetX() >= 49 && cur->GetY() >= 149 &&
						cur->GetX() <= 191 && cur->GetY() <= 351) {
						chara_num = FIONA_OPERATION;
					}
					// musya
					if (cur->GetX() >= 249 && cur->GetY() >= 149 &&
						cur->GetX() <= 391 && cur->GetY() <= 351) {
						chara_num = MUSYA_OPERATION;
					}
					// ninin
					if (cur->GetX() >= 449 && cur->GetY() >= 149 &&
						cur->GetX() <= 591 && cur->GetY() <= 351) {
						chara_num = NININ_OPERATION;
					}
				}
				if (IsKeyOn(KEY_INPUT_TAB)) {
					number = START;
				}
			}
		}
		// ---------------
		// キャラクター説明 fiona
		// ---------------
		if (chara_num == FIONA_OPERATION) {
			if (IsKeyOn(KEY_INPUT_TAB)) {
				number = CHARA_OPERATION;
				chara_num = NOT_OPERATION;
			}
		}
		// ---------------
		// キャラクター説明 musya
		// ---------------
		if (chara_num == MUSYA_OPERATION) {
			if (IsKeyOn(KEY_INPUT_TAB)) {
				number = CHARA_OPERATION;
				chara_num = NOT_OPERATION;
			}
		}
		// ---------------
		// キャラクター説明 ninin
		// ---------------
		if (chara_num == NININ_OPERATION) {
			if (IsKeyOn(KEY_INPUT_TAB)) {
				number = CHARA_OPERATION;
				chara_num = NOT_OPERATION;
			}
		}
		// ---------------
		// ランキング
		// ---------------
		if (number == RANKING) {
			if (IsKeyOn(KEY_INPUT_TAB)) {
				number = START;
			}
		}
		// ---------------
		// チュートリアル
		// ---------------
		if (number == TUTORIAL) {
			if (IsKeyOn(KEY_INPUT_TAB)) {
				number = START;
				mode_num = NOT_MODE;
			}
		}
		// ---------------
		// pause
		// ---------------
		if (number == PAUSE) {
			if (cur != nullptr) {
				cur->Move();
				if (CheckMouseInput()) {
					// 戻る
					if (cur->GetX() >= 290 && cur->GetY() >= 100 &&
						cur->GetX() <= 340 && cur->GetY() <= 125) {
						number = NOT_SCREEN;
					}
					// マップビルダー
					if (cur->GetX() >= 230 && cur->GetY() >= 150 &&
						cur->GetX() <= 413 && cur->GetY() <= 175) {
						number = NOT_SCREEN;
						SetGraphMode(MAX_WINDOW_W, MAX_WINDOW_H, 32);
						mode_num = MAP_MODE;
						gamemain->ReloadGraph();
					}
					// スタート画面に戻る
					if (cur->GetX() >= 200 && cur->GetY() >= 200 &&
						cur->GetX() <= 441 && cur->GetY() <= 225) {
						number = START;
						mode_num = NOT_MODE;
					}
					// 操作説明
					if (cur->GetX() >= 270 && cur->GetY() >= 250 &&
						cur->GetX() <= 370 && cur->GetY() <= 275) {
						number = OPERATION;
					}
					// リスポーン
					if (cur->GetX() >= 255 && cur->GetY() >= 300 &&
						cur->GetX() <= 388 && cur->GetY() <= 325) {
						number = NOT_SCREEN;
					}
				}
			}
		}
		// ---------------
		// ゲーム中のpause
		// ---------------
		if (number == NOT_SCREEN && mode_num == GAME_MODE) {
			if (IsKeyOn(KEY_INPUT_TAB)) {
				number = PAUSE;
			}
		}
		// ---------------
		// スタート画面
		// ---------------
		if (mode_num == NOT_MODE && old_num  == START) {
			if (cur != nullptr) {
				cur->Move();
				if (CheckMouseInput()) {
					// ゲームスタート
					if (cur->GetX() >= 60 && cur->GetY() >= 150 &&
						cur->GetX() <= 260 && cur->GetY() <= 250) {
						SetGraphMode(WINDOW_W, WINDOW_H, 32);
						mode_num = GAME_MODE;
					}
					// マップビルダー
					if (cur->GetX() >= 380 && cur->GetY() >= 150 &&
						cur->GetX() <= 580 && cur->GetY() <= 250) {
						SetGraphMode(MAX_WINDOW_W, MAX_WINDOW_H, 32);
						mode_num = MAP_MODE;
					}
					// チュートリアル
					if (cur->GetX() >= 380 && cur->GetY() >= 380 &&
						cur->GetX() <= 580 && cur->GetY() <= 440) {
						number = TUTORIAL;
						SetGraphMode(WINDOW_W, WINDOW_H, 32);
						mode_num = TUTORIAL_MODE;
					}
					// 画像の読み直し	
					gamemain->ReloadGraph();
				}
			}
		}
		// ---------------
		// マップビルダー
		// ---------------
		if (mode_num == MAP_MODE) {
			if(IsKeyOn(KEY_INPUT_TAB)) {
				SetGraphMode(WINDOW_W, WINDOW_H, 32);
				mode_num = GAME_MODE;
				gamemain->ReloadGraph();
			}
		}


		gamemain->Update(number,mode_num,old_num, chara_num);

		// ---------------
		// 描画処理
		// ---------------
		ClearDrawScreen();

		clsDx();
		gamemain->Draw();
		ScreenFlip();
	}

	// ---------------
	// 終了処理
	// ---------------
	KeyInputExit();
	gamemain->Exit();
	
	delete gamemain;

	DxLib_End();

	return 0;
}

