#pragma once

//---------------------------------------------
// MapManager�N���X
//---------------------------------------------
class MapManager
{
public:
	MapManager();
	~MapManager();

	void Update();
	void Draw();

	// Castle�^�ŏ��Ԃ�
	Castle* GetCastle()const;

private:
	
	// �I�u�W�F�N�g�𐶐�
	void Create();
	// ����
	void Erase();

	//=======================================
	// �e�N�X�`���[
	//=======================================
	
	std::vector<ObjectBase*>object;		// �}�b�v�e���v���[�g

	Vector3 pos;	// ���W
	Vector3 scale;	// �T�C�Y
	Vector3 rot;	// ��]

};

