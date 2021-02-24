#pragma once
#include "PlayerBase.h"

//-----------------------------------------------------------
// 定数
//-----------------------------------------------------------
#define NININ_SPEED   5.0F			// キャラの移動スピード
#define NININ_SIZE    64			// キャラのサイズ
#define NININ_X_SIZE  40			// キャラのXサイズ
#define NININ_Y_SIZE  48			// キャラのYサイズ
#define NININ_JUMP_POWER	9.0F   // キャラのジャンプ力
#define NININ_G       0.6F

//-----------------------------------------------------------
// クラス
//-----------------------------------------------------------
class Ninin : public PlayerBase
{
public:
	Ninin();
	Ninin(float x_, float y_, bool dir_,float draw_x_);
	~Ninin();
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
enum Ninin_Dir {
	NININ_PTR_01 = 0,
	//MUSYA_PTR_02,
	//MUSYA_PTR_03,
	//MUSYA_PTR_04,

	NININ_PTR_MAX,
};
enum Ninin_Draw {
	NININ_NORMAL = 0,
	NININ_DASH,

};
