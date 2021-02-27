#include "Fiona.h"
#include "WinMain.h"

//-----------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------
Fiona::Fiona()
{
	
}
//-----------------------------------------------------------
// 引数付きコンストラクタ
//-----------------------------------------------------------
Fiona::Fiona(float x_, float y_,bool dir_,float draw_x_)
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
Fiona::~Fiona()
{
	Exit();
}

//-----------------------------------------------------------
//　初期化処理 
//-----------------------------------------------------------
void Fiona::Init()
{
	ReloadGraph();

	ptr = 0;
	speed = FIONA_SPEED;
	gspd = FIONA_G;
	jspd = FIONA_JUMP_POWER;
}
//-----------------------------------------------------------
// 終了処理
//-----------------------------------------------------------
void Fiona::Exit()
{
	for (int i = 0; i < FIONA_PTR_MAX; i++) {
		DeleteGraph(image[i]);
	}
}
//-----------------------------------------------------------
// 更新処理
//-----------------------------------------------------------
void Fiona::Update()
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
void Fiona::Draw() {
	switch (SetDraw())
	{
	case FIONA_NORMAL:
		NormalDraw();
		break;
	case FIONA_DASH:
		DashDraw();
		break;
	default:
		break;
	}
	//printfDx("speed fiona %f", speed);
	//printfDx("draw_x fiona %f", draw_x);

}//-----------------------------------------------------------
// 画像の読み込み関数
//-----------------------------------------------------------
void Fiona::ReloadGraph()
{
	image[FIONA_PTR_01] = LoadGraph("data/player/fiona/fiona01.png");
	image[FIONA_PTR_02] = LoadGraph("data/player/fiona/fiona02.png");
	image[FIONA_PTR_03] = LoadGraph("data/player/fiona/fiona03.png");
	image[FIONA_PTR_04] = LoadGraph("data/player/fiona/fiona04.png");
}


//-----------------------------------------------------------
// ダッシュ描画
//-----------------------------------------------------------
void Fiona::DashDraw()
{
	if (dir) {
		DrawRectGraphF(x, y, (ptr + 3)* FIONA_X_SIZE, 2 * FIONA_Y_SIZE - 37,
			FIONA_X_SIZE, FIONA_Y_SIZE, image[FIONA_PTR_01], TRUE, FALSE);
	}
	else {
		DrawRectGraphF(x, y, (ptr + 3)* FIONA_X_SIZE, 2 * FIONA_Y_SIZE - 37,
			FIONA_X_SIZE, FIONA_Y_SIZE, image[FIONA_PTR_01], TRUE, TRUE);
	}
}
//-----------------------------------------------------------
// 通常時描画
//-----------------------------------------------------------
void Fiona::NormalDraw()
{
	if (dir) {
		DrawRectGraphF(x,y, ptr * FIONA_X_SIZE, 2 * FIONA_Y_SIZE - 37,
			FIONA_X_SIZE, FIONA_Y_SIZE, image[FIONA_PTR_01], TRUE, FALSE);
	}
	else {
		DrawRectGraphF(x, y, ptr * FIONA_X_SIZE, 2 * FIONA_Y_SIZE - 37,
			FIONA_X_SIZE, FIONA_Y_SIZE, image[FIONA_PTR_01], TRUE, TRUE);
	}
}
//-----------------------------------------------------------
// 何を描画するかを設定
//-----------------------------------------------------------
int Fiona::SetDraw()
{
	if ( CheckHitKey(KEY_INPUT_LSHIFT)) {
		return FIONA_DASH;
	}

	return FIONA_NORMAL;
}


