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

	void Screen_Set();			//	�L�����N�^�[�Z�b�g
	void Screen_Delete();		//  �Z�b�g�����L�����N�^�[�łȂ����f���[�g����

};

