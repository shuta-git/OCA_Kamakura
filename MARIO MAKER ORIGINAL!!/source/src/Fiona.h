#pragma once
#include "PlayerBase.h"

//-----------------------------------------------------------
// 定数
//-----------------------------------------------------------
#define FIONA_SPEED   5.0F			// キャラの移動スピード
#define FIONA_SIZE    64			// キャラのサイズ
#define FIONA_X_SIZE  40			// キャラのXサイズ
#define FIONA_Y_SIZE  48			// キャラのYサイズ
#define FIONA_JUMP_POWER	10.0F   // キャラのジャンプ力
#define FIONA_G       0.6F

//-----------------------------------------------------------
// クラス
//-----------------------------------------------------------
class Fiona : public PlayerBase
{
public:
	Fiona();
	Fiona(float x_, float y_,bool dir_,float draw_x_);
	~Fiona();
	void Init() override;				// 初期化処理
	void Exit() override;				// 終了処理
	void Update() override;				// 更新処理
	void Draw() override;				// 描画処理
	void ReloadGraph() override;		// 画像の読み込み関数

private:
	void DashDraw() override;
	void NormalDraw() override;
	int SetDraw() override;

	
};

//-----------------------------------------------------------
// 列挙体
//-----------------------------------------------------------
enum Fiona_Dir {
	FIONA_PTR_01 = 0,
	FIONA_PTR_02,
	FIONA_PTR_03,
	FIONA_PTR_04,

	FIONA_PTR_MAX,
};
enum Fiona_Draw {
	FIONA_NORMAL = 0,
	FIONA_DASH ,
	
};








