#pragma once

class Castle : public ObjectBase
{
public:
	Castle();
	~Castle();
	void Update() override;
	void Draw() override;

	// ��̓����蔻��p�̍��W1
	VECTOR GetCastlePos1()const { return castle_pos1; }
	// ��̓����蔻��p�̍��W2
	VECTOR GetCastlePos2()const { return castle_pos2; }
	// �ő�HP
	int GetMaxHp()const;

private:
	
	//=======================================
	// �ϐ�
	//=======================================

	VECTOR castle_pos1;		// ��̓����蔻��p�̍��W
	VECTOR castle_pos2;		// ��̓����蔻��p�̍��W
};

