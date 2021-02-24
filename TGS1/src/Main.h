#pragma once
#include <Siv3D.hpp>
#include <HamFramework.hpp>
#include <rnfs.h>


// ウィンドウのサイズ
#if ! defined(NDEBUG)
#define WINDOW_W 1920
#define WINDOW_H 1080
#else
#define WINDOW_W 1920
#define WINDOW_H 1080
#endif

// ブロックのサイズ
#define MAP_SIZE 40

// ブロックの数
#define BLOCK_W 100
#define BLOCK_H 25

// マップの端
#define MAX_W MAP_SIZE*BLOCK_W
#define MAX_H MAP_SIZE*BLOCK_H

// カメラの範囲
#define CAMERA_MIN_X 360
#define CAMERA_MAX_X 2360
#define CAMERA_MIN_Y 780

// プレイヤー
#define PLAYER_ROT 30
#define PLAYER_POS_X 200
#define PLAYER_PO_Y  100

// ドロップトラップの落ちる速度
#define TRAP_DROP_SPEED 5.0

// コントローラー
static XInput controller = XInput(0);


//----------------
// タスク用
//----------------
enum CallGroup
{
	CallGroup_Update,
	CallGroup_Draw,
};
enum CallPriority
{
	CallPriority_Map,     //最初に描画が行われる
	CallPriority_Trap,
	CallPriority_Player,    //その後に描画が行われるので、プレイヤーが隠れない

};

//----------------
// トラップ種類
//----------------
enum Type_
{
	TRIANGLE_UP,
	TRIANGLE_DOWN,
	TRIANGLE_RIGHT,
	TRIANGLE_LEFT,
	TRIANGLE_OUT,
	TRIANGLE_DROP,
	BLOCK,
	TRIANGLE_OUT_1,
};

enum Scene_
{
	START_SCREEN,			// スタート画面
	SELECT_SCREEN,			// セレクト画面
	GAMEMAIN_SCREEN,		// ゲーム画面
	GAMEOVER_SCREEN,		// ゲームオーバー画面
	OPERATION_SCREEN,		// 操作説明画面
	CLEAR_SCREEN,			// クリア画面
	PAUSE_SCREEN,			// ポーズ画面

	SCREEN_COUNT,
};

enum Map_Num_
{
	MAP_ONE,
	MAP_TWO,
	MAP_THREE,

	MAP_COUNT,
};

enum Select_
{
	SELECT_OPERATION,
	SELECT_EASY,
	SELECT_NORMAL,
	SELECT_HARD,

	SELECT_COUNT,
};

enum Pause_
{
	PAUSE_GAME,
	PAUSE_OPERATION,
	PAUSE_SPAWN,
	PAUSE_START,

	PAUSE_COUNT,
};