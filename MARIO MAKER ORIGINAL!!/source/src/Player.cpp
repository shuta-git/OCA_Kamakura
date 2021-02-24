#include "WinMain.h"
#include "Player.h"
#include "Fiona.h"
#include "Musya.h"
#include "Ninin.h"

//-----------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------
Player::Player()
{
	Init();
	if (playerbase != nullptr) {
		playerbase = nullptr;
	}
	jflag = false;
	gflag = true;
}
//-----------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------
Player::~Player()
{
	Exit();
}

//-----------------------------------------------------------
//　初期化処理 
//-----------------------------------------------------------
void Player::Init()
{
	score = 0.00;
	x = 150;
	y = 0;
	number = FIONA;
	old_number = number;
}
//-----------------------------------------------------------
// 終了処理
//-----------------------------------------------------------
void Player::Exit()
{
	if (playerbase != nullptr) {
		delete playerbase;
		playerbase = nullptr;
	}
}
//-----------------------------------------------------------
// 更新処理
//-----------------------------------------------------------
void Player::Update()
{
	SetOld();
	old_number = number;
	// キャラクターチェンジ
	Character_Change();
	// セットしたキャラクター出ないとき消す
	Character_Delete();
	// どのキャラクターにするか設定
	Character_Set();

	// キャラクターの更新処理
	if (playerbase != nullptr) {
		playerbase->Update();
	}
	Draw_Storage();
	Pos_Old_Storage();
	Pos_Storage();
	Dir_Storage();
	Speed_Storage();
}
//-----------------------------------------------------------
// 描画処理
//-----------------------------------------------------------
void Player::Draw(){
	if (playerbase != nullptr) {
		playerbase->Draw();
	}
}
//-----------------------------------------------------------
// 画像の読み直し用の関数
//-----------------------------------------------------------
void Player::ReloadGraph()
{
	if (playerbase != nullptr) {
		playerbase->ReloadGraph();
	}
}
//-----------------------------------------------------------
// ジャンプフラグ切り替え
//-----------------------------------------------------------
void Player::ChangeJflag()
{
	if (jflag) {
		jflag = false;
	}
	move_y = 0;
	if (playerbase != nullptr) {
		playerbase->ChangeJflag();
	}
}
//-----------------------------------------------------------
// old_xに戻す
//-----------------------------------------------------------
void Player::ResetPosX() 
{
	x = old_x;
	if (playerbase != nullptr) {
		playerbase->ResetPosX();
	}
}
//-----------------------------------------------------------
// draw_oldxに戻す
//-----------------------------------------------------------
void Player::ResetDrawX()
{
	if (playerbase != nullptr) {
		playerbase->ResetDrawX();
	}
}
//-----------------------------------------------------------
// old_yに戻す
//-----------------------------------------------------------
void Player::ResetPosY()
{
	y = old_y;
	if (playerbase != nullptr) {
		playerbase->ResetPosY();
	}
}
//-----------------------------------------------------------
// キャラクターチェンジ
//-----------------------------------------------------------
void Player::Character_Change()
{
	if (CheckHitKey(KEY_INPUT_1)) {
		number = FIONA;
	}
	if (CheckHitKey(KEY_INPUT_2)) {
		number = MUSYA;
	}
	if (CheckHitKey(KEY_INPUT_3)) {
		number = NININ;
	}
}
//-----------------------------------------------------------
// セットしたキャラクターでない時デリート処理
//-----------------------------------------------------------
void Player::Character_Delete()
{
	if (number != old_number) {
		Exit();
	}
}
//-----------------------------------------------------------
// 座標を保存
//-----------------------------------------------------------
void Player::Pos_Storage()
{
	if (playerbase != nullptr) {
		x = playerbase->GetPosX();
		y = playerbase->GetPosY();
		move_y = playerbase->GetMoveY();
	}
}
//-----------------------------------------------------------
// old座標を保存
//-----------------------------------------------------------
void Player::Pos_Old_Storage()
{
	if (playerbase != nullptr) {
		old_x = playerbase->GetOldX();
		old_y = playerbase->GetOldY();
	}
}
//-----------------------------------------------------------
// 向きを保存
//-----------------------------------------------------------
void Player::Dir_Storage()
{
	if (playerbase != nullptr) {
		dir = playerbase->GetDir();
	}
}
//-----------------------------------------------------------
// スピードを保存
//-----------------------------------------------------------
void Player::Speed_Storage()
{
	if (playerbase != nullptr) {
		speed = playerbase->GetSpeed();
	}
}
//-----------------------------------------------------------
// スピードを保存
//-----------------------------------------------------------
void Player::Draw_Storage()
{
	if (playerbase != nullptr) {
		draw_x = playerbase->GetDrawX();
		draw_y = playerbase->GetDrawY();
	}
}
//-----------------------------------------------------------
// ジャンプフラグ取得
//-----------------------------------------------------------
bool Player::GetJflag() const
{
	return playerbase->GetJflag();
}
//-----------------------------------------------------------
// マップの範囲内か見る
//-----------------------------------------------------------
bool Player::Check_Map_Range()
{
	if (playerbase != nullptr) {
		if (playerbase->GetPosX() <= -12 || playerbase->GetDrawX() >= MAP_WIDTH_MAX * MAP_SIZE - 185) {
			playerbase->ResetPosX();
			playerbase->ResetDrawX();
			return true;
		}
	}
	return false;
}
//-----------------------------------------------------------
// キャラクター設定
//-----------------------------------------------------------
void Player::Character_Set()
{
	switch (number)
	{
	case FIONA:
		if (playerbase == nullptr) {
			playerbase = new Fiona(x,y,dir,draw_x);
		}
		break;
	case MUSYA:
		if (playerbase == nullptr) {
			playerbase = new Musya(x,y,dir,draw_x);
		}
		break;
	case NININ:
		if (playerbase == nullptr) {
			playerbase = new Ninin(x, y, dir,draw_x);
		}
		break;
	default:
		break;
	}
}
//-----------------------------------------------------------
// スコアを足す
//-----------------------------------------------------------
void Player::AddScore()
{
	score += ADD_SCORE;
}
//-----------------------------------------------------------
// タイマーが進むたびにスコアを足す
//-----------------------------------------------------------
void Player::TimerScore(double timer_)
{
	score += timer_;
}


