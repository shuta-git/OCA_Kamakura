#include "stdafx.h"
#include <unordered_map>

namespace {
	std::unordered_map<std::string, int> modleHandleTable;
	std::unordered_map<std::string, int> GraphHandleTable;
	std::unordered_map<std::string, int> EffectHandleTable;
}

namespace resource {

//---------------------------------------------
//�@���f���f�[�^��ǂݍ��� 
//---------------------------------------------
int LoadModel(const char* name)
{
	// ���łɓǂݍ��񂾂��̂���f�[�^��������
	if (modleHandleTable[name] != 0) {
		// ���̂܂܂��̃f�[�^��n��
		return modleHandleTable[name];
	}
	// �Ⴄ��������
	// �V�K�f�[�^��ǂݍ���
	int	handle = ::MV1LoadModel(name);
	// �f�[�^��ۑ�
	modleHandleTable[name] = handle;

	return handle;
}
//---------------------------------------------
// �O���t�B�b�N�f�[�^��ǂݍ���
//---------------------------------------------
int LoadGraph(const char* name)
{
	// ���łɓǂݍ��񂾂��̂���f�[�^��������
	if (GraphHandleTable[name] != 0) {
		// ���̂܂܂��̃f�[�^��n��
		return GraphHandleTable[name];
	}
	// �Ⴄ��������
	// �V�K�f�[�^��ǂݍ���
	int	handle = ::LoadGraph(name);
	// �f�[�^��ۑ�
	GraphHandleTable[name] = handle;

	return handle;
}
//---------------------------------------------
// �G�t�F�N�g�f�[�^��ǂݍ���
//---------------------------------------------
int LoadEffekseerEffect(const char * name)
{
	// ���łɓǂݍ��񂾂��̂���f�[�^��������
	if (EffectHandleTable[name] != 0) {
		// ���̂܂܂��̃f�[�^��n��
		return EffectHandleTable[name];
	}
	// �Ⴄ��������
	// �V�K�f�[�^��ǂݍ���
	int	handle = ::LoadEffekseerEffect(name);
	// �f�[�^��ۑ�
	EffectHandleTable[name] = handle;

	return handle;
}

//---------------------------------------------
// unordered_map�ŕۑ������f�[�^�����ׂď���
//---------------------------------------------
void DeleteHandleTable() {
	for (auto &handle: modleHandleTable) {
		::MV1DeleteModel(handle.second);
	}
	modleHandleTable.clear();
	for (auto &handle : GraphHandleTable) {
		::DeleteGraph(handle.second);
	}
	GraphHandleTable.clear();
	for (auto &handle : EffectHandleTable) {
		::DeleteEffekseerEffect(handle.second);
	}
	EffectHandleTable.clear();
}

}
