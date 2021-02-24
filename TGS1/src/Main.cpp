#include "Main.h"
#include "GameMain.h"
#include "StartScreen.h"
#include "SelectScreen.h"
#include "ClearScreen.h"
#include "GameOverScreen.h"
#include "PauseScreen.h"
#include "OperationScreen.h"

void Main()
{
	int screen_num = START_SCREEN;
	int old_screen_num = SCREEN_COUNT;
	int map_number = MAP_COUNT;

	const Font font(15);
	const Font pause_font(50);
	const Font pause_font1(60);

	const Font select_font(70);
	const Font select_font1(90);

#if! defined(NDEBUG)
#else
#endif

	Window::Resize(WINDOW_W, WINDOW_H);


	GameMain* gamemain = nullptr; 
	StartScreen* startscreen = nullptr;
	SelectScreen* selectscreen = nullptr;
	PauseScreen* pausescreen = nullptr;
	OperationScreen* operationscreen = nullptr;
	GameOverScreen* gameoverscreen = nullptr;
	ClearScreen* clearscreen = nullptr;
	

	while (System::Update())
	{	
		

		switch (screen_num)
		{
			//------------------------------------------------------
			// スタート画面
			//------------------------------------------------------
		case START_SCREEN:
			if (startscreen == nullptr) {
				startscreen = new StartScreen();
			}
			else {
#if! defined(NDEBUG)
				font(L"スタート画面").draw();
#else
#endif
				startscreen->Update();
				startscreen->Draw(pause_font, select_font1);
				// セレクト画面に進む（Aボタンまたは SPACE キー）
				if (startscreen->GetStart()) {
					// 一つ前の screen_num を保存
					old_screen_num = screen_num;
					screen_num = SELECT_SCREEN;
				}
			}
			// デリート処理
			if (screen_num != START_SCREEN) {
				delete startscreen;
				startscreen = nullptr;
			}
			break;
			//------------------------------------------------------
			// セレクト画面
			//------------------------------------------------------
		case SELECT_SCREEN:
			if (selectscreen == nullptr) {
				selectscreen = new SelectScreen();
			}
			else {
				selectscreen->Update();
				selectscreen->Draw(select_font, select_font1);
				// マップEASY
				if (selectscreen->GetMapNum() == MAP_ONE) {
					map_number = MAP_ONE;
					screen_num = GAMEMAIN_SCREEN;
				}
				// マップNORMAL
				if (selectscreen->GetMapNum() == MAP_TWO) {
					map_number = MAP_TWO;
					screen_num = GAMEMAIN_SCREEN;
				}
				// マップHARD
				if (selectscreen->GetMapNum() == MAP_THREE) {
					map_number = MAP_THREE;
					screen_num = GAMEMAIN_SCREEN;
				}
				// 操作説明
				if (selectscreen->GetIsOperation() == true) {
					// 一つ前の screen_num を保存
					old_screen_num = screen_num;
					screen_num = OPERATION_SCREEN;
				}
				// Spaceキーを押したらスタート画面に戻る
				if (Input::KeySpace.clicked) {
					screen_num = START_SCREEN;
				}
				// デリート処理
				if (screen_num != SELECT_SCREEN) {
					delete selectscreen;
					selectscreen = nullptr;
				}
			}
			break;
			//------------------------------------------------------
			// ゲーム画面
			//------------------------------------------------------
		case GAMEMAIN_SCREEN:
			if (gamemain == nullptr) {
				gamemain = new GameMain(map_number);
			}
			else {
#if! defined(NDEBUG)
				font(L" [0　：　▲]　[1　：　▼]　[2　：　▶]　[3　：　◀]　[4　：　▲↑]　[5　：　▼↓]　[6　：　▆]").draw();
				font(L"Pキーでポーズ画面").draw(5,30);
#else
#endif			
			

				// ポーズ画面
				if (Input::KeyP.clicked || controller.buttonStart.clicked) {
					screen_num = PAUSE_SCREEN;
				}
				// ゲームオーバー
				if (gamemain->GetIsPlayerDeath()){
					screen_num = GAMEOVER_SCREEN;
				}
				// ゲームクリア
				if (gamemain->GetIsGoal()) {
					screen_num = CLEAR_SCREEN;
				}
				// ポーズ画面時描画をやめる
				if (screen_num != PAUSE_SCREEN || screen_num != GAMEOVER_SCREEN) {
					gamemain->Draw();
				}
				
				// 操作説明画面かポーズ画面じゃないとき
				if (screen_num != PAUSE_SCREEN &&
					screen_num != OPERATION_SCREEN&&
					screen_num != GAMEOVER_SCREEN) {
					gamemain->Update();
					if (controller.isConnected()){
#if! defined(NDEBUG)
#else
						pause_font(L"startボタンでpause").draw(0, 0);
#endif	
					}
					if (map_number == MAP_ONE) {
						if (gamemain->GetPlayerPosX() < CAMERA_MIN_X) {
							pause_font(L"左スティックで移動").draw(280, 200);
							pause_font(L"Aボタンでジャンプ").draw(1200, 200);

						}
						if (gamemain->GetPlayerPosX() > CAMERA_MAX_X) {
							pause_font(L"注意！").draw(0, 200, Palette::Red);
							pause_font(L"注意！").draw(360, 200, Palette::Red);
							pause_font(L"注意！").draw(710, 200, Palette::Red);

						}
						if (gamemain->GetPlayerPosX() > CAMERA_MIN_X && gamemain->GetPlayerPosX() < CAMERA_MAX_X) {

							pause_font(L"左スティックで移動").draw(280 - (gamemain->GetPlayerPosX() - 360), 200);
							pause_font(L"Aボタンでジャンプ").draw(1200 - (gamemain->GetPlayerPosX() - 360), 200);
							pause_font(L"注意！").draw(2000 - (gamemain->GetPlayerPosX() - 360), 200, Palette::Red);
							pause_font(L"注意！").draw(2360 - (gamemain->GetPlayerPosX() - 360), 200, Palette::Red);
							pause_font(L"注意！").draw(2710 - (gamemain->GetPlayerPosX() - 360), 200, Palette::Red);

						}
					}
					// ゲーム画面じゃない時
					if (screen_num != GAMEMAIN_SCREEN) {
						// ゲーム画面終了
						// デリート処理
						delete gamemain;
						gamemain = nullptr;
						map_number = MAP_COUNT;
					}
				}
				
			}
			break;
			//------------------------------------------------------
			// ゲームオーバー画面
			//------------------------------------------------------
		case GAMEOVER_SCREEN:
			if (gameoverscreen == nullptr) {
				gameoverscreen = new GameOverScreen();
				
			}
			else {
#if! defined(NDEBUG)
				font(L"ゲームオーバー画面").draw();
#else
#endif

				gameoverscreen->Update();
				gamemain->Draw();
				gameoverscreen->Draw(pause_font, select_font1);
				

				if (gameoverscreen->GetDeath()) {
					if (Input::KeySpace.clicked || controller.buttonA.clicked) {
						screen_num = GAMEMAIN_SCREEN;
						if (gamemain != nullptr) {
							delete gamemain;
							gamemain = nullptr;
						}
					}
				}
				if (screen_num != GAMEOVER_SCREEN) {
					delete gameoverscreen;
					gameoverscreen = nullptr;
				}
			}
			break;
			//------------------------------------------------------
			// 操作説明画面
			//------------------------------------------------------
		case OPERATION_SCREEN:
			if (operationscreen == nullptr) {
				operationscreen = new OperationScreen();
			}
			else {
#if! defined(NDEBUG)
				font(L"操作説明画面").draw();
#else
#endif
				operationscreen->Draw(select_font, select_font1);

				// spaceキーまたはAボタンを押したら一つ前に戻る
				if (Input::KeySpace.clicked || controller.buttonA.clicked) {
					screen_num = old_screen_num;
				}
				if (screen_num != OPERATION_SCREEN) {
					delete operationscreen;
					operationscreen = nullptr;
				}
			}
			break;
			//------------------------------------------------------
			// クリア画面
			//------------------------------------------------------
		case CLEAR_SCREEN:
			if (clearscreen == nullptr) {
				clearscreen = new ClearScreen();
			}
			else {
#if! defined(NDEBUG)
				font(L"クリア画面").draw();
#else
#endif

				clearscreen->Draw(select_font, select_font1);

				// spaceキーまたはAボタンを押したら一つ前に戻る
				if (Input::KeySpace.clicked || controller.buttonA.clicked) {
					screen_num = START_SCREEN;
				}
			}
			break;
			//------------------------------------------------------
			// ポーズ画面
			//------------------------------------------------------
		case PAUSE_SCREEN:
			if (pausescreen == nullptr) {
				pausescreen = new PauseScreen();
			}
			else {
#if! defined(NDEBUG)
				font(L"ポーズ画面").draw();
#else
#endif
				
				// ゲーム画面
				if (pausescreen->GetIsGame()) {
					screen_num = GAMEMAIN_SCREEN;
				}
				// 操作説明画面ではないとき
				if (screen_num != OPERATION_SCREEN ) {
					pausescreen->Update();
					gamemain->Draw();
					pausescreen->Draw(pause_font, pause_font1);
				}
				// ポーズ画面
				if (pausescreen->GetIsSpawn()) {
					screen_num = GAMEMAIN_SCREEN;
					if (gamemain != nullptr) {
						delete gamemain;
						gamemain = nullptr;
					}
				}
				// スタート画面
				if (pausescreen->GetIsStart()) {
					screen_num = START_SCREEN;
					if (gamemain != nullptr) {
						delete gamemain;
						gamemain = nullptr;
					}
				}
				// 操作説明画面
				if (pausescreen->GetIsOperation()) {
					old_screen_num = PAUSE_SCREEN;
					screen_num = OPERATION_SCREEN;
				}
				// デリーと処理
				if (screen_num != PAUSE_SCREEN ) {
					delete pausescreen;
					pausescreen = nullptr;
				}
			}
			break;
		default:
			break;
		}

	
	}

}




