#pragma once
#include "Main.h"

class Player 
{
private:
	Vec2 m_pos;		// �ړ��p�̕ϐ�
	Vec2 p_pos;	    // ���W�p�̕ϐ�
	int p_rot;		// ���a

	PhysicsWorld world;		// �������Z�ݒ�
	PhysicsBody player;		// �������Z�p�̌^���쐬

	// �W�����v�t���O
	bool jump_f;
public:
	Player(PhysicsWorld w);
	~Player();

	//----------------
	// Get�֐�
	//----------------
	double GetPosX() const { return p_pos.x; }			// X���W���擾
	double GetPosY() const { return p_pos.y; }			// Y���W���擾
	double GetMPosX()const { return m_pos.x; }
	Vec2 GetPos() const { return p_pos; }				// Vec2�ō��W���擾
	int GetRot() const { return p_rot; }				// ���a�����擾
	PhysicsBody GetBody() const { return player; }		// PhysicsBody���擾
	
	// �W�����v�t���O��ς���
	void CahegFlag();

	//----------------
	// �K��֐�
	//----------------
	void Init();
	void Update();
	void Draw();

	void InitM_Pos();

private:

	//----------------
	// �R���g���[���[
	//----------------
	void JumpCon();			// �W�����v
	void JumpKey();			// �W�����v
	void MoveCon();			// �ړ�
	void MoveKey();			// �ړ�
	void Gravity();			// �d��
};

