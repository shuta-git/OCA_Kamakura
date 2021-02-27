#pragma once
#include "BaseObj.h"

#define JUMP_UP 1.3F

class PlayerBase : public BaseObj
{

private:

	

public:
	PlayerBase();
	~PlayerBase();

	virtual void Init() = 0;
	virtual void Exit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void ReloadGraph() = 0;
	void SetDrawX() { draw_old_x = draw_x; }
	void ResetDrawX() { draw_x = draw_old_x; }					// ���Z�b�gdraw_X
	void ResetPosX() { x = old_x; }					// ���Z�b�gX
	void ResetPosY() { y = old_y, move_y = 0; }		// ���Z�b�gY
	void ChangeJflag();								// �t���O�`�F���W
	bool MoveCheckRange();					// ������͈͓����`�F�b�N

protected:

	float gspd;
	float jspd;

	virtual void Dir_Change();				// �����̃`�F�b�N
	virtual void Animation();				// �A�j���[�V����
	virtual void Move();					// �ړ�
	void Jump();							// �W�����v
	void Gravity();							// �d��

	virtual void DashDraw() = 0;
	virtual void NormalDraw() = 0;
	virtual int SetDraw() = 0;

	void MoveDrawX();


	

};

