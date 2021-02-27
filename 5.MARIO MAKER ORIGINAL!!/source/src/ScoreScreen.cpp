#include "ScoreScreen.h"
#include "WinMain.h"

#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
using namespace std;

fstream score_screen;

//-----------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------
ScoreScreen::ScoreScreen(double score_, double timer_)
{
	count = 0;
	score = score_;
	timer = timer_;
	FileRead();
	FileSave();
	int w = GetColor(255, 255, 255);

	InputHandle = MakeKeyInput(80, FALSE, FALSE, FALSE);
	SetActiveKeyInput(InputHandle);
	SetKeyInputStringColor(w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w);
}
//-----------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------
ScoreScreen::~ScoreScreen()
{
	DeleteKeyInput(InputHandle);
}
//-----------------------------------------------------------
// �X�V
//-----------------------------------------------------------
void ScoreScreen::Update()
{
	if (CheckKeyInput(InputHandle) != 1) {
		return;
	}
	if (InputHandle != 1) {
		GetKeyInputString(name, InputHandle);
	}
	DeleteKeyInput(InputHandle);
	InputHandle = -1;
}
//-----------------------------------------------------------
// �`��
//-----------------------------------------------------------
void ScoreScreen::Draw()
{
	DrawBox(60, 60, 580, 420, GetColor(255, 255, 255), FALSE);
	DrawBox(61, 61, 579, 419, GetColor(0, 0, 0), TRUE);
	SetFontSize(25);
	DrawFormatString(225, 100, GetColor(255, 255, 255), "�X�R�A �F %0.2f", score);
	DrawFormatString(225, 200, GetColor(255, 255, 255), "�^�C�� �F %0.2f", timer);
	if (count == 1) {
		SetFontSize(50);
		DrawFormatString(200, 300, GetColor(255, 0, 0), "HIGH SCORE!", score);
		
	}


	DrawKeyInputString(240, 350, InputHandle);
	if (InputHandle == -1) {
		DrawString(240, 350, name, WHITE, TRUE);
		FileNameRead();
		FileNameSave();
	}
}
void ScoreScreen::ReloadGraph()
{
}

//-----------------------------------------------------------
//�@�X�R�A�ǂݍ���
//-----------------------------------------------------------
void ScoreScreen::FileRead()
{
	score_screen.open("data/txt/ScoreRanking.txt", ios::in);

	if (!score_screen.is_open()) {
		// �t�@�C�����J�����ƂɎ��s���܂���
		return;
	}

	string str;
	while (getline(score_screen, str))
	{
		while (true) {
			string::size_type pos = str.find(',', 0);
			if (pos == string::npos) break;
			str.replace(pos, 1, " ");	// replace=�u������/ replace(�ꏊ,������,�Ȃ�ĕ�����)
		}

		stringstream sstr(str);
		double score_;
		for (int i = 0; i < MAX_RANKING; i++) {
			sstr >> score_;
			ranking[i] = score_;
			old_ranking[i] = score_;
		}

		for (int i = MAX_RANKING - 1; i > 0 - 1; i--) {
			if (ranking[i] < score) {
				if (i != MAX_RANKING - 1) {
					ranking[i + 1] = ranking[i];
					ranking[i] = score;
				}
			}
			else {
				break;
			}
		}
		
	}

	// �t�@�C�������
	score_screen.close();
}
//-----------------------------------------------------------
// �X�R�A�Z�[�u
//-----------------------------------------------------------
void ScoreScreen::FileSave()
{
	score_screen.open("data/txt/ScoreRanking.txt", ios::out);

	if (!score_screen.is_open()) {
		// �t�@�C�����J�����ƂɎ��s���܂���
		return;
	}

	for (int i = 0; i < MAX_RANKING; i++) {
		score_screen << ranking [i]<<",";
	}

	// �t�@�C�������
	score_screen.close();
}




//-----------------------------------------------------------
// ���O�ǂݍ���
//-----------------------------------------------------------
void ScoreScreen::FileNameRead()
{
	score_screen.open("data/txt/PlayerName.txt", ios::in);

	if (!score_screen.is_open()) {
		// �t�@�C�����J�����ƂɎ��s���܂���
		return;
	}

	string str;
	while (getline(score_screen, str))
	{
		while (true) {
			string::size_type pos = str.find(',', 0);
			if (pos == string::npos) break;
			str.replace(pos, 1, " ");	// replace=�u������/ replace(�ꏊ,������,�Ȃ�ĕ�����)
		}

		stringstream sstr(str);
		char name_[MAX_NAME];



		for (int i = 0; i < MAX_RANKING; i++) {
			sstr >> name_;
			for (int x = 0; x < MAX_NAME; x++) {
				num[i][x] = name_[x];
			}
		}

		for (int i = MAX_RANKING - 1; i > 0 - 1; i--) {
			if (old_ranking[i] < score) {
				if (i != MAX_RANKING - 1) {
					for (int x = 0; x < MAX_NAME; x++) {
						num[i + 1][x] = num[i][x];
						num[i][x] = name[x];
					}
				}
				if (i != MAX_RANKING - 1) {
					old_ranking[i + 1] = old_ranking[i];
					old_ranking[i] = score;
				}
			}
			else {
				break;
			}
		}
	}

	// �t�@�C�������
	score_screen.close();
}
//-----------------------------------------------------------
// ���O�Z�[�u
//-----------------------------------------------------------
void ScoreScreen::FileNameSave()
{
	score_screen.open("data/txt/PlayerName.txt", ios::out);

	if (!score_screen.is_open()) {
		// �t�@�C�����J�����ƂɎ��s���܂���
		return;
	}
	
	for (int i = 0; i < MAX_RANKING; i++) {
		score_screen << num[i]<< ",";
	}
	
	//ranking_score <<  name <<",";
	
	// �t�@�C�������
	score_screen.close();
}
