#pragma once

class ResultScene : public SceneBase
{
public:
	ResultScene();
	~ResultScene();

	void Update() override;
	void Draw() override;

private:

	// ファイル読み込み用
	fstream file;

	bool result;		// クリアか失敗か
	int score;			// スコア
	int defeat_count;	// 撃破カウント
	int hp;				// 城の残りHP
	int total;			// 合計
	
	std::vector<int>ranking;		// ランキング
	std::vector<int>score_v;		// スコア
	std::vector<int>defeat_count_v;	// 撃破数
	std::vector<int>hp_v;			// HP

	// resultファイル読み込み
	void ResultFileRead();
	// rankingファイル読み込み
	void RankingFileRead();
	// rankingファイル書き込み
	void RankingFileWrite();
	// ランキングの計算
	void RankingCalculation();
	// トータルの計算
	void TotalCalculation();
};

