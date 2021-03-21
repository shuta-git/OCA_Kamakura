#include "Player.h"
//---------------------------------------------
// 定数の宣言
//---------------------------------------------
const int PLAYER_GAUGE_X = WINDOW_W / 2;	// プレイヤーのゲージの座標X（ゲージの中心）
const int PLAYER_GAUGE_Y = WINDOW_H - 100;	// プレイヤーのゲージの座標Y（ゲージの中心）
const int PLAYER_GAUGE_W = 200;				// プレイヤーのゲージの横幅
const int PLAYER_GAUGE_H = 13;				// プレイヤーのゲージの縦幅
const int PLAYER_MPGAUGE_H = 7;				// プレイヤーのMPゲージの縦幅
const int PLAYER_STAMINA_H = 7;				// プレイヤーのスタミナの縦幅
// プライヤーのスタミナゲージの座標Y
const int PLAYER_STAMINA_Y = PLAYER_GAUGE_Y + PLAYER_GAUGE_H + PLAYER_STAMINA_H;
// プレイヤーのMPゲージの座標Y
const int PLAYER_MPGAUGE_Y = PLAYER_STAMINA_Y + PLAYER_STAMINA_H + PLAYER_MPGAUGE_H;
const int BACK_GAUGE = 2;					//　ふち
const float DEC_RATE = 0.71f;				// 減少したときの係数
const float P_ROT_RANGE = (DX_PI_F / 18);	// PLAYERの回転範囲	
const float P_SPEED = 0.4f;					// PLAYERの速度
const float P_DASH__SPEED = 0.6f;			// PLAYERのダッシュスピード
const float P_ROT_SPEED = 18.0f;			// PLAYERの回転速度
const float P_ROT_PI_F = 90.0f;				// float 型の角度
const int	MAX_HP = 100;					// プレイヤーの最大HP
const int	MAX_MP = 100;					// MPの最大		
const int   MAX_STAMINA = 100;				// スタミナの最大
const int   STAMIN_RED_GAUGE = 40;			// スタミナが赤くなるタイミング
const int   SUB_MP = 40;					// MPの消費量
const int   SP_MAX_TIME = 130;				// 特殊攻撃最大時間

