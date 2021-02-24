#include "ScoreScreen.h"
#include "WinMain.h"

#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
using namespace std;

fstream score_screen;

//-----------------------------------------------------------
// コンストラクタ
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
// デストラクタ
//-----------------------------------------------------------
ScoreScreen::~ScoreScreen()
{
	DeleteKeyInput(InputHandle);
}
//-----------------------------------------------------------
// 更新
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
// 描画
//-----------------------------------------------------------
void ScoreScreen::Draw()
{
	DrawBox(60, 60, 580, 420, GetColor(255, 255, 255), FALSE);
	DrawBox(61, 61, 579, 419, GetColor(0, 0, 0), TRUE);
	SetFontSize(25);
	DrawFormatString(225, 100, GetColor(255, 255, 255), "スコア ： %0.2f", score);
	DrawFormatString(225, 200, GetColor(255, 255, 255), "タイム ： %0.2f", timer);
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
//　スコア読み込み
//-----------------------------------------------------------
void ScoreScreen::FileRead()
{
	score_screen.open("data/txt/ScoreRanking.txt", ios::in);

	if (!score_screen.is_open()) {
		// ファイルを開くことに失敗しました
		return;
	}

	string str;
	while (getline(score_screen, str))
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

	// ファイルを閉じる
	score_screen.close();
}
//-----------------------------------------------------------
// スコアセーブ
//-----------------------------------------------------------
void ScoreScreen::FileSave()
{
	score_screen.open("data/txt/ScoreRanking.txt", ios::out);

	if (!score_screen.is_open()) {
		// ファイルを開くことに失敗しました
		return;
	}

	for (int i = 0; i < MAX_RANKING; i++) {
		score_screen << ranking [i]<<",";
	}

	// ファイルを閉じる
	score_screen.close();
}




//-----------------------------------------------------------
// 名前読み込み
//-----------------------------------------------------------
void ScoreScreen::FileNameRead()
{
	score_screen.open("data/txt/PlayerName.txt", ios::in);

	if (!score_screen.is_open()) {
		// ファイルを開くことに失敗しました
		return;
	}

	string str;
	while (getline(score_screen, str))
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

	// ファイルを閉じる
	score_screen.close();
}
//-----------------------------------------------------------
// 名前セーブ
//-----------------------------------------------------------
void ScoreScreen::FileNameSave()
{
	score_screen.open("data/txt/PlayerName.txt", ios::out);

	if (!score_screen.is_open()) {
		// ファイルを開くことに失敗しました
		return;
	}
	
	for (int i = 0; i < MAX_RANKING; i++) {
		score_screen << num[i]<< ",";
	}
	
	//ranking_score <<  name <<",";
	
	// ファイルを閉じる
	score_screen.close();
}
