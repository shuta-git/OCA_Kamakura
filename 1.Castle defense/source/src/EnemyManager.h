#pragma once

class GameMain;
//---------------------------------------------
// EnemyManagerクラス
//---------------------------------------------
class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();

	void Update(int time);
	void Draw();

	//=======================================
	// Get関数
	//=======================================

	// EnemyBase型で取得
	EnemyBase* GetEnemyBase(int i)const;

	// エネミーのサイズを取得
	size_t GetSize() const;							 

private:
	//=======================================
	// 関数
	//=======================================
	// エネミーのデリート処理
	void DeleteEnemy(int i);	
	// エネミーのイレース処理
	void EraseEnemy();
	// エネミーのスポーン設定
	void SetSpawn(int time);
	// エネミーのスポーン
	void Spawn();

	std::vector<EnemyBase*>enemies;	// エネミー

	int spawn_timer;		// 生成時間をはかる
	int num;				// エネミーの番号
	int enemy_count;		// エネミーの数を数える

	Vector3 pos;
	Vector3 rot;
	Vector3 scale;

	VECTOR spawn_pos;

	// エネミーの種類
	enum Type
	{
		ENEMY1 = 0,
		ENEMY2,
		ENEMY3,

		ENEMY_MAX,
	};

};

