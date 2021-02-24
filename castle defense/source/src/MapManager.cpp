#include "MapManager.h"

//---------------------------------------------
// �R���X�g���N�^
//---------------------------------------------
MapManager::MapManager()
{
	// ��
	object.push_back(new Castle());
}
//---------------------------------------------
// �f�X�g���N�^
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
// �X�V
//---------------------------------------------
void MapManager::Update()
{
	for (int i = 0; i < object.size(); i++) {
		object[i]->Update();
	}
}
//---------------------------------------------
// �`��
//---------------------------------------------
void MapManager::Draw()
{
	for (int i = 0; i < object.size(); i++) {
		object[i]->Draw();
		
	}
}
//---------------------------------------------
// Castle�^�ŏ��Ԃ�
//---------------------------------------------
Castle * MapManager::GetCastle() const
{
	if (object[0] == nullptr)return nullptr;
	return (Castle*)object[0];
}
//---------------------------------------------
// �I�u�W�F�N�g�𐶐�
//---------------------------------------------
void MapManager::Create()
{
	// ��
	object.push_back(new Castle());
}
//---------------------------------------------
// ����
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
