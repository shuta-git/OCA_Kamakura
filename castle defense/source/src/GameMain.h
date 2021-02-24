#pragma once


//---------------------------------------------
// GameMainクラス  継承SceneBase
//---------------------------------------------
class GameMain : public SceneBase
{
public:
	GameMain();
	~GameMain();

	void Update() override;
	void Draw() override;

private:
	
	int skydome;	 // スカイドーム
	int play_timer;	 // プレイ時間
	int old_time;	 // 前の時間を保存
	int count_time;	 // スタートしてからの経過時間
	int start_time;  // スタート時間
	int result_timer; // クリア時に使うタイマー

	bool clear;		 // ゲームクリア用のフラグ
	bool fail;		 // ゲームオーバー用のフラグ
	bool result;	 // クリアか失敗か
	int score;		 // スコア
	int defeat_count;// 撃破カウント

	bool pause;		// ポーズ

	MapManager* map_m;			// マップマネジャー
	EnemyManager* enemy_m;		// エネミーマネジャー
	PlayerManager* player_m;	// プレイヤーマネージャー

	// ファイル読み込み用
	fstream file;
	
	//=======================================
	//　Debug用
	//=======================================
	bool isUpdata;						// 更新処理をしているか
	bool isOneUpdate;					// 更新処理を一度行う
	// Debugの更新処理
	void DebugUpdata();	

	//=======================================
	// 当たり判定
	//======================================= 
	// 当たり判定まとめ
	void CheckHit();											
	// プレイヤーとエネミーの範囲の当たり判定
	void CheckHitPlayer_EnemyRange(Player* p, EnemyBase* e);
	// プレイヤーとエネミーの攻撃範囲の当たり判定
	void CheckHitPlayer_EnemyAttackRange(Player* p, EnemyBase* e);
	// プレイヤーとエネミーの攻撃の当たり判定
	void CheckHitPlayer_EnemyAttack(Player* p, EnemyBase* e);
	// 剣とエネミーのあたり範囲
	void CheckHitSword_Enemy(Player* p);
	// 特殊攻撃とエネミーの当たり判定
	void CheckHitSpacial_Enemy(Player* p);
	// エネミーの殴り攻撃と城の当たり判定
	void CheckHitEnemyAttack_Castle(EnemyBase*e, Castle* c);
	// マップの当たり判定
	void CheckHitMap(Castle* m, CollisionBase* c);

	//=======================================
	// 関数
	//=======================================
	// ゲームクリアチェック
	void CheckGameClear();
	// ゲームオーバーチェック
	void CheckGameOver();
	// ゲームクリア
	void GameClear();
	// ゲームオーバー
	void GameOver();
	// 残りHPをチェック
	void HPCheck(CollisionBase* c);
	// CastleのHPゲージの描画
	void DrawCastleHPGauge(Castle* m);

	//=======================================
	// ファイル関係
	//=======================================
	// pos1ファイル読み込み
	void Pos1FileRead();
	// pos2ファイル読み込み
	void Pos2FileRead();
	// radiusファイル読み込み
	void RadiusFileRead();
	// resultファイル書き込み
	void ResultFileWrite();

	Vector3 pos;
	Vector3 rot;
	Vector3 scale;

	Vector3 enemy_pos;

	std::vector<Vector3>pos1;
	std::vector<Vector3>pos2;
	std::vector<float>radius;


	enum FontType {
		FONT_HP = 0,
		FONT_TIME,
		FONT_GUIDE,
		FONT_CLEAR,
		FONT_PAUSE,
		

		FONT_MAX,
	};

};
