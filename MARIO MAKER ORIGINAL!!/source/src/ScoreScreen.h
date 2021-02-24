#pragma once
#include "ScreenBase.h"

#define MAX_RANKING 3
#define MAX_NAME 20



class ScoreScreen :
	public ScreenBase
{
public:
	ScoreScreen(double score_,double timer_);
	~ScoreScreen();

	void Update();
	void Draw();
	void ReloadGraph();

private:

	int count;
	int InputHandle;

	char name[MAX_NAME];
	char num[MAX_RANKING][MAX_NAME];

	double score;
	double timer;

	double ranking[MAX_RANKING];
	double old_ranking[MAX_RANKING];

	void FileRead();			// ファイルの読み込み
	void FileSave();			// ファイルセーブ
	void FileNameRead();			// ファイルの読み込み
	void FileNameSave();
};




