#pragma once
#include "CharaOperationBase.h"
#include "ScreenBase.h"
#include "Cursor.h"


class CharaOperation : public ScreenBase
{
public:
	CharaOperation(int chara_num_);
	~CharaOperation();

	void Update();
	void Update(int chara_num_);
	void Draw();
	void ReloadGraph();

private:
	Cursor * cur = nullptr;
	CharaOperationBase * charaoperationbase;

	void Screen_Set();			//	キャラクターセット
	void Screen_Delete();		//  セットしたキャラクターでない時デリート処理

};

