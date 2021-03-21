#include "EnemyManager.h"

//---------------------------------------------
// コンストラクタ
//---------------------------------------------
EnemyManager::EnemyManager()
{
	pos = { 0.0f,0.0f,-60.0f };
	rot = { 0.0f,0.0f,0.0f };
	scale = { 1.0f,1.0f,1.0f };

	spawn_timer = 0;
	num = 0;
	enemy_count = 0;

	spawn_pos = pos.GetV();
}
//---------------------------------------------
// デストラクタ
//---------------------------------------------
EnemyManager::~EnemyManager()
{
}
//---------------------------------------------
// 更新
//---------------------------------------------
void EnemyManager::Update(int time)
{
	spawn_timer++;

	if (time != 0) {
		// スポーン
		SetSpawn(time);
	}

	for (int i = 0; i < enemies.size();i++) {
		if (enemies[i] != nullptr) {
			enemies[i]->Update();
			DeleteEnemy(i);
		}
	}

	EraseEnemy();
}
//---------------------------------------------
// 描画
//---------------------------------------------
void EnemyManager::Draw()
{
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i] != nullptr) {
			enemies[i]->Draw();
		}
	}

#if(NDEBUG)
	
#endif
}
//---------------------------------------------
// EnemyBase型で取得
//---------------------------------------------
EnemyBase * EnemyManager::GetEnemyBase(int i) const
{
	if (enemies[i] == nullptr)return nullptr;
	return (EnemyBase*)enemies[i];
}
//---------------------------------------------
// エネミーのサイズを取得
//---------------------------------------------
size_t EnemyManager::GetSize() const
{
	return enemies.size();
}
//---------------------------------------------
// デリート処理
//---------------------------------------------
void EnemyManager::DeleteEnemy(int i)
{
	if (enemies[i]->GetDeathAnim()) {
		delete enemies[i];
		enemies[i] = nullptr;
		enemy_count--;
	}
}
//---------------------------------------------
// イレース処理
//---------------------------------------------
void EnemyManager::EraseEnemy()
{
	std::vector<EnemyBase*>::iterator it = enemies.begin();
	while (it != enemies.end())
	{
		if (*it == nullptr) {
			it = enemies.erase(it);
		}
		else {
			it++;
		}
	}
}
//---------------------------------------------
// エネミーのスポーン設定
//---------------------------------------------
void EnemyManager::SetSpawn(int time)
{
	if (time > 110) {
		spawn_timer %= 120;
	}
	else if(time > 70) {
		spawn_timer %= 90;
	}
	else  {
		spawn_timer %= 50;
	}

	if (spawn_timer == 0) {
		enemy_count++;
		num = GetRand(7);
		int type = num;
		if (type < 5) {
			num = ENEMY1;
		}
		if (type > 4 && type < 8) {
			num = ENEMY2;
		}
		Spawn();
	}
}
//---------------------------------------------
// エネミーのスポーン
//---------------------------------------------
void EnemyManager::Spawn()
{
	int spawn_num = GetRand(2);

	spawn_pos.x = GetRand(70);
	int type = GetRand(1);
	if (type == 0) {
		spawn_pos.x *= -1;
	}

	switch (num)
	{
	case ENEMY1:
		enemies.push_back(new Enemy1(spawn_pos));
		break;
	case ENEMY2:
		enemies.push_back(new Enemy2(spawn_pos));
		break;
	case ENEMY3:
		enemies.push_back(new Enemy3(spawn_pos));
		break;
	default:
		break;
	}
}
