#pragma once

//---------------------------------------------
// MapManagerクラス
//---------------------------------------------
class MapManager
{
public:
	MapManager();
	~MapManager();

	void Update();
	void Draw();

	// Castle型で城を返す
	Castle* GetCastle()const;

private:
	
	// オブジェクトを生成
	void Create();
	// 消去
	void Erase();

	//=======================================
	// テクスチャー
	//=======================================
	
	std::vector<ObjectBase*>object;		// マップテンプレート

	Vector3 pos;	// 座標
	Vector3 scale;	// サイズ
	Vector3 rot;	// 回転

};

