#pragma once
#include "PlayerBase.h"

//-----------------------------------------------------------
// �萔
//-----------------------------------------------------------
#define PLAYER_MAX 3
#define ADD_SCORE 5.00

//-----------------------------------------------------------
// �N���X
//-----------------------------------------------------------
class Player : public BaseObj
{
public:
	Player();
	~Player();
	void Init();
	void Exit();
	void Update();
	void Draw();
	void ReloadGraph();

	void ChangeJflag();
	void ResetPosX();
	void ResetDrawX();
	void ResetPosY();

	void AddScore();
	void TimerScore(double timer_);

	double GetScore() const { return score; }
	int GetNum() const { return number; }
	bool GetJflag()const;

	bool Check_Map_Range();

private:
	int number;
	int old_number;
	double score;
	void Character_Set();			//	�L�����N�^�[�Z�b�g
	void Character_Delete();		//  �Z�b�g�����L�����N�^�[�łȂ����f���[�g����
	void Character_Change();		//  �L�����N�^�[�`�F���W
	void Pos_Storage();				//  ���W��ۑ�
	void Pos_Old_Storage();			//  old���W��ۑ�
	void Dir_Storage();				//  ������ۑ�
	void Speed_Storage();			//  �X�s�[�h��ۑ�
	void Draw_Storage();			//  draw���W��ۑ�
	

	PlayerBase* playerbase;

};



















