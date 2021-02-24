//---------------------------------------------
// 定数の宣言
//---------------------------------------------
const float MAX_RANGE_RADIUS = 5.0f;	// 最大範囲
const float ATTACK_RANGE_RADIUS = 2.0f;	// 攻撃範囲
const float MAX_SPEED = 0.1f;			// 最大移動速度
const int DAMAGE = 13;					// ダメージ数
const int SCORE = 300;					// スコア
const int MAX_HP = 2;					// HP

//---------------------------------------------
// コンストラクタ
//---------------------------------------------
Enemy1::Enemy1(Vector3 pos_ ,Vector3 rot_ ,Vector3 scale_ )
{
	pos = pos_;
	rot = rot_;
	scale = scale_;
	radius = MAX_RANGE_RADIUS;
	range_radius = MAX_RANGE_RADIUS;
	speed = MAX_SPEED;
	damage = DAMAGE;
	score = SCORE;
	hp = MAX_HP;

	SetTextureHandle("haar_detail_NRM.jpg");
	SetTextureHandle("Spinnen_Bein_tex_COLOR_.jpg");
	SetTextureHandle("Spinnen_Bein_tex_2.jpg");
	SetTextureHandle("SH3.png");
	
	SetModelHandle("Spider_3.mv1");

	ApplyTexture();		// テクスチャー適用
	SetAnimNum(POSE);	// アニメーション番号指定
	SetAnim();			// アニメーション関係の設定
}
//---------------------------------------------
// デストラクタ
//---------------------------------------------
Enemy1::~Enemy1()
{
	MV1DeleteModel(model);
}
//---------------------------------------------
// 更新
//---------------------------------------------
void Enemy1::Update()
{
	SetOld();

	// 死亡時
	if (GetDeath()) {
		SetAnimNum(DIE);
		play_anim_speed = 0.3f;
		// アニメーション関係の設定
		SetAnim();
		// アニメーションを更新
		UpdateAnim();
		if (anim_num == DIE && play_anim == 0.0f) {
			death_anim = true;
		}
		return;
	}

	// 無敵フラグが立っていたら
	if (GetInvincible()) {
		// 無敵処理
		Invincible();
	}

	// ゴールに向かう
	if (!GetGoal()) {
		Approach(goal_pos);
		SetAnimNum(WAIK_VOR);
	}
	// ゴールチェック
	CheckGoal();

	// アイドル
	if (anim_num == POSE) {
		recast_time++;
		recast_time %= 90;
	}

	// ゴールしたら
	if (GetGoal()) {
		if (recast_time == 0) {
			SetAnimNum(ATTACK);
		}
	}

	// 攻撃モーション時
	if (anim_num == ATTACK) {
		// アニメーションが13までいったら
		if ((int)play_anim == 13) {
			attack = true;
		}
		// アニメーションが終了したら
		if (play_anim == 0.0f) {
			// アイドル
			SetAnimNum(POSE);
			pos.y = 0.0f;
		}
	}

	// アニメーション関係の設定
	SetAnim();
	// アニメーションを更新
	UpdateAnim();

}
//---------------------------------------------
// 描画
//---------------------------------------------
void Enemy1::Draw()
{
	// modelのポジションを設定
	MV1SetPosition(model, pos.GetV());
	// modelの向きを設定
	MV1SetRotationXYZ(model, rot.GetV());
	// modelのサイズを設定
	MV1SetScale(model , scale.GetV());
	// アニメーション
	MV1SetAttachAnimTime(model,attach_anim,play_anim);

	if (!GetInvincible()) {
		// npc.xモデルの描画
		MV1DrawModel(model);
	}
	if (GetInvincible() && GetInvincibleTimer() % 10 != 0) {
		// npc.xモデルの描画
		MV1DrawModel(model);
	}
	

#if(NDEBUG)
	//DrawSphere3D(pos.GetV(), radius +  attack_range_radius, 20, SKYBLUE, SKYBLUE, false);

	DrawSphere3D(pos.GetV(), radius, 20, BLUE, BLUE, false);
#endif
}