//---------------------------------------------
// コンストラクタ
//---------------------------------------------
Player::Player()
{
	MatrixInit();
	
	// データ読み込み
	model = resource::LoadModel("data/player/player2.mv1");
	SetTextureHandle("Texture/kami01_tekusutya.jpg");
	SetTextureHandle("Texture/kao_tekusutya1.jpg");
	SetTextureHandle("Texture/nunoyoroi.jpg");
	ApplyTexture();

	// 変数の初期化
	Init();

	// 各パラメータを初期化
	Range = 2000.0f;
	Atten0 = 0.0f;
	Atten1 = 0.0006f;
	Atten2 = 0.0f;
}
//---------------------------------------------
// デストラクタ
//---------------------------------------------
Player::~Player()
{
}
//---------------------------------------------
// 初期化
//---------------------------------------------
void Player::Init()
{
	scale = { 0.5f,0.5f,0.5f };

	old_dir = 0;
	dir = 0;
	attack_time = 0;
	hp = MAX_HP;
	mp = MAX_MP;
	stamina = MAX_STAMINA;
	old_mp = MAX_MP;
	old_hp = MAX_HP;

	sp_timer = 0;

	speed = P_SPEED;
	rot_speed = (DX_PI_F / 360) * P_ROT_SPEED;
	radius = 1.0f;

	attack = false;
	now_attack = false;
	stamina_out = false;

	sp_pos = pos;
	camera_rot = Vector3(0.0f, 0.0f, 0.0f);
}
//---------------------------------------------
// 更新
//---------------------------------------------
void Player::Update()
{
	camera_mat = MGetIdent();
	// 行列の初期化
	MatrixInit();
	// 回転行列に値を設定
	SetMatRot();

	old_pos = pos;
	old_rot_y = rot.y;

	// 回転
	MoveRot();
	// 移動
	Move();
	
	// プレイヤーとカメラの回転値の差が円周率を超えた
	// プレイヤーにカメラの値を代入
	if (rot.y - camera_rot.y > (DX_PI_F * 2) || rot.y - camera_rot.y < -(DX_PI_F * 2)) {
		rot.y = camera_rot.y;
	}
	// 無敵フラグが立っていたら
	if (GetInvincible()) {
		// 無敵処理
		Invincible();
	}

	// 攻撃アクション
	AttackAction();

	// エフェクトの処理
	for (int i = 0; i < effect.size();i++) {
		// 特殊攻撃のエフェクトの時
		if (effect[i]->CheckPath("Tornade.efk")) {
			Vector3 scale_ = {2.0f,2.0f,2.0f};
			effect[i]->SetScale(scale_);
		}
		effect[i]->Update();
		DeleteEffect(i);
	}

	// ゲージの更新処理
	GeugeUpdate();

	// ポイントライトの調整
	//PointLightUpdate();
	// モデルの上空にポイントライトを設定
	ChangeLightTypePoint(
		VGet(pos.x, 1000.0f, pos.z - 30.0f),
		Range,
		Atten0,
		Atten1,
		Atten2);
}
//---------------------------------------------
// 描画
//---------------------------------------------
void Player::Draw()
{
	// modelのポジションを設定
	MV1SetPosition(model, pos.GetV());
	// modelの向きを設定
	MV1SetRotationXYZ(model, rot.GetV());
	// modelのサイズを設定
	MV1SetScale(model,scale.GetV());

	if (!GetInvincible()) {
		// player.xモデルの描画
		MV1DrawModel(model);
	}
	if (GetInvincible() && GetInvincibleTimer() % 10 != 0) {
		// player.xモデルの描画
		MV1DrawModel(model);
	}
	// エフェクトの描画
	for (int i = 0; i < effect.size(); i++) {
		effect[i]->Draw();
	}
	
	// ゲージの描画
	DrawGeuge();

	// パラメータの内容を画面に表示
	//SetFontSize(20);
	//DrawFormatString(0, 0, GetColor(255, 255, 255), "Key:D.C  Range  %f", Range);
	//DrawFormatString(0, 16, GetColor(255, 255, 255), "Key:F.V  Atten0 %f", Atten0);
	//DrawFormatString(0, 32, GetColor(255, 255, 255), "Key:G.B  Atten1 %f", Atten1);
	//DrawFormatString(0, 48, GetColor(255, 255, 255), "Key:H.N  Atten2 %f / 100.0f", Atten2 * 100.0f);

#if(NDEBUG)
	// player 本体の当たり判定
	DrawCapsule3D(GetCapsuleUp(pos.GetV(),8.0f), GetCapsuleDown(pos.GetV()), GetRadius(), 20, RED, RED, FALSE);
	// 攻撃のあたり
	Vector3 sword_tip = pos;
	sword_tip.x += sinf(rot.y);
	sword_tip.z += cosf(rot.y);
	Vector3 sword_root = pos;
	sword_root.x += sin(rot.y) * 5.0f;
	sword_root.z += cos(rot.y) * 5.0f;
	DrawCapsule3D(GetCapsuleTip(sword_root.GetV()), GetCapsuleRoot(sword_tip.GetV()), GetRadius(), 20, RED, RED, FALSE);
	// 特殊攻撃範囲
	for (int i = 0; i < (int)effect.size(); i++) {
		if (effect[i]->IsEffectPlaying("Tornade.efk") == 0 && sp_timer < SP_MAX_TIME) {
			DrawSphere3D(sp_pos.GetV(), 8.0f, 20, SKYBLUE, SKYBLUE, FALSE);
		}
	}
#endif
}
//---------------------------------------------
// 剣の先端の座標を取得
//---------------------------------------------
VECTOR Player::GetSwordTipPos() const
{
	Vector3 sword_tip = pos;
	sword_tip.x += sinf(rot.y);
	sword_tip.z += cosf(rot.y);
	return sword_tip.GetV();
}
//---------------------------------------------
// 剣の持ち手の座標を取得
//---------------------------------------------
VECTOR Player::GetSwordRootPos() const
{
	Vector3 sword_root = pos;
	sword_root.x += sin(rot.y) * 5.0f;
	sword_root.z += cos(rot.y) * 5.0f;
	return sword_root.GetV();
}
//---------------------------------------------
// 攻撃アクション
//---------------------------------------------
void Player::AttackAction()
{
	// 左クリック かつ 今攻撃していない時
	if (now_attack == false) {
		if (IsMouseOn(MOUSE_INPUT_LEFT) || IsPadOn(PAD_Y)) {
			attack = true;
			now_attack = true;
			anim_num = ATTACK;
		}
	}

	for (int i = 0; i < (int)effect.size(); i++) {
		// 再生されている時
		if (effect[i]->IsEffectPlaying("Tornade.efk") == 0) {
			sp_timer++;
		}
		// 再生が終わっている
		if (sp_timer > SP_MAX_TIME){
			spacial_attack = false;
			sp_timer = 0;
		}
	}

	if (now_attack == false && mp >= SUB_MP && spacial_attack == false) {
		// 右クリック かつ 今攻撃していない時 かつ mpが20以上の時
		if (IsMouseOn(MOUSE_INPUT_RIGHT) || IsPadOn(PAD_B)) {
			sp_pos = pos;
			sp_pos.x += sinf(rot.y) * 10;
			sp_pos.z += cosf(rot.y) * 10;
			effect.push_back(new Effect("Tornade.efk", sp_pos, true));
			spacial_attack = true;
			now_attack = true;
			anim_num = MAGIC;
			SubMp(SUB_MP);
		}
	}

	// 攻撃時間の更新処理
	AttackAnimTimeUpdate(ATTACK, 10);
	AttackAnimTimeUpdate(MAGIC, 30);
}
//---------------------------------------------
// 特殊攻撃の最大時間の取得
//---------------------------------------------
int Player::GetSpacialMaxTimer() const
{
	return SP_MAX_TIME;
}
//---------------------------------------------
// カメラ回転用を設定
//---------------------------------------------
void Player::SetCameraRot(Vector3 rot_)
{
	// カメラの値を代入
	camera_rot = rot_;
}
//---------------------------------------------
// MPをプラス
//---------------------------------------------
void Player::AddMp(int num)
{
	// 一つ前のmpを保存
	old_mp = mp;
	// mpを指定された分タス
	mp+=num;
	// mpが最大を超えたら
	if (mp > MAX_MP) {
		// mpを最大にする
		mp = MAX_MP;
	}
}
//---------------------------------------------
// MPを減らす
//---------------------------------------------
void Player::SubMp(int num)
{
	// 一つ前のmpを保存
	old_mp = mp;
	// mpを指定された分減らす
	mp -= num;
	// mpが0を下回ったら
	if (mp < 0) {
		//mpを0にする
		mp = 0;
	}
}
//---------------------------------------------
//  回転行列に値を設定
//---------------------------------------------
void Player::SetMatRot()
{
	// 全体行列  * 回転行列
	camera_mat = MMult(camera_mat, MGetRotY(camera_rot.y));
}
//---------------------------------------------
// 移動
//---------------------------------------------
void Player::Move()
{
	// 係数用
	float rate = 1.0f;

	if (CheckHitKey(KEY_INPUT_D)  || IsPadRepeat(PAD_RIGHT) || CheckHitKey(KEY_INPUT_A) || IsPadRepeat(PAD_LEFT)) {
		if (CheckHitKey(KEY_INPUT_W) || IsPadRepeat(PAD_UP) || CheckHitKey(KEY_INPUT_S) || IsPadRepeat(PAD_DOWN)) {
			rate = DEC_RATE;
		}
	}

	// ダッシュ
	speed = P_SPEED;
	if (stamina_out == false) {
		if (CheckHitKey(KEY_INPUT_LSHIFT) || IsPadRepeat(PAD_R)){
			speed = P_DASH__SPEED;
			stamina -= 2;
		}
	}
	// スタミナが0以下にならないようにする
	if (stamina <= 0) {
		stamina = 0;
		stamina_out = true;
	}

	// 移動量
	float move = speed * rate;

	if (CheckHitKey(KEY_INPUT_D) || IsPadRepeat(PAD_RIGHT)) {
		pos.x += camera_mat.m[0][0] * move;
		pos.z += camera_mat.m[0][2] * move;
	}
	if (CheckHitKey(KEY_INPUT_A) || IsPadRepeat(PAD_LEFT)) {
		pos.x -= camera_mat.m[0][0] * move;
		pos.z -= camera_mat.m[0][2] * move;
	}
	if (CheckHitKey(KEY_INPUT_S) || IsPadRepeat(PAD_DOWN)) {
		pos.x -= camera_mat.m[2][0] * move;
		pos.z -= camera_mat.m[2][2] * move;
	}
	if (CheckHitKey(KEY_INPUT_W) || IsPadRepeat(PAD_UP)) {
		pos.x += camera_mat.m[2][0] * move;
		pos.z += camera_mat.m[2][2] * move;
	}
}
//---------------------------------------------
// 回転
//---------------------------------------------
void Player::MoveRot()
{
	// 一つ前の向きを保存
	old_dir = dir;

	// 右上
	if (CheckHitKey(KEY_INPUT_D) && CheckHitKey(KEY_INPUT_W) || IsPadRepeat(PAD_RIGHT) && IsPadRepeat(PAD_UP)) {
		dir = RIGHT_UP;
		CameraDirRotRange(DIR_RIGHT_UP);
	}
	// 右下
	else if (CheckHitKey(KEY_INPUT_D) && CheckHitKey(KEY_INPUT_S) || IsPadRepeat(PAD_RIGHT) && IsPadRepeat(PAD_DOWN)) {
		dir = RIGHT_DOWN;
		CameraDirRotRange(DIR_RIGHT_DOWN);
	}
	// 左上
	else if (CheckHitKey(KEY_INPUT_A) && CheckHitKey(KEY_INPUT_W) || IsPadRepeat(PAD_LEFT) && IsPadRepeat(PAD_UP)) {
		dir = LEFT_UP;
		CameraDirRotRange(DIR_LEFT_UP);
	}
	// 左下
	else if (CheckHitKey(KEY_INPUT_A) && CheckHitKey(KEY_INPUT_S) || IsPadRepeat(PAD_LEFT) && IsPadRepeat(PAD_DOWN)) {
		dir = LEFT_DOWN;
		CameraDirRotRange(DIR_LEFT_DOWN);
	}
	// 右
	else if (CheckHitKey(KEY_INPUT_D) || IsPadRepeat(PAD_RIGHT)) {
		RotSpeed(RIGHT, LEFT, DIR_RIGHT);
	}
	// 左
	else if (CheckHitKey(KEY_INPUT_A) || IsPadRepeat(PAD_LEFT)) {
		RotSpeed(LEFT, RIGHT, DIR_LEFT);
	}
	// 上
	else if (CheckHitKey(KEY_INPUT_W) || IsPadRepeat(PAD_UP)) {
		RotSpeed(UP, DOWN, DIR_UP1);
	}
	// 下
	else if (CheckHitKey(KEY_INPUT_S) || IsPadRepeat(PAD_DOWN)) {
		RotSpeed(DOWN, UP, DIR_DOWN);
	}

	// 初期化
	rot_speed = (DX_PI_F / 360) * P_ROT_SPEED;
}
//---------------------------------------------
// 回転スピード
//---------------------------------------------
void Player::RotSpeed(int dir_, int old_, float DIR)
{
	dir = dir_;
	if (old_dir == old_) {
		rot_speed = (DX_PI_F / 360) * P_ROT_PI_F;
	}
	CameraDirRotRange(DIR);
}
//---------------------------------------------
// カメラの回転範囲の処理
// 引数にcamera.yとrot.yの差分を入れる
//---------------------------------------------
void Player::CameraDirRotRange(float p_rot_range)
{
	// ここが悪い

	float range = 0.0f;

	if (p_rot_range > DX_PI_F || rot.y < -DX_PI_F || camera_rot.y >(DX_PI_F / 2)) {
		range = -((DX_PI_F * 2) - p_rot_range);

		if (camera_rot.y - P_ROT_RANGE + range < rot.y &&
			camera_rot.y + P_ROT_RANGE + range > rot.y) {
			return;
		}

	}

	if (camera_rot.y - P_ROT_RANGE + p_rot_range < rot.y &&
		camera_rot.y + P_ROT_RANGE + p_rot_range > rot.y ) 
	{
		return;
	}

	DirSetRotRange();
}
//---------------------------------------------
// カメラの向き範囲を設定 
//---------------------------------------------
void Player::DirSetRotRange()
{
	switch (dir)
	{
	case RIGHT:
		SetRotRange(DIR_RIGHT, DIR_LEFT, DIR_LEFT, DIR_RIGHT);
		break;
	case LEFT:
		SetRotRange(DIR_UP1, DIR_RIGHT, DIR_LEFT, DIR_UP2, DIR_RIGHT, DIR_UP1, DIR_UP2, DIR_LEFT);
		break;
	case UP:
		SetRotRange(DIR_UP1, DIR_DOWN, DIR_UP2, DIR_DOWN);
		break;
	case DOWN:
		SetRotRange(DIR_DOWN, DIR_UP2, DIR_DOWN, DIR_UP1);
		break;
	case RIGHT_UP:
		SetRotRange(DIR_RIGHT_UP, DIR_LEFT_DOWN, DIR_LEFT_UP, DIR_RIGHT_DOWN);
		break;
	case RIGHT_DOWN:
		SetRotRange(DIR_RIGHT_DOWN, DIR_LEFT_UP, DIR_LEFT_DOWN, DIR_RIGHT_UP);
		break;
	case LEFT_UP:
		SetRotRange(DIR_UP1, DIR_RIGHT_DOWN, DIR_LEFT_UP, DIR_UP2, DIR_RIGHT_UP, DIR_UP1, DIR_UP2, DIR_LEFT_DOWN);
		break;
	case LEFT_DOWN:
		SetRotRange(DIR_UP1, DIR_RIGHT_UP, DIR_LEFT_DOWN, DIR_UP2, DIR_RIGHT_DOWN, DIR_UP1, DIR_UP2, DIR_LEFT_UP);
		break;
	default:
		break;
	}
}
//---------------------------------------------
// 向きの範囲を設定
// マイナスに回るかプラスに回るか判定
//---------------------------------------------
void Player::SetRotRange(float d1, float d2, float d3, float d4)
{
	if (rot.y > d1 + camera_rot.y &&
		rot.y <  d2 + camera_rot.y ||
		rot.y > -d3 + camera_rot.y &&
		rot.y < -d4 + camera_rot.y)
	{
		rot.y -= rot_speed;
	}
	else {
		rot.y += rot_speed;
	}
}
//---------------------------------------------
// 向きの範囲を設定
// マイナスに回るかプラスに回るか判定
//---------------------------------------------
void Player::SetRotRange(float d1, float d2, float d3, float d4, float d5, float d6, float d7, float d8)
{
	if (rot.y >= d1 + camera_rot.y &&
		rot.y <  d2 + camera_rot.y ||
		rot.y >  d3 + camera_rot.y &&
		rot.y <  d4 + camera_rot.y ||

		rot.y >  -d5 + camera_rot.y &&
		rot.y <= d6 + camera_rot.y ||
		rot.y > -d7 + camera_rot.y &&
		rot.y < -d8 + camera_rot.y)
	{
		rot.y -= rot_speed;
	}
	else {
		rot.y += rot_speed;
	}
}
//---------------------------------------------
// テクスチャーのパスを取得
//---------------------------------------------
void Player::SetTextureHandle(string str)
{
	string data = "data/player/";
	string path = data + str;
	texture_handle.push_back(resource::LoadGraph(path.c_str()));
}
//---------------------------------------------
// テクスチャーを適用
//---------------------------------------------
void Player::ApplyTexture()
{
	for (int i = 0; i < texture_handle.size(); i++) {
		int num = MV1GetMaterialDifMapTexture(model, i);
		MV1SetTextureGraphHandle(model, num, texture_handle[i], FALSE);
	}

	texture_handle.clear();
}
//---------------------------------------------
// ポイントライト調整
//---------------------------------------------
void Player::PointLightUpdate()
{
	// ＤＣキーでライトの影響範囲を変更
	if (CheckHitKey(KEY_INPUT_D) == 1)
	{
		Range += 20.0f;
	}
	if (CheckHitKey(KEY_INPUT_C) == 1)
	{
		Range -= 20.0f;
	}

	// ＦＶキーでライトの距離減衰パラメータ０の値を変更
	if (CheckHitKey(KEY_INPUT_F) == 1)
	{
		Atten0 += 0.001f;
	}
	if (CheckHitKey(KEY_INPUT_V) == 1)
	{
		Atten0 -= 0.001f;
	}

	// ＧＢキーでライトの距離減衰パラメータ１の値を変更
	if (CheckHitKey(KEY_INPUT_G) == 1)
	{
		Atten1 += 0.00001f;
	}
	if (CheckHitKey(KEY_INPUT_B) == 1)
	{
		Atten1 -= 0.00001f;
	}

	// ＨＮキーでライトの距離減衰パラメータ２の値を変更
	if (CheckHitKey(KEY_INPUT_H) == 1)
	{
		Atten2 += 0.0000001f;
	}
	if (CheckHitKey(KEY_INPUT_N) == 1)
	{
		Atten2 -= 0.0000001f;
	}

	// 影響距離の値を補正
	if (Range < 0.0f) Range = 0.0f;

	// 距離減衰パラメータの値を補正
	if (Atten0 < 0.0f) Atten0 = 0.0f;
	if (Atten1 < 0.0f) Atten1 = 0.0f;
	if (Atten2 < 0.0f) Atten2 = 0.0f;
}
//---------------------------------------------
// ゲージの値を設定して描画
// 横幅　縦幅　座標X　座標Y　
// なんのゲージか　そのゲージの一つ前の値 色
//---------------------------------------------
void Player::SetDrawGeuge(int size_x, int size_y, int x, int y,int type, int old_type, unsigned int color)
{
	// 外枠
	DrawBox(x - BACK_GAUGE, y - BACK_GAUGE, x + size_x * 2 + BACK_GAUGE, y + size_y * 2 + BACK_GAUGE, GetColor(50, 50, 50), TRUE);
	// 灰色ゲージ
	DrawBox(x , y , x + size_x * 2 , y + size_y * 2 , GetColor(90, 90, 90), TRUE);
	// 一つ前のゲージ
	DrawBox(x, y, x + (old_type *  size_x * 2 / MAX_MP), y + size_y * 2, GRAY, TRUE);
	// ゲージ
	DrawBox(x, y,x + (type *  size_x * 2 / MAX_MP), y + size_y * 2,color, TRUE);
}
//---------------------------------------------
// 攻撃モーション時間の更新
// 攻撃の種類　終了時間
//---------------------------------------------
void Player::AttackAnimTimeUpdate(int attack_type, int finish_time)
{
	// アニメーションが攻撃の時
	if (anim_num == attack_type) {
		// 今攻撃していたら
		if (now_attack) {
			// 時間を更新
			attack_time++;
		}
		// 攻撃時間が終了したら
		if (attack_time == finish_time) {
			// 攻撃を終了
			now_attack = false;
			// 攻撃時間を初期化
			attack_time = 0;
		}
	}
}
//---------------------------------------------
// ゲージの描画
//---------------------------------------------
void Player::DrawGeuge()
{
	// HPゲージ
	int life_size_x = PLAYER_GAUGE_W;
	int life_size_y = PLAYER_GAUGE_H;
	int life_x = PLAYER_GAUGE_X - life_size_x;
	int life_y = PLAYER_GAUGE_Y - life_size_y;
	// スタミナゲージ
	int st_size_x = PLAYER_GAUGE_W;
	int st_size_y = PLAYER_STAMINA_H;
	int st_x = PLAYER_GAUGE_X - st_size_x;
	int st_y = PLAYER_STAMINA_Y - st_size_y;
	unsigned int st_color = 0;
	if (stamina > STAMIN_RED_GAUGE) {
		st_color = GetColor(253, 126, 0);
	}
	else {
		st_color = GetColor(253, 0, 0);
	}
	// MPゲージ
	int mp_size_x = PLAYER_GAUGE_W;
	int mp_size_y = PLAYER_MPGAUGE_H;
	int mp_x = PLAYER_GAUGE_X - mp_size_x;
	int mp_y = PLAYER_MPGAUGE_Y - mp_size_y;
	// HPゲージ
	SetDrawGeuge(life_size_x, life_size_y, life_x, life_y, hp, old_hp, WHITE);
	// MPゲージ
	SetDrawGeuge(mp_size_x, mp_size_y, mp_x, mp_y, mp, old_mp, SKYBLUE);
	// スタミナゲージ
	SetDrawGeuge(st_size_x, st_size_y, st_x, st_y, stamina, 0, st_color);

	// MPゲージの格子
	int lattice_size_x = PLAYER_GAUGE_W / 5;
	int lattice_size_y = PLAYER_MPGAUGE_H;
	int lattice_x[5];
	int lattice_y = mp_y;
	for (int i = 0; i < 5; i++) {
		lattice_x[i] = mp_x + (lattice_size_x * i * 2);
	}
	// MPゲージの格子
	for (int i = 0; i < 5; i++) {
		DrawBox(lattice_x[i] - 1, lattice_y - 1, lattice_x[i] + lattice_size_x * 2 + 1, lattice_y + lattice_size_y * 2 + 1, GetColor(50, 50, 50), FALSE);
	}
}
//---------------------------------------------
// ゲージの更新処理
//---------------------------------------------
void Player::GeugeUpdate()
{
	// old_hpの処理
	if (old_hp != hp) {
		old_hp--;
	}
	// old_mpの処理
	if (old_mp > mp) {
		old_mp--;
	}
	if (old_mp < mp) {
		old_mp++;
	}

	// スタミナ切れの時
	if (stamina_out) {
		// スタミナが満タンでなければ
		if (stamina != MAX_STAMINA) {
			// スタミナを回復
			stamina++;
		}
		if (stamina > STAMIN_RED_GAUGE) {
			stamina_out = false;
		}
	}
	else {
		// ダッシュ中ではないとき
		if (!CheckHitKey(KEY_INPUT_LSHIFT) && !IsPadRepeat(PAD_R)) {
			// スタミナが満タンでなければ
			if (stamina != MAX_STAMINA) {
				// スタミナを回復
				stamina++;
			}
		}
	}
}
