#pragma once
#include"Map.h"
#include"Block.h"
#include"Main.h"

class GameMain
{
private:
	// ブロックの位置用
	Vec2 b_pos;
	// トラップの位置用
	Vec2 t_pos;
	Vec2 t_drop_pos;
	// カメラ
	CameraBox2D camera;
	// マップ
	TaskKeep<Map> map;
	// MAPテキスト用
	String MAP_text;
	// Trapテキスト用
	String Trap_text;
	// トラップの種類
	int type;
	// マップ選択変数
	int map_num;
	// ゴール判定
	bool goal;

	// TRIANGLE_OUT 用の変数
	int tout_cnt;		// ファイルの中の何個目か見る用
	int tout_cnt1;		// ファイルの中の数を数える
	int tout_add;		// numを書き換えた数を数える
	int tout_add1;		// クリエイトされた数を数える

	// フォント用
	const Font m_font;

public:
	GameMain(int num);
	~GameMain();

	//----------------
	// 規定関数
	//----------------
	void Init();
	void Update();
	void Draw();
	void Exit();
	// 取得したい番号と座標の三角形を取得
	Triangle GetTriangle(int num, double x,double y) const;

	//----------------
	// Get関数
	//----------------
	bool GetIsPlayerDeath()const { return map->GetPDeath(); }
	bool GetIsGoal()const { return goal; }
	RectF GetRectGoal() const { return RectF(MAX_W - 100, MAP_SIZE, MAP_SIZE, MAP_SIZE * 3); }
	double GetPlayerPosX()const { return map->GetPlayerPosX(); }

private:
	// テキストファイルを読み込んでブロックを作る
	void TextReadCreatBlock();
	// MAPテキストを読み込む
	void ReadTextMAP();
	// MAPテキストに書き込む
	void WriterTextMAP();
	// MAPテキストの中身の書き換え
	void ChangeTextBlock();

	// 最初だけ呼ばれる関数( Init → Trap2 , DropTrap2 )
	void InitTextCreatTrap();
	// 初期のテキストにTrapテキストを上書き保存( Trap , DropTrap → Init ) 
	void InitWriterTextTrap();
	// トラップテキストを読み込みトラップ作成( Trap )
	void TextReadCreatTrap();
	// トラップテキストを読み込み飛び出るトラップを作成( Trap )
	void TextReadCreatOutTrap1();

	// トラップテキストに書き込み( Trap2 → Trap )
	void WriterTextTrap();
	// トラップ2テキストに書き込み( Trap → Trap2 )
	void WriterText2Trap();
	// トラップを消す( Trap → Trap2 )( DropTrap → DropTrap2)
	void DeleteTrap();
	// トラップをチェンジする( Trap → Trap2 )
	void ChangeTrap();
	// ドロップトラップテキストを読み込む( DropTrap )
	void TextReadCreatDropTrap();
	// トラップテキストに書き込み( DropTrap2 → DropTrap )
	void WriterTextDropTrap();
	// トラップテキストに書き込み( DropTrap → DropTrap2 )
	void WriterText2DropTrap();
	

	// トラップの種類をキーで切り替え
	void KeyChangeTrapType();
};

