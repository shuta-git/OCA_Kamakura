#pragma once
#include "Cursor.h"

//-----------------------------------------------------------
// 定数
//-----------------------------------------------------------
#define MAP_WIDTH_MAX   200        // マップの最大横幅
#define MAP_HEIGHT_MAX  15         // マップの最大縦長さ
#define MAP_MAX         1          // マップの数
#define MAX_MAP_IMG     10
#define MAP_SPEED  6.0F

//-----------------------------------------------------------
// クラス
//-----------------------------------------------------------
class MAP : public BaseObj{
public:
	MAP();

	~MAP();

	Cursor* cur = nullptr;

	void Init();
	void Exit();
	void Update(int num_,int mode_num_,int old_num_);
	void Draw();
	void ReloadGraph();
	// 当たり判定
	void SetPlayer(Player* player);
	void CheckHitPlayer(Player* player);
	void ResetPosX() { draw_x = old_x; }// リセットX
	void SetMoveX() { draw_old_x = move_x; }
	void ResetMoveX() { move_x = draw_old_x; }

	void InitDrawLeftX() { draw_x = 0; }
	void InitDrawRightX();

	float GetDrawX() const { return draw_x; }

	

private:

	
	int number;					// ステージナンバー
	int mode_num;
	int old_num;

	int images[MAX_MAP_IMG];
	int BlockData[MAP_HEIGHT_MAX][MAP_WIDTH_MAX];
	
	int image_count = 0;
	bool start;

	void ImageChange();			// ブロックのイメージチェンジ
	
	void InitFileRead();
	void TutorialFileRead();
	void TutorialFileSave();
	void OldFileRead();
	void OldFileSave();
	void FileRead();			// ファイルの読み込み
	void FileSave();			// ファイルセーブ

	void Move();
	void DrawMoveX();

	bool MoveCheckRangeLeft();					// 動ける範囲内かチェック
	bool MoveCheckRangeRight();					// 動ける範囲内かチェック
								
	void DrawMoveRange();		// マップビルダー時のスクロール範囲

};


