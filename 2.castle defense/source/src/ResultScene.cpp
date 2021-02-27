#include "ResultScene.h"
//---------------------------------------------
// 定数の宣言
//---------------------------------------------
const int CENTER_POS_X = WINDOW_W / 2;
const int CENTER_POS_Y = WINDOW_H / 2;
const int RANKING_SIZE = 4;				// ランキング３　計算用に１

//---------------------------------------------
// コンストラクタ
//---------------------------------------------
ResultScene::ResultScene()
{
	// シーンタイプ
	scene_type = RESULT;
	// リザルトファイル読み込み
	ResultFileRead();
	// ランキングファイル読み込み
	RankingFileRead();
	// スコアのトータルを計算
	TotalCalculation();

	if (result) { // win
		SetGraphHandle("result/win/win_background.png",CENTER_POS_X, CENTER_POS_Y);
		SetGraphHandle("result/win/win_Foundation.png", CENTER_POS_X + 400, CENTER_POS_Y, 0.4f);
		SetGraphHandle("result/win/win_character.png", CENTER_POS_X + 400, CENTER_POS_Y - 150, 0.3f);
		SetGraphHandle("result/win/win_letter.png", CENTER_POS_X - 450, CENTER_POS_Y - 350, 0.4f);
		SetGraphHandle("result/win/win_frame.png", CENTER_POS_X, CENTER_POS_Y,0.4f);
	}
	else {// lose
		SetGraphHandle("result/lose/lose_background.png", CENTER_POS_X, CENTER_POS_Y);
		SetGraphHandle("result/lose/lose_spotlight.png", CENTER_POS_X + 250, CENTER_POS_Y - 100, 0.4f);
		SetGraphHandle("result/lose/lose_skeleton.png", CENTER_POS_X + 400, CENTER_POS_Y + 50, 0.38f);
		SetGraphHandle("result/lose/lose_letter.png", CENTER_POS_X - 350, CENTER_POS_Y - 350, 0.4f);
		SetGraphHandle("result/lose/lose_frame.png", CENTER_POS_X, CENTER_POS_Y, 0.4f);
	}

	font_handle.push_back(CreateFontToHandle(NULL, 90, -1));
}
//---------------------------------------------
// デストラクタ
//---------------------------------------------
ResultScene::~ResultScene()
{

}
//---------------------------------------------
// 更新処理
//---------------------------------------------
void ResultScene::Update()
{
	if (IsKeyOn(KEY_INPUT_SPACE) || IsPadOn(PAD_B)) {
		// ランキング画面に移動
		scene_type = SELECT;
		// ランキングの計算
		RankingCalculation();
		// ランキングファイルにランキングを書き込み
		RankingFileWrite();
	}
}
//---------------------------------------------
// 描画処理
//---------------------------------------------
void ResultScene::Draw()
{
	// 画像の描画
	DrawGraphHandle();

	unsigned int color;
	if (result) { // win
		color = BLACK;
	}
	else {	// lose
		color = GetColor(169, 190, 244);
	}
	// HP文字の表示
	DrawCenterFormatString(600, 400, color, font_handle[0], "HP : %d％", hp);
	// 撃破数の表示
	DrawCenterFormatString(600, 550, color, font_handle[0], "撃破数 : %d", defeat_count);
	// 合計スコアの表示
	DrawFormatStringToHandle(300, 800, color, font_handle[0], "TOTAL : %d", total);
	DrawBox(300, 700, 900, 710, color, TRUE);


#if (NDEBUG)
	for (int i = 0; i < ranking.size(); i++) {
		printfDx("%d位 %d %d %d %d \n", i + 1, ranking[i],hp_v[i],defeat_count_v[i],score_v[i]);
	}
#else
#endif
}
//---------------------------------------------
// resultファイル読み込み
//---------------------------------------------
void ResultScene::ResultFileRead()
{
	file.open("data/txt/Result.txt", ios::in);

	if (!file.is_open()) {
		// ファイルを開くことに失敗しました
		return;
	}

	string str;
	while (getline(file, str))
	{
		stringstream sstr(str);
		// winかloseか　スコア　HP　撃破数
		sstr >> result >> score >> hp >> defeat_count;
	}

	// ファイルを閉じる
	file.close();
}
//---------------------------------------------
// rankingファイル読み込み
//---------------------------------------------
void ResultScene::RankingFileRead()
{
	file.open("data/txt/Ranking.txt", ios::in);

	if (!file.is_open()) {
		// ファイルを開くことに失敗しました
		return;
	}

	string str;
	while (getline(file, str))
	{
		stringstream sstr(str);
		int ranking_ ,hp_,defeat_count_, score_;
		sstr >> ranking_ >> hp_ >> defeat_count_ >> score_;
		// ランキングを保存
		ranking.push_back(ranking_);
		hp_v.push_back(hp_);
		defeat_count_v.push_back(defeat_count_);
		score_v.push_back(score_);
	}

	int count = RANKING_SIZE;
	// ファイルに何も書き込まれていなくてランキングに保存されなかった分は
	// すべて　0　で保存
	for (int i = (int)ranking.size(); i < count;i++) {
		ranking.push_back(0);
		hp_v.push_back(0);
		defeat_count_v.push_back(0);
		score_v.push_back(0);
	}

	// ファイルを閉じる
	file.close();
}
//---------------------------------------------
// rankingファイル書き込み
//---------------------------------------------
void ResultScene::RankingFileWrite()
{
	file.open("data/txt/Ranking.txt", ios::out);

	if (!file.is_open()) {
		// ファイルを開くことに失敗しました
		return;
	}
	
	// ランキングをファイルに書き込み
	for (int i = 0; i < (int)ranking.size() - 1; i++) {
		file << ranking[i]<< ' ' << hp_v[i]<<' '<<defeat_count_v[i]<<' '<<score_v[i]<< '\n';
	}

	// ファイルを閉じる
	file.close();
}
//---------------------------------------------
// ランキングの計算
//---------------------------------------------
void ResultScene::RankingCalculation()
{
	// ランキングを下から見ていく
	for (int i = (int)ranking.size() - 2; i > -1; --i) {
		// スコア合計の方が大きかったら
		if (ranking[i] < total) {
			// ランキングに下に繰り下げ
			ranking[i + 1] = ranking[i];
			hp_v[i + 1] = hp_v[i];
			defeat_count_v[i + 1] = defeat_count_v[i];
			score_v[i + 1] = score_v[i];
			// ランキングに登録
			ranking[i] = total;
			hp_v[i] = hp;
			defeat_count_v[i] = defeat_count;
			score_v[i] = score;
		}
	}
}
//---------------------------------------------
// トータルの計算
//---------------------------------------------
void ResultScene::TotalCalculation()
{
	if (result) {
		total += 10000;
	}
	total += hp * 123;
	total += defeat_count * 100;
	total += score;
}
