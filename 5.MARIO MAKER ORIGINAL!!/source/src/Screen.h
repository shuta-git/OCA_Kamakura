#pragma once
#include "BaseObj.h"
#include "ScreenBase.h"

//-----------------------------------------------------------
// �N���X
//-----------------------------------------------------------
class Screen : public BaseObj
{
public:
	Screen();
	Screen(int num_,int chara_num_);
	~Screen();

	void Exit();
	void Update(int num_, int chara_num_);
	void Draw();
	void  ReloadGraph();

	void SetPlayer(Player* player,double timer_);

private:
	double timer = 0.00;
	double score = 0.00;
	void Screen_Set();			//	�L�����N�^�[�Z�b�g
	void Screen_Delete();		//  �Z�b�g�����L�����N�^�[�łȂ����f���[�g����

	ScreenBase * screenbase;
};


