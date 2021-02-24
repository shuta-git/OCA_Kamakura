#pragma once
#include "Pos.h"

//-----------------------------------------------------------
// �萔
//-----------------------------------------------------------
#define ANIMATIONA_SPEED 5

//-----------------------------------------------------------
// �N���X
//-----------------------------------------------------------
class BaseObj :public Pos
{
private:
	
protected:
	int image[5];
	bool dir;
	int ptr;
	float speed;
	int count;
	bool jflag;								// �W�����v�t���O
	bool gflag;								// �d�̓t���O
	float old_x;
	float old_y;
	float move_x;
	float move_y;
	float draw_old_x;
	float draw_x;				// �}�b�v�̃X�N���[����
	float draw_y;				// �}�b�v�̃X�N���[����
	int number;
	int old_number;
	int chara_num;

public:
	BaseObj();
	~BaseObj();
	float GetPosX() const { return x; }				// X���W�̎擾
	float GetPosY() const { return y; }				// Y���W�̎擾
	
	float GetMoveX() const { return move_x; }
	float GetMoveY() const { return move_y; }

	bool GetDir() const { return dir; }
	float GetSpeed() const { return speed; }

	bool GetJflag() const { return jflag; }
	bool GetGflag() const { return gflag; }

	float GetOldX() const { return old_x; }
	float GetOldY() const { return old_y; }

	float GetDrawX() const { return draw_x; }				// X���W�̎擾
	float GetDrawY() const { return draw_y; }				// Y���W�̎擾

	void SetOld();


	



};

