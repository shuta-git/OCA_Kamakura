#include "PlayerManager.h"

//---------------------------------------------
// コンストラクタ
//---------------------------------------------
PlayerManager::PlayerManager()
{
	// カメラの初期化
	if (camera == nullptr) { camera = new Camera(); }
	// プレイヤーの初期化
	if (player == nullptr) { player = new Player(); }
}
//---------------------------------------------
// デストラクタ
//---------------------------------------------
PlayerManager::~PlayerManager()
{
	if (camera != nullptr) {
		delete camera;
		camera = nullptr;
	}
	if (player != nullptr) {
		delete player;
		player = nullptr;
	}
}
//---------------------------------------------
// 更新
//---------------------------------------------
void PlayerManager::Update()
{
	// プレイヤーにカメラ回転Vectorを設定
	if (player != nullptr) {
		player->SetCameraRot(camera->GetRot());
	}
	// プレイヤー更新
	if (player != nullptr) {
		player->Update();
	}
	// カメラにプレイヤーのVector座標と回転用のVectorを設定
	if (camera != nullptr) {
		camera->SetTarget(player->GetPos());
		camera->SetRot(player->GetRot());
	}
	// カメラの更新
	if (camera != nullptr) {
		camera->Update();
	}
}
//---------------------------------------------
// 描画
//---------------------------------------------
void PlayerManager::Draw()
{
	// カメラ描画
	if (camera != nullptr) { camera->Draw(); }
	//　プレイヤー描画
	if (player != nullptr) { player->Draw(); }
}
//---------------------------------------------
// Player型でプレイヤーを返す
//---------------------------------------------
Player * PlayerManager::GetPlayer() const
{
	if (player == nullptr)return nullptr;
	return (Player*)player;
}

