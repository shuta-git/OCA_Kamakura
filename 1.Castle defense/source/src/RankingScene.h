#pragma once

class RankingScene : public SceneBase
{
public:
	RankingScene();
	~RankingScene();

	void Update() override;
	void Draw() override;

private:

	// ファイル読み込み用
	fstream file;

	std::vector<int>ranking;		// ランキング
	std::vector<int>score;			// スコア
	std::vector<int>defeat_count;	// 撃破数
	std::vector<int>hp;				// HP

	int flush;				// フラッシュ
	int select_type;		// 選択している種類
	int letter_type;		// 文字の種類

	// セレクトを変える
	void ChangeSelect();
	// 決定
	void Decision();
	// 文字のカーソルを描画
	void LineCursor(int select_type_);

	// ランキングの描画
	void DrawRanking();
	// 詳細の描画
	void DrawRankingDetails();
	// 詳細の設定
	void SetRankingDetails();
	// rankingファイル読み込み
	void RankingFileRead();

	enum RankingLetter {
		RANKING1 = 0,
		RANKING2,
		RANKING3,

		RANKING_MAX,
	};
};

