#include "ObjectBase.h"

//---------------------------------------------
// �R���X�g���N�^
//---------------------------------------------
ObjectBase::ObjectBase()
{
	pos = {0.0f,0.0f,0.0f};
	scale = { 1.0f, 1.0f, 1.0f };
	rot = { 0.0f, 0.0f, 0.0f };
}
//---------------------------------------------
// �f�X�g���N�^
//---------------------------------------------
ObjectBase::~ObjectBase()
{
}
//---------------------------------------------
// �e�N�X�`���[��K�p
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
// �e�N�X�`���[�̃p�X���擾
//---------------------------------------------
void ObjectBase::SetTextureHandle(string str)
{
	// �p�X��ݒ�
	string data = "data/map/Texture/";
	string path = data + str;
	texture_handle.push_back(resource::LoadGraph(path.c_str()));
}
//---------------------------------------------
// ���f���̃p�X��ݒ�
//---------------------------------------------
void ObjectBase::SetModelHandle(string str)
{
	// �p�X��ݒ�
	string data = "data/map/";
	string path = data + str;
	model_handle.push_back(resource::LoadModel(path.c_str()));
}
//---------------------------------------------
// ���f����`��
//---------------------------------------------
void ObjectBase::DrawModel()
{
	for (int i = 0; i < model_handle.size(); i++) {
		MV1DrawModel(model_handle[i]);
	}
}
//---------------------------------------------
// �l��ݒ�
//---------------------------------------------
void ObjectBase::SetValue()
{
	for (int i = 0; i < model_handle.size();i++) {
		MV1SetPosition(model_handle[i], pos.GetV());
		MV1SetRotationXYZ(model_handle[i], rot.GetV());
		MV1SetScale(model_handle[i], scale.GetV());
	}
}
