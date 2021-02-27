#include "ObjectBase.h"

//---------------------------------------------
// コンストラクタ
//---------------------------------------------
ObjectBase::ObjectBase()
{
	pos = {0.0f,0.0f,0.0f};
	scale = { 1.0f, 1.0f, 1.0f };
	rot = { 0.0f, 0.0f, 0.0f };
}
//---------------------------------------------
// デストラクタ
//---------------------------------------------
ObjectBase::~ObjectBase()
{
}
//---------------------------------------------
// テクスチャーを適用
//---------------------------------------------
void ObjectBase::ApplyTexture()
{
	int texture_count = 0;
	for (int i = 0; i < model_handle.size(); i++) {
		int texture_size = MV1GetTextureNum(model_handle[i]);
		for (int j = 0; j < texture_size;j++) {
			int num = MV1GetMaterialDifMapTexture(model_handle[i], j);
			MV1SetTextureGraphHandle(model_handle[i], num, texture_handle[texture_count], FALSE);
			texture_count++;
		}
	}
}
//---------------------------------------------
// テクスチャーのパスを取得
//---------------------------------------------
void ObjectBase::SetTextureHandle(string str)
{
	// パスを設定
	string data = "data/map/Texture/";
	string path = data + str;
	texture_handle.push_back(resource::LoadGraph(path.c_str()));
}
//---------------------------------------------
// モデルのパスを設定
//---------------------------------------------
void ObjectBase::SetModelHandle(string str)
{
	// パスを設定
	string data = "data/map/";
	string path = data + str;
	model_handle.push_back(resource::LoadModel(path.c_str()));
}
//---------------------------------------------
// モデルを描画
//---------------------------------------------
void ObjectBase::DrawModel()
{
	for (int i = 0; i < model_handle.size(); i++) {
		MV1DrawModel(model_handle[i]);
	}
}
//---------------------------------------------
// 値を設定
//---------------------------------------------
void ObjectBase::SetValue()
{
	for (int i = 0; i < model_handle.size();i++) {
		MV1SetPosition(model_handle[i], pos.GetV());
		MV1SetRotationXYZ(model_handle[i], rot.GetV());
		MV1SetScale(model_handle[i], scale.GetV());
	}
}
