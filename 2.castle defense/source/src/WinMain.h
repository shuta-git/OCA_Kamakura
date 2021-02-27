#pragma once

//---------------------------------------------
// 定数
//---------------------------------------------

#define PI		 3.141592653589793
#define RadToDeg 57.29577951f
#define R2D(rad) rad * RadToDeg

#define DegToRad 0.017453293f
#define D2R(deg) deg * DegToRad

#define BLACK	GetColor(0, 0, 0)
#define BLACKGRAY GetColor(90,90,90)
#define GRAY	GetColor(125,125,125)
#define WHITE	GetColor(255, 255, 255)
#define RED		GetColor(255, 0, 0)
#define GREEN	GetColor(0, 255, 0)
#define BLUE	GetColor(0, 0, 255)
#define YELLOW	GetColor(255, 255, 0)
#define PINK	GetColor(255, 0, 255)
#define SKYBLUE	GetColor(0, 255, 255)


#define WINDOW_MODE	TRUE
#define MOUSE_FLG	TRUE

#define WINDOW_W	1920
#define WINDOW_H	1080

//#if (NDEBUG)
//#define WINDOW_W	960
//#define WINDOW_H	540
//#else
//#define WINDOW_W	1920
//#define WINDOW_H	1080
//#endif


// 右	90度
#define DIR_RIGHT (DX_PI_F / 2)
// 左	270度
#define DIR_LEFT  (DX_PI_F + (DX_PI_F / 2))
// 上	0度
#define DIR_UP1 0.0f
// 上	360度
#define DIR_UP2 (DX_PI_F * 2)
// 下	180度
#define DIR_DOWN DX_PI_F 
// 右上 45度
#define DIR_RIGHT_UP (DX_PI_F / 4)
// 右下 135度
#define DIR_RIGHT_DOWN (DX_PI_F - (DX_PI_F / 4))
// 左上 315度
#define DIR_LEFT_UP ((DX_PI_F * 2 ) - (DX_PI_F / 4))
// 左下 225度
#define DIR_LEFT_DOWN (DX_PI_F + (DX_PI_F / 4))



enum ModelType
{
	PLAYER_MODEL = 0,	// プレイヤー
	//GROUND_MODEL,		// グラウンド
	CASTLE_MODEL,		// 城
	SPIDER_MODEL,		// 蜘蛛(エネミー1)
	ENEMY2_MODEL,		// (エネミー2)
	ENEMY3_MODEL,		// (エネミー3)
};

enum SceneType {
	TITLE = 0,	// タイトル
	SELECT,		// セレクト	
	LOAD,		// ロード
	GAME_MAIN,	// ゲームメイン
	OPERATION,	// 操作説明
	RANKING,	// ランキング
	PAUSE,		// ポーズ
	RESULT,		// リザルト
};
