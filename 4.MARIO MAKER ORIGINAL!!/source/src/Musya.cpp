#include "Musya.h"
#include "WinMain.h"

//-----------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------
Musya::Musya()
{
}
//-----------------------------------------------------------
// 引数付きコンストラクタ
//-----------------------------------------------------------
Musya::Musya(float x_, float y_,bool dir_,float draw_x_)
{
	x = x_;
	y = y_;
	dir = dir_;
	draw_x = draw_x_;
	Init();
}
//-----------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------
Musya::~Musya()
{
	Exit();
}

//-----------------------------------------------------------
//　初期化処理 
//-----------------------------------------------------------
void Musya::Init()
{
	ReloadGraph();

	ptr = 0;
	speed = MUSYA_SPEED;
	gspd = MUSYA_G;
	jspd = MUSYA_JUMP_POWER;
}
//-----------------------------------------------------------
// 終了処理
//-----------------------------------------------------------
void Musya::Exit()
{
	for (int i = 0; i < MUSYA_PTR_MAX; i++) {
		DeleteGraph(image[i]);
	}
}
//-----------------------------------------------------------
// 更新処理
//-----------------------------------------------------------
void Musya::Update()
{
	SetOld();
	if (MoveCheckRange()) {
		Move();
	}
	Animation();
	Dir_Change();
	Jump();
	Gravity();
}
//-----------------------------------------------------------
// 描画処理
//-----------------------------------------------------------
void Musya::Draw()
{
	switch (SetDraw())
	{
	case MUSYA_NORMAL:
		NormalDraw();
		break;
	case MUSYA_DASH:
		DashDraw();
		break;
	default:
		break;
	}
}
//-----------------------------------------------------------
// 画像の読み込み関数
//-----------------------------------------------------------
void Musya::ReloadGraph()
{
	image[MUSYA_PTR_01] = LoadGraph("data/player/musya/musya01.png");
}


//-----------------------------------------------------------
// ダッシュ描画
//-----------------------------------------------------------
void Musya::DashDraw()
{
	if (dir) {
		DrawRectGraphF(x, y, (ptr + 3)* MUSYA_X_SIZE, 4 * MUSYA_Y_SIZE - 27,
			MUSYA_X_SIZE, MUSYA_Y_SIZE, image[MUSYA_PTR_01], TRUE, FALSE);
	}
	else {
		DrawRectGraphF(x, y, (ptr + 3)* MUSYA_X_SIZE, 4 * MUSYA_Y_SIZE - 27,
			MUSYA_X_SIZE, MUSYA_Y_SIZE, image[MUSYA_PTR_01], TRUE, TRUE);
	}
}
//-----------------------------------------------------------
// 通常時描画
//-----------------------------------------------------------
void Musya::NormalDraw()
{
	if (dir) {
		DrawRectGraphF(x, y, ptr * MUSYA_X_SIZE, 2 * MUSYA_Y_SIZE - 37,
			MUSYA_X_SIZE, MUSYA_Y_SIZE, image[MUSYA_PTR_01], TRUE, FALSE);
	}
	else {
		DrawRectGraphF(x,y, ptr * MUSYA_X_SIZE, 2 * MUSYA_Y_SIZE - 37,
			MUSYA_X_SIZE, MUSYA_Y_SIZE, image[MUSYA_PTR_01], TRUE, TRUE);
	}
}
//-----------------------------------------------------------
// 何を描画するかを設定
//-----------------------------------------------------------
int Musya::SetDraw()
{
	if (CheckHitKey(KEY_INPUT_LSHIFT)) {
		return MUSYA_DASH;
	}

	return MUSYA_NORMAL;
}
