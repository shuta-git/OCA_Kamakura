#pragma once

//---------------------------------------------
// PlayerManagerクラス
//---------------------------------------------
class PlayerManager
{
public:
	PlayerManager();
	~PlayerManager();

	// 更新
	void Update();
	// 描画
	void Draw();

	//=======================================
	// Get関数
	//=======================================

	// Player型でプレイヤーを返す
	Player* GetPlayer()const;

private:

	Camera* camera = nullptr;	// カメラ作成
	Player* player = nullptr;	// プレイヤー作成

};

