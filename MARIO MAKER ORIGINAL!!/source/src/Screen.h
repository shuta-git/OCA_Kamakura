#pragma once
#include "BaseObj.h"
#include "ScreenBase.h"

//-----------------------------------------------------------
// クラス
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
	void Screen_Set();			//	キャラクターセット
	void Screen_Delete();		//  セットしたキャラクターでない時デリート処理

	ScreenBase * screenbase;
};


