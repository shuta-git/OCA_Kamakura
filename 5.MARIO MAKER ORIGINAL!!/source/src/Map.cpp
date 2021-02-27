#include "WinMain.h"
#include "Map.h"

#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
using namespace std;

fstream file;

//-----------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------
MAP::MAP()
{

	for (int a = 0; a < MAX_MAP_IMG; a++) {
		char file_str[256];
		sprintfDx(file_str, "data/block/block%02d.png", (a + 1));
		images[a] = LoadGraph(file_str);
	}
	x = 0;
	draw_x = 0;
	draw_y = 0;
	move_x = 0;
}

//-----------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------
MAP::~MAP() {
	Exit();
}

//-----------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------
void MAP::Init() {
	x = 0;
	draw_x = 0;
	draw_y = 0;
	move_x = 0;
	if (mode_num == MAP_MODE) {
		FileRead();
		OldFileSave();
	}
	if (mode_num == TUTORIAL_MODE) {
		TutorialFileRead();
		OldFileSave();
	}
	if (mode_num  == GAME_MODE) {
		FileRead();
		OldFileSave();
	}
}
//-----------------------------------------------------------
// 終了処理
//-----------------------------------------------------------
void MAP::Exit() {
	
	for (int a = 0; a < MAX_MAP_IMG; a++) {
		DeleteGraph(images[a]);
	}

	if (cur != nullptr) {
		delete cur;
		cur = nullptr;
	}
}
//-----------------------------------------------------------
// 更新処理
//-----------------------------------------------------------
void MAP::Update(int num_,int mode_num_, int old_num_) {
	
	if (cur == nullptr) {
		cur = new Cursor();
	}
	if (mode_num == MAP_MODE) {
		if (IsKeyOn(KEY_INPUT_TAB)) {
			OldFileSave();
		}
	}

	number = num_;
	mode_num = mode_num_;
	old_num = old_num_;


	//-- マップビルダー
	if (mode_num == MAP_MODE) {
		if (cur != nullptr) {	
			cur->Move();
		}
		if (CheckHitKey(KEY_INPUT_D)) {
			draw_x += MAP_SIZE;
		}
		if (CheckHitKey(KEY_INPUT_A)) {
			draw_x -= MAP_SIZE;
		}
		DrawMoveRange();
		// ブロックのイメージを切り替え
		ImageChange();
		// ファイルの読み込み
		FileRead();
		// ファイルのセーブ
		if (IsKeyOn(KEY_INPUT_L)) {
			InitFileRead();
			FileSave();
			OldFileSave();
		}
	}

	if (mode_num == TUTORIAL_MODE) {
		old_x = draw_x;
		// カーソル
		if (cur != nullptr) {
			delete cur;
			cur = nullptr;
		}
	}

	//-- ゲーム中
	if (mode_num == GAME_MODE) {
		if (old_num == PAUSE ) {
			FileRead();
			OldFileSave();
		}
		// ファイルの読み込み
		OldFileRead();
		old_x = draw_x;
		// カーソル
		if (cur != nullptr) {
			delete cur;
			cur = nullptr;
		}
	}
}

//-----------------------------------------------------------
// 描画処理
//-----------------------------------------------------------
void MAP::Draw() {

	for (int h = 0; h < MAP_HEIGHT_MAX; h++) {
		for (int w = 0; w < MAP_WIDTH_MAX; w++) {
			int a = BlockData[h][w];
			x = (float)(MAP_SIZE * w) - draw_x;
			y = (float)(MAP_SIZE * h);
			DrawGraphF(x, y, images[a], TRUE);
		}
	}
	
	if (mode_num == MAP_MODE) {
		SetFontSize(17);
		DrawString(20, 35, "Lで初期マップに戻す", GetColor(255, 255, 255));
		DrawString(20, 89, "TABでゲームモード", GetColor(255, 255, 255));
		DrawString(20, 62, "左クリックでブロックをセット（長押しもできます）", GetColor(255, 255, 255));
		DrawFormatString(8, 8, GetColor(255, 255, 255), "現在のブロックナンバー : %d", image_count + 1);
		DrawString(260, 8, "1 : 石ブロック", GetColor(255, 255, 255));
		DrawString(400, 8, "2 : 青ブロック", GetColor(255, 255, 255));
		DrawString(540, 8, "3 : 鉄ブロック", GetColor(255, 255, 255));
		DrawString(680, 8, "4 : 黄？ブロック", GetColor(255, 255, 255));
		DrawString(825, 8, "5 : 茶？ブロック", GetColor(255, 255, 255));
		DrawString(980, 8, "6 : 茶ブロック", GetColor(255, 255, 255));
		DrawString(1120, 8, "7 : 草ブロック", GetColor(255, 255, 255));
		DrawString(1260, 8, "8 : 危険ブロック", GetColor(255, 255, 255));
		DrawString(1435, 8, "9 : 土ブロック", GetColor(255, 255, 255));
		DrawString(1580, 8, "10 : ブロック消去", GetColor(255, 255, 255));
		DrawString(20,116,"カーソルの位置 ↓",GetColor(255,255,255));

		DrawCircle(25, 147, 5, GetColor(255, 255, 255), TRUE);
		DrawCircle(422, 147, 5, GetColor(255, 255, 255), TRUE);
		DrawCircle(225, 147, 5, GetColor(255, 255, 255), TRUE);
		DrawBox(25, 147, 425, 149, GetColor(255, 255, 255), TRUE);
		DrawBox(20 + (cur->GetX() + (int)draw_x) / 16, 142, 31 + (cur->GetX() + (int)draw_x) / 16, 153, GetColor(255, 0, 0), FALSE);

		// カーソル
		if (cur != nullptr) {
			cur->Draw();
		}
		if (CheckMouseInput()) {
			int h = (cur->GetY() / MAP_SIZE  * MAP_SIZE + (int)draw_y) / MAP_SIZE;
			int w = (cur->GetX() / MAP_SIZE * MAP_SIZE + (int)draw_x) / MAP_SIZE;
			BlockData[h][w] = image_count;
			FileSave();
		}
		
	}
}

