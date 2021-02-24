#include "GameMain.h"
//---------------------------------------------
// 定数の宣言
//---------------------------------------------
const int CASTLE_LIFEGAUGE_X = WINDOW_W / 2;	// 城のライフゲージの座標X（ライフゲージに中心）
const int CASTLE_LIFEGAUGE_Y = 80;				// 城のライフゲージの座標Y（ライフゲージに中心）
const int CASTLE_LIFEGAUGE_W = 225;				// 城のラーフゲージの横幅
const int CASTLE_LIFEGAUGE_H = 30;				// 城のライフゲージの縦幅
const int MAX_TIME = 150;						// 最大時間 150
const int SCORE = 100;							// スコア
const int CLEAR_MAX_TIME = 300;					// クリア時の最大時間
const int FAIL_MAX_TIME = 60;					// ゲームオーバー時の最大時間
const int ADD_MP = 3;							// MP回復量

//---------------------------------------------
// コンストラクタ
//---------------------------------------------
GameMain::GameMain()
{
	// フォグを有効にする
	SetFogEnable(TRUE);
	// フォグの色を白色にする
	SetFogColor(255, 255, 255);
	// フォグの開始距離を０、終了距離を130にする
	SetFogStartEnd(0.0f, 130.0f);

	// ライフゲージの読み込み
	SetGraphHandle("UI/LifeGauge.png", CASTLE_LIFEGAUGE_X, CASTLE_LIFEGAUGE_Y, 0.4f);
	SetGraphHandle("UI/pause.png", WINDOW_W - 70,60,0.07f);
	// スカイドーム
	skydome = resource::LoadModel("data/scene/skydome/dome321.X");

	if (map_m == nullptr)	 { map_m = new MapManager(); }
	if (player_m == nullptr) { player_m = new PlayerManager(); }
	if (enemy_m == nullptr)  { enemy_m = new EnemyManager(); }

	isUpdata = true;
	isOneUpdate = false;
	pause = false;
	// ファイル読み込み
	Pos1FileRead();
	Pos2FileRead();
	RadiusFileRead();

	scene_type = GAME_MAIN;
	enemy_pos = { 0.0f,0.0f,0.0f };
	
	play_timer = MAX_TIME;
	start_time = GetNowCount();
	old_time = start_time;
	count_time = 0;
	defeat_count = 0;
	result = false;

	clear = false;

	// HPの文字
	font_handle.push_back(CreateFontToHandle(NULL, 30, -1));
	// タイムの文字
	font_handle.push_back(CreateFontToHandle(NULL, 50, -1));
	// ガイドの文字
	font_handle.push_back(CreateFontToHandle(NULL, 40, -1));
	// クリア文字
	font_handle.push_back(CreateFontToHandle(NULL, 90, -1));
	// ポーズ
	font_handle.push_back(CreateFontToHandle(NULL, 20, -1));
}
//---------------------------------------------
// デストラクタ
//---------------------------------------------
GameMain::~GameMain()
{
	if (map_m != nullptr) {
		delete map_m;
		map_m = nullptr;
	}
	if (player_m != nullptr) { 
		delete player_m;
		player_m = nullptr;
	}
	if (enemy_m != nullptr) {
		delete enemy_m;
		enemy_m = nullptr;
	}
	pos1.clear();
	pos2.clear();
	radius.clear();
}
//---------------------------------------------
//		更新処理
//---------------------------------------------
void GameMain::Update()
{
#if(NDEBUG)
	// デバッグの更新
	DebugUpdata();
#endif
	// デバッグの更新
	DebugUpdata();

	if (IsKeyOn(KEY_INPUT_F) || IsPadOn(PAD_START)) {
		scene_type = PAUSE;
	}

	if (isUpdata ) {

		// クリアしたら
		if (clear) {
			GameClear();
		}
		// ゲームオーバー
		if (fail) {
			GameOver();
		}
		// ゲーム中
		if(clear == false && fail == false){
			// 前の経過時間を保存
			old_time = count_time;
			// 経過時間 = 今のミリ秒時間 - スタートした時間 / 秒に直す
			count_time = (GetNowCount() - start_time) / 1000;
			// 経過時間が違うとき
			if (old_time != count_time) {
				// タイマーを減らす
				play_timer--;
				// スコアを増やす
				score += SCORE;
			}

			// ゲームオーバーチェック
			CheckGameOver();
			// ゲームクリアチェック
			CheckGameClear();
		}
		
		// 更新
		if (player_m != nullptr) {
			player_m->Update();
		}
		if (enemy_m != nullptr) {
			enemy_m->Update(play_timer);
		}
		if (map_m != nullptr) {
			map_m->Update();
		}

		// 当たり判定
		CheckHit();

		// Effekseerにより再生中のエフェクトを更新する。
		UpdateEffekseer3D();

		// isOneUdateがtrueの時
		if (isOneUpdate) {
			// 更新を止める
			isOneUpdate = false;
			isUpdata = false;
		}
	}

	// エフェクトの処理
	for (int i = 0; i < effect.size(); i++) {
		effect[i]->Update();
		DeleteEffect(i);
	}
}
//---------------------------------------------
//		描画処理
//---------------------------------------------
void GameMain::Draw()
{
	// スカイドーム
	VECTOR a_ = { 0.0f,0.0f,0.0f };
	VECTOR b_ = { 0.3f,0.3f,0.3f };
	VECTOR c_ = { 1.0f,1.0f,1.0f };
	VECTOR d_ = { 3.0f,3.0f,3.0f };
	VECTOR e_ = { 5.0f,5.0f,5.0f };
	MV1SetScale(skydome, c_);
	MV1SetPosition(skydome, a_);
	MV1DrawModel(skydome);

	// マップの描画
	if (map_m != nullptr) {
		map_m->Draw();
	}
	// エネミーの描画
	if (enemy_m != nullptr) {
		enemy_m->Draw();
	}
	// プレイヤーの描画
	if (player_m != nullptr) {
		player_m->Draw();
	}
	// エフェクトの描画
	for (int i = 0; i < effect.size(); i++) {
		effect[i]->Draw();
	}
	// 城のライフゲージの描画
	if (map_m != nullptr) {
		DrawCastleHPGauge(map_m->GetCastle());
	}

	// 画像の描画
	DrawGraphHandle();

	// クリア
	if (clear) {
		if (result_timer < 150) {
			DrawCenterString(WINDOW_W / 2, 400, "防衛成功 !!", RED, font_handle[FONT_CLEAR]);
		}
		else {
			DrawCenterString(WINDOW_W / 2, 400, "Congratulation!", YELLOW, font_handle[FONT_CLEAR]);
		}
		DrawStringToHandle(0, 0, "残り時間 : FINISH ", BLACK, font_handle[FONT_TIME]);
	}
	// ゲームオーバー
	if (fail) {
		DrawCenterString(WINDOW_W / 2, 400, "防衛失敗 !!",BLUE, font_handle[FONT_CLEAR]);
	}
	if(clear == false && fail == false)
	{
		// 経過時間の表示
		DrawFormatStringToHandle(0, 0, BLACK, font_handle[FONT_TIME], "残り時間 : %d", play_timer);
		// 文字の描画
		DrawRightString(150, "制限時間まで城を守れ！", RED, font_handle[FONT_GUIDE]);
		// ポーズに文字
		DrawRightString(0,"START", BLACK,font_handle[FONT_PAUSE]);
	}

#if(NDEBUG)
	VECTOR a = {100.0f,0.0f,0.0f};
	VECTOR b = {-100.0f,0.0f,0.0f };
	DrawLine3D(a,b,RED);
	a = { 0.0f,100.0f,0.0f };
	b = { 0.0f,-100.0f,0.0f };
	DrawLine3D(a,b,GREEN);
	a = { 0.0f,0.0f,100.0f };
	b = { 0.0f,0.0f,-100.0f };
	DrawLine3D(a,b,BLUE);
	for (int i = 0; i < pos2.size();i++) {
		DrawLine3D(pos1[i].GetV(), pos2[i].GetV(), SKYBLUE);
	}
	for (int i = 0; i < radius.size(); i++) {
		DrawSphere3D(pos1[i + pos2.size()].GetV(),radius[i], 20, SKYBLUE, SKYBLUE,FALSE);
	}
#endif
}
//---------------------------------------------
// デバッグの更新
//---------------------------------------------
void GameMain::DebugUpdata()
{
	// F1が押されたら更新を切り替え
	if (IsKeyOn(KEY_INPUT_F1)) isUpdata = !isUpdata;
	// F2が押されていて更新が止まっていたら
	if (IsKeyOn(KEY_INPUT_F2) && !isUpdata) {
		// 更新開始
		isUpdata = true;
		isOneUpdate = true;
	}
}
//---------------------------------------------
// 当たり判定をまとめる
//---------------------------------------------
void GameMain::CheckHit()
{
	// どちらかが nullptr の時 return
	if (enemy_m == nullptr || player_m == nullptr)return;

	// プレイヤーとエネミーの当たり判定
	for (int i = 0; i < enemy_m->GetSize(); i++) 
	{
		CheckHitPlayer_EnemyRange(player_m->GetPlayer(), enemy_m->GetEnemyBase(i));
		CheckHitPlayer_EnemyAttackRange(player_m->GetPlayer(), enemy_m->GetEnemyBase(i));
	}

	// 剣とエネミー
	CheckHitSword_Enemy(player_m->GetPlayer());
	// 特殊攻撃とエネミー
	CheckHitSpacial_Enemy(player_m->GetPlayer());

	// nullptr の時 return
	if (map_m == nullptr) return;
	for (int i = 0; i < enemy_m->GetSize(); i++) {
		CheckHitEnemyAttack_Castle(enemy_m->GetEnemyBase(i),map_m->GetCastle());
	}
	
	// マップの当たり判定
	CheckHitMap(map_m->GetCastle(),player_m->GetPlayer());
	for (int i = 0; i < enemy_m->GetSize(); i++)
	{
		CheckHitMap(map_m->GetCastle(), enemy_m->GetEnemyBase(i));
	}
}
//---------------------------------------------
// プレイヤーとエネミーの範囲の当たり判定
//---------------------------------------------
void GameMain::CheckHitPlayer_EnemyRange(Player * p, EnemyBase * e)
{
	// エネミーの範囲に入っていない場合処理をしないまたは、
	// 無敵の時
	if (!HitCheck_Sphere_Sphere(p->GetVPos(), p->GetRadius(),
		e->GetVPos(), e->GetRadius() + e->GetRangeRadius())
		|| p->GetInvincible()) 
	{
		// エネミーの range を false にする
		e->SetFalseRange();
		return;
	}
	else { // 入っていたら
		// エネミーにヒット宣言
		e->SetIsHitRange();
		e->SetPlayerPos(p->GetVPos());
	}
}
//---------------------------------------------
// プレイヤーとエネミーの攻撃範囲の当たり判定
//---------------------------------------------
void GameMain::CheckHitPlayer_EnemyAttackRange(Player * p, EnemyBase * e)
{
	// エネミーの攻撃範囲に入っていない場合処理をしない
	if (!HitCheck_Sphere_Sphere(p->GetVPos(), p->GetRadius(),
		e->GetVPos(), e->GetRadius() + e->GetAttackRangeRadius()))
	{
		// エネミーの attackrange を false にする
		e->SetFalseAttackRange();
		return;
	}
	else { // 入っていたら
		// エネミーにヒット宣言
		e->SetIs_HitAttackRange();
		CheckHitPlayer_EnemyAttack(p, e);
	}
}
//---------------------------------------------
// プレイヤーとエネミーの攻撃の当たり判定
//---------------------------------------------
void GameMain::CheckHitPlayer_EnemyAttack(Player * p, EnemyBase * e)
{
	// 当たっていたら
	// エネミーが死んでいなかったら
	if (e->GetAttack() && !e->GetGoal()) {
		if (HitCheck_Sphere_Capsule(e->GetVPos(), e->GetRadius() + e->GetAttackRangeRadius(),
			p->GetCapsuleUp(p->GetVPos(), 8.0f), p->GetCapsuleDown(p->GetVPos()), p->GetRadius())
			&& !e->GetDeath() && !p->GetInvincible())
		{
			// HP をダメージ分減らす
			p->HpProcessing(e->GetDamage());
			// 無敵フラグを立てる
			p->SetInvincible();
		}
	}
}
//---------------------------------------------
// 剣とエネミーの当たり判定
//---------------------------------------------
void GameMain::CheckHitSword_Enemy(Player * p)
{
	// 攻撃しているか確認
	// していない場合処理を終了
	if (!p->GetAttack())return;

	for (int i = 0; i < enemy_m->GetSize(); i++)
	{
		EnemyBase * e = enemy_m->GetEnemyBase(i);
		// エネミーが死んでいたり無敵だった場合
		if (e->GetDeath() || e->GetInvincible()) {
			continue;
		}
		// できたら、剣とエネミーが当たっているか確認
		if (HitCheck_Sphere_Capsule(e->GetVPos(), e->GetRadius(),
			p->GetCapsuleTip(p->GetSwordTipPos()), p->GetCapsuleRoot(p->GetSwordRootPos()), p->GetRadius()))
		{
			// HPを減らす
			e->HpProcessing(1);
			if (e->GetHp() == 0) {
				// 当たっていたらエネミーをデリート
				e->SetIsHitDeath();
				// MPを回復
				p->AddMp(ADD_MP);
			}
			else {
				// エネミーの無敵化
				e->SetInvincible();
			}
			// エネミーの座標を取得
			enemy_pos = e->GetPos();
			enemy_pos.y += 3.0f;
			// スコアを加算
			score += e->GetScore();
			// 撃破カウントをプラスする
			defeat_count++;
			// エネミーの座標に斬撃のエフェクトを生成
			effect.push_back(new Effect("hit_eff.efk", enemy_pos, true));
		}
	}
	// 攻撃フラグをたおす
	p->SetFalseAttack();
}
//---------------------------------------------
// 特殊攻撃とエネミーの当たり判定
//---------------------------------------------
void GameMain::CheckHitSpacial_Enemy(Player * p)
{
	// 特殊攻撃しているか確認
	// していない場合処理を終了
	if (!p->GetSpacialAttack())return;

	for (int i = 0; i < enemy_m->GetSize(); i++)
	{
		EnemyBase *e = enemy_m->GetEnemyBase(i);

		// エネミーが死んでいたり無敵だった場合
		if (e->GetDeath() || e->GetInvincible()) {
			continue;
		}

		// エネミーと特殊攻撃の範囲
		// 特殊攻撃時間が特殊攻撃の最大時間に達していない時
		if(HitCheck_Sphere_Sphere(e->GetVPos(), e->GetRadius(), p->GetSpacialPos(),10.0f) 
			&& p->GetSpacialTimer() < p->GetSpacialMaxTimer())
		{
			// 当たっていたらエネミーをデリート
			e->SetIsHitDeath();
			// スコアを加算
			score += e->GetScore();
			// 撃破カウントをプラスする
			defeat_count++;
		}
	}
}
//---------------------------------------------
// エネミーの攻撃と城の当たり判定
//---------------------------------------------
void GameMain::CheckHitEnemyAttack_Castle(EnemyBase * e, Castle * c)
{
	if (e->GetAttack() && e->GetGoal()) {
		if (HitCheck_Line_Sphere(c->GetCastlePos1(), c->GetCastlePos2(), 
			e->GetVPos(), e->GetRadius() + e->GetAttackRangeRadius()) 
			&& !e->GetDeath())
		{
			// HP をダメージ分減らす
			c->HpProcessing(e->GetDamage());
			// エネミーの攻撃フラグをfalseにする
			e->SetFalseAttack();
			// 城のHPが０以下の時
			if (c->GetHp() <=  0) {
				c->SetHPInit();
			}
		}
	}
}
//---------------------------------------------
// マップの当たり判定
//---------------------------------------------
void GameMain::CheckHitMap(Castle * m, CollisionBase * c)
{
	// 線（壁）とカプセル（プレイヤー）の当たり判定
	for (int i = 0; i < pos2.size(); i++) {
		if (HitCheck_Line_Sphere(pos1[i].GetV(),pos2[i].GetV(),c->GetVPos(),c->GetRadius())) 
		{
			//------------------------------------
			// 点と線上の１番近い距離を求める処理
			//------------------------------------
			// ライン上のベクトル
			Vector3 line_v = pos1[i] - pos2[i];
			// ラインの座標pos1とプレイヤーの座標のベクトル
			Vector3 pos_v = c->GetPos() - pos1[i];

			Vector3 dot_f;		// 内積を求める用

			// ラインの座標pos1から点と線上の点の1番近い座標との距離
			float line_lenght = dot_f.dot(line_v.normalize(),pos_v);
			// 点と線上の１番近い点座標
			Vector3 min_point = pos1[i] + (line_v.normalize() * line_lenght);

			//------------------------------------
			// 押し戻し
			//------------------------------------
			// ２点間のベクトル = プレイヤーの座標 - 線上1番近い点座標
			Vector3 v = c->GetPos() - min_point;

			float lenght;	// 当たった長さを求める用

			// 当たった長さ = プレイヤーの半径 - 点と線の１番近い距離
			lenght = c->GetRadius() - Segment_Point_MinLength(pos1[i].GetV(), pos2[i].GetV(), c->GetVPos());
			// 押し戻した座標　=　プレイヤーの座標 + ２点間の単位ベクトル * 当たった長さ
			Vector3 result = c->GetPos() + v.normalize() * lenght;
			// プレイヤーの座標に代入
			c->SetPos(result);
		}
	}
	// 円（噴水、タワー）とカプセル（プレイヤー）の当たり判定
	for (int i = 0; i < radius.size(); i++) 
	{
		// 円とカプセルの当たり判定
		if (HitCheck_Sphere_Capsule(pos1[i + pos2.size()].GetV(), radius[i], 
			c->GetCapsuleUp(c->GetVPos(), 8.0f), c->GetCapsuleDown(c->GetVPos()), c->GetRadius())) 
		{
			// ２点間のベクトル = プレイヤーの座標 - 円の座標
			Vector3 v = c->GetPos() - pos1[i + pos2.size()];

			Vector3 sprt;	// ２点間の距離を求める用
			float lenght;	// 当たった長さを求める用

			// 当たった長さ = プレイヤーの半径 + 円の半径 - ２点間の距離
			lenght = c->GetRadius() + radius[i] - sprt.GetSqrtf(c->GetPos(), pos1[i + pos2.size()]);
			// 押し戻した座標 = プレイヤーの座標 + ２点間の単位ベクトル * 当たった長さ
			Vector3 result = c->GetPos() + v.normalize() * lenght;
			// プレイヤーの代入
			c->SetPos(result);
		}
	}
}
//---------------------------------------------
// ゲームオーバーチェック
//---------------------------------------------
void GameMain::CheckGameOver()
{
	if (map_m == nullptr) { return; }

	HPCheck(map_m->GetCastle());

	if (player_m == nullptr) { return; }

	HPCheck(player_m->GetPlayer());
}
//---------------------------------------------
// ゲームクリアチェック
//---------------------------------------------
void GameMain::CheckGameClear()
{
	if (play_timer == 0) {
		clear = true;
	}
}
//---------------------------------------------
// ゲームクリア
//---------------------------------------------
void GameMain::GameClear()
{
	result_timer++;
	for (int i = 0; i < enemy_m->GetSize(); i++)
	{
		EnemyBase *e = enemy_m->GetEnemyBase(i);
		// 当たっていたらエネミーをデリート
		e->SetIsHitDeath();
	}

	// リザルトタイマーがクリアの最大時間を超えたら
	if (result_timer > CLEAR_MAX_TIME) {
		scene_type = RESULT;
		result = true;
		// リザルトファイルに書き込み
		ResultFileWrite();
	}
}
//---------------------------------------------
// ゲームオーバー
//---------------------------------------------
void GameMain::GameOver()
{
	result_timer++;

	// リザルトタイマーがゲームオーバーの最大時間を超えたら
	if (result_timer > FAIL_MAX_TIME) {
		scene_type = RESULT;
		result = false;
		// リザルトファイルに書き込み
		ResultFileWrite();
	}
}
//---------------------------------------------
// 城の残りHPをチェック
//---------------------------------------------
void GameMain::HPCheck(CollisionBase * c)
{
	if (c->GetHp() <= 0) {
		fail = true;
	}
}
//---------------------------------------------
// pos1ファイルを読み込み
//---------------------------------------------
void GameMain::Pos1FileRead()
{
	file.open("data/txt/LoadObjectPos1.txt", ios::in);

	if (!file.is_open()) {
		// ファイルを開くことに失敗しました
		return;
	}

	string str;
	while (getline(file, str))
	{
		while (true) {
			string::size_type pos = str.find(',', 0);
			if (pos == string::npos) break;
			str.replace(pos, 1, " ");	// replace=置き換え/ replace(場所,文字数,なんて文字に)
		}

		stringstream sstr(str);
		float x,y,z;
		sstr >> x >> y >> z;
		pos1.push_back(VECTOR{x,y,z});
	}

	// ファイルを閉じる
	file.close();
}
//---------------------------------------------
// pos2ファイルを読み込み
//---------------------------------------------
void GameMain::Pos2FileRead()
{
	file.open("data/txt/LoadObjectPos2.txt", ios::in);

	if (!file.is_open()) {
		// ファイルを開くことに失敗しました
		return;
	}

	string str;
	while (getline(file, str))
	{
		while (true) {
			string::size_type pos = str.find(',', 0);
			if (pos == string::npos) break;
			str.replace(pos, 1, " ");	// replace=置き換え/ replace(場所,文字数,なんて文字に)
		}

		stringstream sstr(str);
		float x, y, z;
		sstr >> x >> y >> z;
		pos2.push_back(VECTOR{ x,y,z });
	}

	// ファイルを閉じる
	file.close();
}
//---------------------------------------------
// radiusファイルを読み込み
//---------------------------------------------
void GameMain::RadiusFileRead()
{
	file.open("data/txt/LoadObjectRadius.txt", ios::in);

	if (!file.is_open()) {
		// ファイルを開くことに失敗しました
		return;
	}

	string str;
	while (getline(file, str))
	{
		stringstream sstr(str);
		float r;
		sstr >>r;
		radius.push_back(r);
	}

	// ファイルを閉じる
	file.close();
}
//---------------------------------------------
// resultファイル書き込み
//---------------------------------------------
void GameMain::ResultFileWrite()
{
	file.open("data/txt/Result.txt", ios::out);

	if (!file.is_open()) {
		// ファイルを開くことに失敗しました
		return;
	}

	if (map_m == nullptr) { return; }
	Castle *c = map_m->GetCastle();
	float rate = (float)c->GetHp() / (float)c->GetMaxHp();
	float life = rate * 100.0f;

	// クリアか失敗か
	file << result << ' ';
	// スコア
	file << score << ' ';
	// 城の残りHP
	file << (int)life << ' ';
	// 撃破数
	file << defeat_count << endl;

	// ファイルを閉じる
	file.close();
}
//---------------------------------------------
// CastleのHPゲージの描画
//---------------------------------------------
void GameMain::DrawCastleHPGauge(Castle * m)
{
	// 横幅450　縦幅 60
	// ダメージ
	// 45
	// 90
	// 135
	int life_size_x = CASTLE_LIFEGAUGE_W;
	int life_size_y = CASTLE_LIFEGAUGE_H;
	int life_x = CASTLE_LIFEGAUGE_X - life_size_x;
	int life_y = CASTLE_LIFEGAUGE_Y - life_size_y;
	// 灰色ゲージ
	DrawBox(life_x, life_y, life_x + life_size_x * 2, life_y + life_size_y * 2, BLACKGRAY, TRUE);
	// old_HPゲージ
	DrawBox(life_x, life_y, life_x + (m->GetOldHp()* life_size_x * 2 / m->GetMaxHp()), life_y + life_size_y * 2, WHITE, WHITE);
	// HPゲージ
	DrawBox(life_x, life_y, 
		life_x + (m->GetHp() * life_size_x * 2 / m->GetMaxHp()) , life_y + life_size_y * 2, GREEN, TRUE);

	float rate = (float)m->GetHp() / (float)m->GetMaxHp();
	float life = rate * 100.0f;
	DrawFormatStringToHandle(CASTLE_LIFEGAUGE_X - 40, CASTLE_LIFEGAUGE_Y - 10, GetColor(255, 255, 255), font_handle[FONT_HP],"%.0f ％", life);
}
