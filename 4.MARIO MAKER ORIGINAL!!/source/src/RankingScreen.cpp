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
	DrawString(150,  + 130, "１位", WHITE, TRUE);
	DrawString(150,  + 230, "２位", WHITE, TRUE);
	DrawString(150,  + 330, "３位", WHITE, TRUE);

	for (int i = 0; i < MAX_RANKING; i++) {
		DrawString(200,(100 * i) + 130,num[i],WHITE,TRUE);
		DrawFormatString(330, (100 * i) + 130, WHITE, "　：　%0.2f", ranking[i], TRUE);
	}
	SetFontSize(20);
	DrawString(400, 380, "※TABで戻る", GetColor(255, 0, 0), TRUE);
}


void RankingScreen::ReloadGraph()
{
}

void RankingScreen::FileRead()
{
	ranking_screen.open("data/txt/ScoreRanking.txt", ios::in);

	if (!ranking_screen.is_open()) {
		// ファイルを開くことに失敗しました
		return;
	}

	string str;
	while (getline(ranking_screen, str))
	{
		while (true) {
			string::size_type pos = str.find(',', 0);
			if (pos == string::npos) break;
			str.replace(pos, 1, " ");	// replace=置き換え/ replace(場所,文字数,なんて文字に)
		}

		stringstream sstr(str);
		double score_;
		for (int i = 0; i < MAX_RANKING; i++) {
			sstr >> score_;
			ranking[i] = score_;
		}

	}

	// ファイルを閉じる
	ranking_screen.close();
}

void RankingScreen::FileNameRead()
{
	ranking_screen.open("data/txt/PlayerName.txt", ios::in);

	if (!ranking_screen.is_open()) {
		// ファイルを開くことに失敗しました
		return;
	}

	string str;
	while (getline(ranking_screen, str))
	{
		while (true) {
			string::size_type pos = str.find(',', 0);
			if (pos == string::npos) break;
			str.replace(pos, 1, " ");	// replace=置き換え/ replace(場所,文字数,なんて文字に)
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
	// ファイルを閉じる
	ranking_screen.close();
}

