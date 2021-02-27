#include "CharaOperation.h"
#include "WinMain.h"
#include "FionaOperationScreen.h"
#include "MusyaOperation.h"
#include "NininOperation.h"

#define CHARAOPERATION_MAX 3

//-----------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------
CharaOperation::CharaOperation(int chara_num_)
{
	chara_num = chara_num_;
	
	if (cur == nullptr) {
		cur = new Cursor();
	}
	if (charaoperationbase != nullptr) {
		charaoperationbase = nullptr;
	}
	image[0] = LoadGraph("data/playerimage/fiona.png");
	image[1] = LoadGraph("data/playerimage/musya.png");
	image[2] = LoadGraph("data/playerimage/ninin.png");
}
//-----------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------
CharaOperation::~CharaOperation()
{
	if (cur != nullptr) {
		delete cur;
		cur = nullptr;
	}
	if (charaoperationbase != nullptr) {
		delete charaoperationbase;
		charaoperationbase = nullptr;
	}
	for (int i = 0; i < CHARAOPERATION_MAX; i++) {
		DeleteGraph(image[i]);
	}
}
//-----------------------------------------------------------
// 更新
//-----------------------------------------------------------
void CharaOperation::Update()
{
}
void CharaOperation::Update(int chara_num_)
{
	chara_num = chara_num_;
	Screen_Delete();
	old_number = chara_num;
	Screen_Set();
}
//-----------------------------------------------------------
//	描画
//-----------------------------------------------------------
void CharaOperation::Draw()
{
	if (charaoperationbase != nullptr) {
		charaoperationbase->Draw();
	}
	if (charaoperationbase == nullptr) {
		if (cur != nullptr) {
			cur->Move();
			// fiona
			if (cur->GetX() >= 49 && cur->GetY() >= 149 &&
				cur->GetX() <= 191 && cur->GetY() <= 351) {
				DrawBox(49, 149, 191, 351, GetColor(255, 255, 0), FALSE);
			}
			else {
				DrawBox(49, 149, 191, 351, GetColor(255, 255, 255), FALSE);
			}
			// musya
			if (cur->GetX() >= 249 && cur->GetY() >= 149 &&
				cur->GetX() <= 391 && cur->GetY() <= 351) {
				DrawBox(249, 149, 391, 351, GetColor(255, 255, 0), FALSE);
			}
			else {
				DrawBox(249, 149, 391, 351, GetColor(255, 255, 255), FALSE);
			}
			// ninin
			if (cur->GetX() >= 449 && cur->GetY() >= 149 &&
				cur->GetX() <= 591 && cur->GetY() <= 351) {
				DrawBox(449, 149, 591, 351, GetColor(255, 255, 0), FALSE);
			}
			else {
				DrawBox(449, 149, 591, 351, GetColor(255, 255, 255), FALSE);
			}
		}

		// fiona
		DrawBox(50, 150, 190, 350, GetColor(0, 0, 0), TRUE);
		// musya
		DrawBox(250, 150, 390, 350, GetColor(0, 0, 0), TRUE);
		// ninin
		DrawBox(450, 150, 590, 350, GetColor(0, 0, 0), TRUE);

		DrawGraph(90, 180, image[0], TRUE);
		DrawGraph(260, 180, image[1], TRUE);
		DrawGraph(450, 180, image[2], TRUE);

		SetFontSize(50);
		DrawString(50, 100, "1.", GetColor(0, 0, 0), TRUE);
		DrawString(250, 100, "2.", GetColor(0, 0, 0), TRUE);
		DrawString(450, 100, "3.", GetColor(0, 0, 0), TRUE);
		SetFontSize(25);
		DrawString(420, 430, "※TABで戻る", GetColor(255, 0, 0), TRUE);
	}
}

void CharaOperation::ReloadGraph()
{
	image[0] = LoadGraph("data/playerimage/fiona.png");
	image[1] = LoadGraph("data/playerimage/musya.png");
	image[2] = LoadGraph("data/playerimage/ninin.png");
}

void CharaOperation::Screen_Set()
{
	switch (chara_num)
	{
	case FIONA_OPERATION:
		if (charaoperationbase == nullptr) {
			charaoperationbase = new FionaOperationScreen();
		}
		break;
	case MUSYA_OPERATION:
		if (charaoperationbase == nullptr) {
			charaoperationbase = new MusyaOperation();
		}
		break;
	case NININ_OPERATION:
		if (charaoperationbase == nullptr) {
			charaoperationbase = new NininOperation();
		}
		break;
	default:
		break;
	}
}

void CharaOperation::Screen_Delete()
{
	if (chara_num != old_number) {
		if (charaoperationbase != nullptr) {
			delete charaoperationbase;
			charaoperationbase = nullptr;
		}
	}
}
