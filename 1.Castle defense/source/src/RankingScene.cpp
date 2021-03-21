//---------------------------------------------
// 定数の宣言
//---------------------------------------------
const int RANKING_POS_X = WINDOW_W / 2 + 150;
const int CENTER_POS_X = WINDOW_W / 2;
const int CENTER_POS_Y = WINDOW_H / 2;
const int RANKING_H = 200;
const int RANKING_SIZE = 4;				// ランキング３　計算用に１

//---------------------------------------------
// コンストラクタ
//---------------------------------------------
RankingScene::RankingScene()
{
	scene_type = RANKING;
	select_type = RANKING1;
	letter_type = RANKING_MAX;

	// ランキングファイルの読み込み
	RankingFileRead();

	SetGraphHandle("ranking/ranking1.png", RANKING_POS_X, CENTER_POS_Y - RANKING_H, 0.4f);
	SetGraphHandle("ranking/ranking2.png", RANKING_POS_X, CENTER_POS_Y, 0.4f);
	SetGraphHandle("ranking/ranking3.png", RANKING_POS_X, CENTER_POS_Y + RANKING_H, 0.4f);

	flush = 0;

	font_handle.push_back(CreateFontToHandle(NULL, 90, -1));
	font_handle.push_back(CreateFontToHandle(NULL, 60, -1));
}
//---------------------------------------------
// デストラクタ
//---------------------------------------------
RankingScene::~RankingScene()
{
	
}
//---------------------------------------------
// 更新処理
//---------------------------------------------
void RankingScene::Update()
{
	flush++;

	// セレクトを変える
	ChangeSelect();
	// 決定
	Decision();
	// 詳細の設定
	SetRankingDetails();

	// シーンを戻る
	if (IsKeyOn(KEY_INPUT_C) || IsPadOn(PAD_A)) {
		if (letter_type == RANKING_MAX) {
			scene_type = SELECT;
		}
		else {
			letter_type = RANKING_MAX;
		}
	}
}
//---------------------------------------------
// 描画処理
//---------------------------------------------
void RankingScene::Draw()
{
	// 画像を描画
	DrawGraphHandle();

	if (letter_type == RANKING_MAX) {
		// ランキングの描画
		DrawRanking();
	}
	else {
		// ランキングの詳細の描画
		DrawRankingDetails();
	}
	

	// 戻る文字の描画
	DrawCenterString(CENTER_POS_X + 280, CENTER_POS_Y + 350,"A : 戻る", BLACK,font_handle[1]);

#if (NDEBUG)
	for (int i = 0; i < ranking.size() - 1; i++) {
		printfDx("%d位　%d\n", i + 1, ranking[i]);
	}
#else
#endif
}
//---------------------------------------------
// セレクトを変える
//---------------------------------------------
void RankingScene::ChangeSelect()
{
	// Wキーを押したとき
	if (IsKeyOn(KEY_INPUT_W) || IsPadOn(PAD_UP)) {
		// セレクトタイプをマイナス
		select_type--;
		// セレクトタイプがセレクトロートより小さくなったら
		if (select_type < RANKING1) {
			// セレクトタイトルにする
			select_type = RANKING3;
		}
	}
	// Sキーを押したとき
	if (IsKeyOn(KEY_INPUT_S) || IsPadOn(PAD_DOWN)) {
		// セレクトタイプをプラス
		select_type++;
		// セレクトタイプがセレクトタイトルより大きかったら
		if (select_type > RANKING3) {
			// セレクトロードにする
			select_type = RANKING1;
		}
	}
}
//---------------------------------------------
// 決定
//---------------------------------------------
void RankingScene::Decision()
{
	if ( IsKeyOn(KEY_INPUT_SPACE) || IsPadOn(PAD_B)) {
		if (letter_type != RANKING_MAX) return;
		// ランキング１
		if (select_type == RANKING1) {
			letter_type = RANKING1;
		}
		// ランキング２
		if (select_type == RANKING2) {
			letter_type = RANKING2;
		}
		// ランキング３
		if (select_type == RANKING3) {
			letter_type = RANKING3;
		}
	}
}
//---------------------------------------------
// 文字のカーソルを描画
//---------------------------------------------
void RankingScene::LineCursor(int select_type_)
{
	// 文字の横幅を取得
	int x_ = GetDrawFormatStringWidthToHandle(font_handle[0], "%d", ranking[select_type_]);
	int y_ = 0;
	if (select_type_ == RANKING1) y_ = -RANKING_H + 60;
	if (select_type_ == RANKING2) y_ = 60;
	if (select_type_ == RANKING3) y_ = RANKING_H + 60;
	
	// カウントするタイミング
	if (flush % 40 <= 20) {
		// 文字の下のカーソル
		DrawBox(RANKING_POS_X + 200, CENTER_POS_Y + y_,
			RANKING_POS_X + 200 + x_, CENTER_POS_Y + y_ + 5, BLACK, TRUE);
	}
}
//---------------------------------------------
// ランキングの描画
//---------------------------------------------
void RankingScene::DrawRanking()
{
	// 読み込んだ画像の描画
	
	DrawFormatStringToHandle(RANKING_POS_X + 200, CENTER_POS_Y - RANKING_H - 30, BLACK, font_handle[0], "%d", ranking[0]);
	DrawFormatStringToHandle(RANKING_POS_X + 200, CENTER_POS_Y - 30, BLACK, font_handle[0], "%d", ranking[1]);
	DrawFormatStringToHandle(RANKING_POS_X + 200, CENTER_POS_Y + RANKING_H - 30, BLACK, font_handle[0], "%d", ranking[2]);
	// ラインカーソル
	LineCursor(select_type);
}
//---------------------------------------------
// 詳細の描画
//---------------------------------------------
void RankingScene::DrawRankingDetails()
{
	if(letter_type == RANKING1) DrawSetPosition("ranking/ranking1.png", CENTER_POS_X + 230,140);
	if(letter_type == RANKING2) DrawSetPosition("ranking/ranking2.png", CENTER_POS_X + 230,140);
	if(letter_type == RANKING3) DrawSetPosition("ranking/ranking3.png", CENTER_POS_X + 230,140);

	DrawCenterFormatString(CENTER_POS_X + 280, CENTER_POS_Y - RANKING_H , BLACK, font_handle[1], "HP : %d％", hp[letter_type]);
	DrawCenterFormatString(CENTER_POS_X + 280, CENTER_POS_Y - 30, BLACK, font_handle[1], "撃破数 : %d", defeat_count[letter_type]);
	DrawCenterFormatString(CENTER_POS_X + 280, CENTER_POS_Y + RANKING_H - 30, BLACK, font_handle[1], "SCORE : %d", score[letter_type]);
}
//---------------------------------------------
// 詳細の設定
//---------------------------------------------
void RankingScene::SetRankingDetails()
{
	switch (letter_type)
	{
	case RANKING1:
		DrawSetPosition("ranking/ranking1.png", CENTER_POS_X + 230, 140);
		DrawSetActive("ranking/ranking2.png", false);
		DrawSetActive("ranking/ranking3.png", false);
		break;
	case RANKING2:
		DrawSetPosition("ranking/ranking2.png", CENTER_POS_X + 230, 140);
		DrawSetActive("ranking/ranking1.png", false);
		DrawSetActive("ranking/ranking3.png", false);
		break;
	case RANKING3:
		DrawSetPosition("ranking/ranking3.png", CENTER_POS_X + 230, 140);
		DrawSetActive("ranking/ranking2.png", false);
		DrawSetActive("ranking/ranking1.png", false);
		break;
	default:
		DrawSetPosition("ranking/ranking1.png", RANKING_POS_X, CENTER_POS_Y - RANKING_H);
		DrawSetPosition("ranking/ranking2.png", RANKING_POS_X, CENTER_POS_Y);
		DrawSetPosition("ranking/ranking3.png", RANKING_POS_X, CENTER_POS_Y + RANKING_H);
		DrawSetActive("ranking/ranking1.png", true);
		DrawSetActive("ranking/ranking2.png", true);
		DrawSetActive("ranking/ranking3.png", true);
		break;
	}
}
//---------------------------------------------
// rankingファイル読み込み
//---------------------------------------------
void RankingScene::RankingFileRead()
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
		int ranking_, hp_, defeat_count_, score_;
		sstr >> ranking_ >> hp_ >> defeat_count_ >> score_;
		// ランキングを保存
		ranking.push_back(ranking_);
		hp.push_back(hp_);
		defeat_count.push_back(defeat_count_);
		score.push_back(score_);
	}

	int count = RANKING_SIZE;
	// ファイルに何も書き込まれていなくてランキングに保存されなかった分は
	// すべて　0　で保存
	for (int i = (int)ranking.size(); i < count; i++) {
		ranking.push_back(0);
		hp.push_back(0);
		defeat_count.push_back(0);
		score.push_back(0);
	}

	// ファイルを閉じる
	file.close();
}
