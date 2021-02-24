#include "MapManager.h"

//---------------------------------------------
// コンストラクタ
//---------------------------------------------
MapManager::MapManager()
{
	// 城
	object.push_back(new Castle());
}
//---------------------------------------------
// デストラクタ
//---------------------------------------------
MapManager::~MapManager()
{
	for (int i = 0; i < object.size(); i++) {
		delete object[i];
		object[i] = nullptr;
	}
	Erase();
}
//---------------------------------------------
// 更新
//---------------------------------------------
void MapManager::Update()
{
	for (int i = 0; i < object.size(); i++) {
		object[i]->Update();
	}
}
//---------------------------------------------
// 描画
//---------------------------------------------
void MapManager::Draw()
{
	for (int i = 0; i < object.size(); i++) {
		object[i]->Draw();
		
	}
}
//---------------------------------------------
// Castle型で城を返す
//---------------------------------------------
Castle * MapManager::GetCastle() const
{
	if (object[0] == nullptr)return nullptr;
	return (Castle*)object[0];
}
//---------------------------------------------
// オブジェクトを生成
//---------------------------------------------
void MapManager::Create()
{
	// 城
	object.push_back(new Castle());
}
//---------------------------------------------
// 消去
//---------------------------------------------
void MapManager::Erase()
{
	std::vector<ObjectBase*>::iterator it = object.begin();
	while (it != object.end())
	{
		if (*it == nullptr) {
			it = object.erase(it);
		}
		else {
			it++;
		}
	}
}
