#pragma once

#include "DxLib.h"
#include "KeyInput.h"
#include "GameMain.h"
#include "Map.h"
#include "Cursor.h"

#define BLACK	GetColor(0, 0, 0)
#define WHITE	GetColor(255, 255, 255)
#define RED		GetColor(255, 0, 0)
#define GREEN	GetColor(0, 255, 0)
#define BLUE	GetColor(0, 0, 255)
#define YELLOW	GetColor(255, 255, 0)
#define PINK	GetColor(255, 0, 255)
#define SKYBLUE	GetColor(0, 255, 255)

#define WINDOW_W	640
#define MAX_WINDOW_W 1920
#define WINDOW_H	480
#define MAX_WINDOW_H  480
#define MAX_DRAW_X 150


#define MAP_SIZE        32         // マップチップ一つのドットサイズ


//-----------------------------------------------------------
// スクリーン
//-----------------------------------------------------------
enum SCREEN {
	START = 0,
	CLEAR,
	GAMEOVER,
	OPERATION,
	PAUSE,
	CHARA_OPERATION,
	RANKING,
	TUTORIAL,
	SCORE,

	NOT_SCREEN,
};
//-----------------------------------------------------------
// ゲームモード
//-----------------------------------------------------------
enum MODE {
	GAME_MODE = 0,
	MAP_MODE,
	TUTORIAL_MODE,

	NOT_MODE,
};
//-----------------------------------------------------------
// キャラクター
//-----------------------------------------------------------
enum Character {
	FIONA = 0,
	MUSYA,
	NININ,

	MAX_CHARACTER,
};
//-----------------------------------------------------------
// キャラクター説明
//-----------------------------------------------------------
enum Character_Operation {
	FIONA_OPERATION = 0,
	MUSYA_OPERATION,
	NININ_OPERATION,

	NOT_OPERATION,
};


