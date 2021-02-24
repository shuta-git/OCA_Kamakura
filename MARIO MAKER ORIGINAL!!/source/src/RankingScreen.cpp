#include "RankingScreen.h"
#include "WinMain.h"

#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
using namespace std;

fstream ranking_screen;

RankingScreen::RankingScreen()
{
	FileRead();
	FileNameRead();
}

RankingScreen::~RankingScreen()
{
}

void RankingScreen::Update()
{
}

void RankingScreen::Draw()
{
	DrawBox(60, 60, 580, 420, GetColor(255, 255, 255), FALSE);
	DrawBox(61, 61, 579, 419, GetColor(0, 0, 0), TRUE);
	DrawString(150,  + 130, "�P��", WHITE, TRUE);
	DrawString(150,  + 230, "�Q��", WHITE, TRUE);
	DrawString(150,  + 330, "�R��", WHITE, TRUE);

	for (int i = 0; i < MAX_RANKING; i++) {
		DrawString(200,(100 * i) + 130,num[i],WHITE,TRUE);
		DrawFormatString(330, (100 * i) + 130, WHITE, "�@�F�@%0.2f", ranking[i], TRUE);
	}
	SetFontSize(20);
	DrawString(400, 380, "��TAB�Ŗ߂�", GetColor(255, 0, 0), TRUE);
}


void RankingScreen::ReloadGraph()
{
}

void RankingScreen::FileRead()
{
	ranking_screen.open("data/txt/ScoreRanking.txt", ios::in);

	if (!ranking_screen.is_open()) {
		// �t�@�C�����J�����ƂɎ��s���܂���
		return;
	}

	string str;
	while (getline(ranking_screen, str))
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
		}

	}

	// �t�@�C�������
	ranking_screen.close();
}

void RankingScreen::FileNameRead()
{
	ranking_screen.open("data/txt/PlayerName.txt", ios::in);

	if (!ranking_screen.is_open()) {
		// �t�@�C�����J�����ƂɎ��s���܂���
		return;
	}

	string str;
	while (getline(ranking_screen, str))
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

	}
	// �t�@�C�������
	ranking_screen.close();
}

