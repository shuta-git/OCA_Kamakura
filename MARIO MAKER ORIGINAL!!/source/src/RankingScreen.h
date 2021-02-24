#pragma once
#include "ScreenBase.h"

#define MAX_RANKING 3
#define MAX_NAME 20

class RankingScreen : public ScreenBase
{
public:
	RankingScreen();
	~RankingScreen();

	void Update();
	void Draw();
	void ReloadGraph();
private:

	double score[MAX_RANKING];

	double ranking[MAX_RANKING];
	char num[MAX_RANKING][MAX_NAME];


	void FileRead();			// �t�@�C���̓ǂݍ���
	void FileNameRead();			// �t�@�C���̓ǂݍ���
};

