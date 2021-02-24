#include "PlayerBase.h"
#include "WinMain.h"

//-----------------------------------------------------------
// コンストラクト
//-----------------------------------------------------------
PlayerBase::PlayerBase()
{
	gspd = 0.0f;
	jspd = 0.0f;
}
//-----------------------------------------------------------
// デストラクト
//-----------------------------------------------------------
PlayerBase::~PlayerBase()
{
}

//-----------------------------------------------------------
// ジャンプ
//-----------------------------------------------------------
void PlayerBase::Jump()
{
	if (CheckHitKey(KEY_INPUT_LSHIFT)) {
		if (IsKeyOn(KEY_INPUT_SPACE) && jflag == false) {
			move_y = -jspd * JUMP_UP;
			jflag = true;
		}
	}
	else {
		if (IsKeyOn(KEY_INPUT_SPACE) && jflag == false) {
			jflag = true;
			move_y = -jspd;
		}
	}

	
}
//-----------------------------------------------------------
// 重力
//-----------------------------------------------------------
void PlayerBase::Gravity()
{
	move_y += gspd;
	y += move_y;
}
//-----------------------------------------------------------
// ジャンプフラグ切り替え
//-----------------------------------------------------------
void PlayerBase::ChangeJflag()
{
	if (jflag) {
		jflag = false;
	}
	move_y = 0;
}
//-----------------------------------------------------------
// 向きの変更
//-----------------------------------------------------------
void PlayerBase::Dir_Change()
{
	if (CheckHitKey(KEY_INPUT_D)) {
		dir = true;
	}
	if (CheckHitKey(KEY_INPUT_A)) {
		dir = false;
	}
}
//-----------------------------------------------------------
// アニメーション
//-----------------------------------------------------------
void PlayerBase::Animation()
{
	count++;
	count %= ANIMATIONA_SPEED;
	if (CheckHitKey(KEY_INPUT_D)) {
		if (count == 0) {
			ptr++;
		}
	}
	if (CheckHitKey(KEY_INPUT_A)) {
		if (count == 0) {
			ptr++;
		}
	}
	if (ptr > 2) {
		ptr = 0;
	}
}
//-----------------------------------------------------------
// 移動
//-----------------------------------------------------------
void PlayerBase::Move()
{
	if (CheckHitKey(KEY_INPUT_D)) {
		x += speed;
		if (CheckHitKey(KEY_INPUT_LSHIFT)) {
			x += speed * 0.5f;
		}
	}
	if (CheckHitKey(KEY_INPUT_A)) {
		x -= speed;
		if (CheckHitKey(KEY_INPUT_LSHIFT)) {
			x -= speed * 0.5f;
		}
	}
}
//-----------------------------------------------------------
// 動ける範囲かチェック
//-----------------------------------------------------------
bool PlayerBase::MoveCheckRange()
{
	SetDrawX();
	MoveDrawX();
	if (draw_x <= 0 || draw_x >= MAP_WIDTH_MAX * MAP_SIZE - WINDOW_W) {
		return true;
	}

	return false;
}
//-----------------------------------------------------------
// マップのスクロール量
//-----------------------------------------------------------
void PlayerBase::MoveDrawX()
{
	if (CheckHitKey(KEY_INPUT_D)) {
		draw_x += speed;
		if (CheckHitKey(KEY_INPUT_LSHIFT)) {
			draw_x += speed * 0.5f;
		}
	}
	if (CheckHitKey(KEY_INPUT_A)) {
		draw_x -= speed;
		if (CheckHitKey(KEY_INPUT_LSHIFT)) {
			draw_x -= speed * 0.5f;
		}
	}
}



