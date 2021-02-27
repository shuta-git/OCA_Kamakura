#pragma once
#include "PlayerBase.h"

//-----------------------------------------------------------
// 定数
//-----------------------------------------------------------
#define MUSYA_SPEED   5.0F			// キャラの移動スピード
#define MUSYA_SIZE    64			// キャラのサイズ
#define MUSYA_X_SIZE  40			// キャラのXサイズ
#define MUSYA_Y_SIZE  48			// キャラのYサイズ
#define MUSYA_JUMP_POWER	8.0F   // キャラのジャンプ力
#define MUSYA_G       0.6F

//-----------------------------------------------------------
// クラス
//-----------------------------------------------------------
class Musya : public PlayerBase
{
public:
	Musya();
	Musya(float x_, float y_,bool dir_,float draw_x_);
	~Musya();
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
enum Musya_Dir {
	MUSYA_PTR_01 = 0,
	//MUSYA_PTR_02,
	//MUSYA_PTR_03,
	//MUSYA_PTR_04,

	MUSYA_PTR_MAX,
};
enum Musya_Draw {
	MUSYA_NORMAL = 0,
	MUSYA_DASH,

};