//-----------------------------------------------------------
// playerを設定
//-----------------------------------------------------------
void MAP::SetPlayer(Player* player)
{
	speed = player->GetSpeed();
	// マップスクロール
	SetMoveX();
	Move();
	if (MoveCheckRangeLeft()) {
		InitDrawLeftX();
	}
	if (MoveCheckRangeRight()) {
		InitDrawRightX();
	}
	// マップのスクロールできる範囲
	if(MoveCheckRangeLeft()==false && MoveCheckRangeRight() == false){
		DrawMoveX();
	}

	// あたり判定		
	
	CheckHitPlayer(player);
	
	
}
//-----------------------------------------------------------
// playerとの当たり判定
//-----------------------------------------------------------
void MAP::CheckHitPlayer(Player* player)
{
	for (int h = 0; h < MAP_HEIGHT_MAX; h++) {
		for (int w = 0; w < MAP_WIDTH_MAX; w++) {
			for (int a = 0; a < MAX_MAP_IMG - 1; a++) {
				if (BlockData[h][w] == a) {
					x = (float)(MAP_SIZE * w);
					y = (float)(MAP_SIZE * h);
					// ninin
					if (player->GetNum() == NININ) {
						if (x - draw_x + MAP_SIZE >= player->GetPosX() + 10 && x - draw_x <= player->GetPosX() + 25 &&
							y + MAP_SIZE >= player->GetPosY() + 15 && y <= player->GetPosY() + 44) {

							if (x - draw_x + MAP_SIZE >= player->GetPosX() + 10 && x - draw_x <= player->GetPosX() + 25 &&
								y + MAP_SIZE >= player->GetOldY() + 15 && y <= player->GetOldY() + 44)
							{
								if (a == 0) {
									player->ChangeJflag();
								}
								player->ResetPosX();
								player->ResetDrawX();
								ResetPosX();
								ResetMoveX();
							}
							if (x - draw_x + MAP_SIZE >= player->GetOldX() + 10 && x - draw_x <= player->GetOldX() + 25 &&
								y + MAP_SIZE >= player->GetPosY() + 15 && y <= player->GetPosY() + 44)
							{
								player->ResetPosY();
								if (player->GetMoveY() > 0) {
									player->ChangeJflag();
								}
								if (player->GetJflag()) {
									if (a == 3) {
										BlockData[h][w] = 4;
										OldFileSave();
										player->AddScore();
									}
								}
							}
						}
					}
					else {// Fiona Musya
						if (x - draw_x + MAP_SIZE >= player->GetPosX() + 10 && x - draw_x <= player->GetPosX() + 25 &&
							y + MAP_SIZE >= player->GetPosY() + 10 && y <= player->GetPosY() + 44) {
							if (x - draw_x + MAP_SIZE >= player->GetPosX() + 10 && x - draw_x <= player->GetPosX() + 25 &&
								y + MAP_SIZE >= player->GetOldY() + 10 && y <= player->GetOldY() + 44)
							{
								player->ResetPosX();
								player->ResetDrawX();
								ResetPosX();
								ResetMoveX();
								// ムシャだけ
								if (CheckHitKey(KEY_INPUT_LSHIFT)) {
									if (a == 8 && player->GetNum() == MUSYA) {
										BlockData[h][w] = 9;
										BlockData[h + 1][w] = 9;
										OldFileSave();
									}
								}
							}
							if (x - draw_x + MAP_SIZE >= player->GetOldX() + 10 && x - draw_x <= player->GetOldX() + 25 &&
								y + MAP_SIZE >= player->GetPosY() + 10 && y <= player->GetPosY() + 44)
							{
								player->ResetPosY();
								if (player->GetMoveY() > 0) {
									player->ChangeJflag();
								}
								if (player->GetJflag()) {
									if (a == 3) {
										BlockData[h][w] = 4;
										OldFileSave();
										player->AddScore();
									}
								}
								if (CheckHitKey(KEY_INPUT_LSHIFT)) {
									if (a == 8 && player->GetNum() == MUSYA) {
										BlockData[h][w] = 9;
										BlockData[h + 1][w] = 9;
										OldFileSave();
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
//-----------------------------------------------------------
// 画像の読み直し用の関数
//-----------------------------------------------------------
void MAP::ReloadGraph()
{
	for (int a = 0; a < MAX_MAP_IMG; a++) {
		char file_str[256];
		sprintfDx(file_str, "data/block/block%02d.png", (a + 1));
		images[a] = LoadGraph(file_str);
	}
}

//-----------------------------------------------------------
// ブロックのイメージチェンジ
//-----------------------------------------------------------
void MAP::ImageChange()
{
	if (IsKeyOn(KEY_INPUT_1)) image_count = 0;
	if (IsKeyOn(KEY_INPUT_2)) image_count = 1;
	if (IsKeyOn(KEY_INPUT_3)) image_count = 2;
	if (IsKeyOn(KEY_INPUT_4)) image_count = 3;
	if (IsKeyOn(KEY_INPUT_5)) image_count = 4;
	if (IsKeyOn(KEY_INPUT_6)) image_count = 5;
	if (IsKeyOn(KEY_INPUT_7)) image_count = 6; 
	if (IsKeyOn(KEY_INPUT_8)) image_count = 7;
	if (IsKeyOn(KEY_INPUT_9)) image_count = 8;
	if (IsKeyOn(KEY_INPUT_0)) image_count = 9;
}
//-----------------------------------------------------------
// 初期ファイル読み込み
//-----------------------------------------------------------
void MAP::InitFileRead()
{
		file.open("data/txt/InitMap.txt", ios::in);

		if (!file.is_open()) {
			// ファイルを開くことに失敗しました
			return;
		}

		string str;
		int h = 0;
		while (getline(file, str))
		{
			while (true) {
				string::size_type pos = str.find(',', 0);
				if (pos == string::npos) break;
				str.replace(pos, 1, " ");	// replace=置き換え/ replace(場所,文字数,なんて文字に)
			}

			stringstream sstr(str);
			int x;
			for (int w = 0; w < MAP_WIDTH_MAX; w++) {
				sstr >> x;
				BlockData[h][w] = x;
			}
			h++;
		}
	
	// ファイルを閉じる
	file.close();
}
//-----------------------------------------------------------
// チュートリアルファイルの読み込み
//-----------------------------------------------------------
void MAP::TutorialFileRead()
{
	file.open("data/txt/TutorialMap.txt", ios::in);

	if (!file.is_open()) {
		// ファイルを開くことに失敗しました
		return;
	}

	string str;
	int h = 0;
	while (getline(file, str))
	{
		while (true) {
			string::size_type pos = str.find(',', 0);
			if (pos == string::npos) break;
			str.replace(pos, 1, " ");	// replace=置き換え/ replace(場所,文字数,なんて文字に)
		}

		stringstream sstr(str);
		int x;
		for (int w = 0; w < MAP_WIDTH_MAX; w++) {
			sstr >> x;
			BlockData[h][w] = x;
		}
		h++;
	}

	// ファイルを閉じる
	file.close();
}
//-----------------------------------------------------------
// チュートリアルファイルセーブ
//-----------------------------------------------------------
void MAP::TutorialFileSave()
{
	file.open("data/txt/TutorialMap.txt", ios::in);

	if (!file.is_open()) {
		// ファイルを開くことに失敗しました
		return;
	}

	string str;
	int h = 0;
	while (getline(file, str))
	{
		while (true) {
			string::size_type pos = str.find(',', 0);
			if (pos == string::npos) break;
			str.replace(pos, 1, " ");	// replace=置き換え/ replace(場所,文字数,なんて文字に)
		}

		stringstream sstr(str);
		int x;
		for (int w = 0; w < MAP_WIDTH_MAX; w++) {
			sstr >> x;
			BlockData[h][w] = x;
		}
		h++;
	}

	// ファイルを閉じる
	file.close();
}
//-----------------------------------------------------------
// 一つ前のファイル読み込み
//-----------------------------------------------------------
void MAP::OldFileRead()
{
	file.open("data/txt/OldMap.txt", ios::in);

	if (!file.is_open()) {
		// ファイルを開くことに失敗しました
		return;
	}

	string str;
	int h = 0;
	while (getline(file, str))
	{
		while (true) {
			string::size_type pos = str.find(',', 0);
			if (pos == string::npos) break;
			str.replace(pos, 1, " ");	// replace=置き換え/ replace(場所,文字数,なんて文字に)
		}

		stringstream sstr(str);
		int x;
		for (int w = 0; w < MAP_WIDTH_MAX; w++) {
			sstr >> x;
			BlockData[h][w] = x;
		}
		h++;
	}

	// ファイルを閉じる
	file.close();
}

//-----------------------------------------------------------
// ファイル読み込み
//-----------------------------------------------------------
void MAP::FileRead()
{
		file.open("data/txt/Map.txt", ios::in);

		if (!file.is_open()) {
			// ファイルを開くことに失敗しました
			return;
		}

		string str;
		int h = 0;
		while (getline(file, str))
		{
			while (true) {
				string::size_type pos = str.find(',', 0);
				if (pos == string::npos) break;
				str.replace(pos, 1, " ");	// replace=置き換え/ replace(場所,文字数,なんて文字に)
			}

			stringstream sstr(str);
			int x;
			for (int w = 0; w < MAP_WIDTH_MAX; w++) {
				sstr >> x;
				BlockData[h][w] = x;
			}
			h++;
		}
	
	// ファイルを閉じる
	file.close();
}
//-----------------------------------------------------------
// 一つ前のファイルセーブ
//-----------------------------------------------------------
void MAP::OldFileSave()
{

	file.open("data/txt/OldMap.txt", ios::out);

	if (!file.is_open()) {
		// ファイルを開くことに失敗しました
		return;
	}
	for (int h = 0; h < MAP_HEIGHT_MAX; h++) {
		for (int w = 0; w < MAP_WIDTH_MAX; w++) {
			int a = BlockData[h][w];
			file << a << ",";
		}
		file << endl;
	}


	// ファイルを閉じる
	file.close();
	//-------------------------------------------------------------------
}
//-----------------------------------------------------------
// ファイルセーブ
//-----------------------------------------------------------
void MAP::FileSave()
{
	
		file.open("data/txt/Map.txt", ios::out);

		if (!file.is_open()) {
			// ファイルを開くことに失敗しました
			return;
		}

		for (int h = 0; h < MAP_HEIGHT_MAX; h++) {
			for (int w = 0; w < MAP_WIDTH_MAX; w++) {
				int a = BlockData[h][w];
				file << a << ",";
			}
			file << endl;
		}

	
	// ファイルを閉じる
	file.close();
	//-------------------------------------------------------------------
}
//-----------------------------------------------------------
// 移動
// @! スクロール
// @! プレイヤー(仮想)
//-----------------------------------------------------------
void MAP::Move()
{
	if (CheckHitKey(KEY_INPUT_D)) {
		move_x += speed;
		if (CheckHitKey(KEY_INPUT_LSHIFT)) {
			move_x += speed * 0.5f;
		}
	}
	if (CheckHitKey(KEY_INPUT_A)) {
		move_x -= speed;
		if (CheckHitKey(KEY_INPUT_LSHIFT)) {
			move_x -= speed * 0.5f;
		}
	}
}
void MAP::DrawMoveX()
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
//-----------------------------------------------------------
// スクロールの左の範囲
//-----------------------------------------------------------
bool MAP::MoveCheckRangeLeft()
{
		if (move_x <= 0 ) {
			return true;
		}
	
		return false;
}
//-----------------------------------------------------------
// スクロールの右の範囲
//-----------------------------------------------------------
bool MAP::MoveCheckRangeRight()
{
		if ( move_x >= MAP_WIDTH_MAX * MAP_SIZE - WINDOW_W) {
			return true;
		}
	
		return false;
}
//-----------------------------------------------------------
// マップビルダー時のスクロール範囲
//-----------------------------------------------------------
void MAP::DrawMoveRange()
{
	if (draw_x < 0) {
		draw_x = 0;
	}
	if (draw_x > MAP_WIDTH_MAX * MAP_SIZE - MAX_WINDOW_W) {
		draw_x = MAP_WIDTH_MAX * MAP_SIZE - MAX_WINDOW_W;
	}
}
//-----------------------------------------------------------
// 右まで行ったら初期化
//-----------------------------------------------------------
void MAP::InitDrawRightX()
{
	draw_x = MAP_WIDTH_MAX * MAP_SIZE - WINDOW_W;
}






