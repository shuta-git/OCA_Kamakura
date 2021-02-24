//---------------------------------------------
// 定数の宣言
//---------------------------------------------
const float MAX_RANGE_RADIUS = 20.0f;	// 最大範囲
const float DEF_ATTACK_RANGE = 8.0f;	// デフォルト攻撃範囲
const float DIVE_ATTACK_RANGE = 12.0f;	// ダイブ攻撃範囲
const float FRONT_ATTACK_RANGE = 1.5f;	// 殴り攻撃範囲
const float MAX_SPEED = 0.1f;			// 最大移動速度
const float DEF_RADIUS = 1.5f;			// デフォルト半径
const int DAMAGE = 23;					// ダメージ数
const int SCORE = 500;					// スコア
const int MAX_HP = 2;					// HP

//---------------------------------------------
// コンストラクタ
//---------------------------------------------
Enemy2::Enemy2( Vector3 pos_, Vector3 rot_, Vector3 scale_)
{
	pos = pos_;
	pos.y = 0.0f;
	rot = rot_;
	scale = {0.01f,0.01f,0.01f};
	radius = DEF_RADIUS;

	range_radius = MAX_RANGE_RADIUS;
	attack_range_radius = DEF_ATTACK_RANGE;
	speed = MAX_SPEED;
	damage = DAMAGE;
	score = SCORE;
	hp = MAX_HP;

	SetModelHandle("idle.mv1");
	SetModelHandle("walk.mv1");
	SetModelHandle("attack_front.mv1");
	SetModelHandle("attack_divebomb.mv1");
	SetModelHandle("attack_fireball.mv1");
	SetModelHandle("intro.mv1");
	SetModelHandle("death.mv1");

	SetTextureHandle("gargoyle_low_gargoyle_body_mat_BaseColor.png");

	type = INTRO;

	ApplyTexture();
	SetAnimNum(0);		// アニメーション番号指定
	SetAnim();			// アニメーション関係の設定
}
//---------------------------------------------
// デストラクタ
//---------------------------------------------
Enemy2::~Enemy2()
{
}
//---------------------------------------------
// 更新
//---------------------------------------------
void Enemy2::Update()
{
	old_pos = pos;
	old_type = type;

	// 死亡時
	if (GetDeath()) {
		// 死亡モーション
		type = DEATH;
		// アニメーション速度の設定
		play_anim_speed = 1.0f;
		SetAnim();
		// アニメーションを更新
		UpdateAnim();
		// エフェクトの消去
		for (int i = 0; i < effect.size(); i++) {
			// エフェクトを止める
			StopEffect(i);
			// エフェクトの消去
			DeleteEffect(i);
		}
		// モーションが終了したら
		if (type == DEATH && play_anim == 0.0f) {
			// マネージャーに伝えるためにフラグを立てる
			death_anim = true;
		}
		return;
	}

	// 無敵フラグが立っていたら
	if (GetInvincible()) {
		// 無敵処理
		Invincible();
	}


	// イントロではない時かつ攻撃範囲に入っていたら
	if (type != INTRO && attack_range && !GetGoal()) {
		// ダイブ攻撃モーション
		type = ATTACK_DIVEBOMB;
		// アニメーション関係の設定
		SetAnim();
		if (type != old_type) {
			effect.push_back(new Effect("Flame.efk", pos,true));
			effect[effect.size() - 1]->SetScale(Vector3(1.2f,1.2f,1.2f));
			effect[effect.size() - 1]->SetSpeed(1.2f);
		}
		
	}

	// イントロと攻撃モーションではないとき
	// 移動処理
	if (type != INTRO && type != ATTACK_DIVEBOMB && !GetGoal()) {
		if (range) {
			// playerに向かう
			Approach(GetPlayerPos());
		}
		if (!range) {
			// 城に向かう
			Approach(goal_pos);
		}
		// ゴールチェック
		CheckGoal();
	}

	// アイドル
	if (type == IDLE) {
		recast_time++;
		recast_time %= 90;
	}

	// 城に着いたら
	if (GetGoal()) {
		if (recast_time == 0) {
			type = ATTACK_FRONT;
			// アニメーション関係の設定
			SetAnim();
		}
	}

	// 歩く
	if (type == WALK) {
		play_anim_speed = 3.0f;
	}
	else {
		play_anim_speed = 1.0f;
	}

	// アニメーション関係の設定
	SetAnim();
	// アニメーションを更新
	UpdateAnim();

	//	ダイブ攻撃
	if (type == ATTACK_DIVEBOMB ) {
		// 攻撃範囲の設定
		attack_range_radius = DIVE_ATTACK_RANGE;
		if (play_anim > 35.0f && play_anim < 100.0f) {
			invincible = true;
		}
		// アニメーションが100.0fを超えたら
		// 当たり判定起動
		if (play_anim > 100.0f) {
			invincible = false;
			attack = true;
			
		}
		// アニメーションが150.0fを超えたら
		// 当たり判定終了
		if (play_anim > 150.0f) {
			// 当たり判定終了
			attack = false;
			// 攻撃範囲を初期化
			attack_range_radius = DEF_ATTACK_RANGE;
		}
		// アニメーションが終了したら
		if (play_anim == 0.0f) {
			// 歩くアニメーションに変更
			type = WALK;
			// アニメーション関係の設定
			SetAnim();
		}
	}

	// 殴り攻撃
	if (type == ATTACK_FRONT) {
		attack_range_radius = FRONT_ATTACK_RANGE;
		if ((int)play_anim == 70) {
			attack = true;
		}
		// アニメーションが終了したら
		if (play_anim == 0.0f) {
			// アイドル
			type = IDLE;
			// 攻撃範囲を初期化
			attack_range_radius = DEF_ATTACK_RANGE;
			// アニメーション関係の設定
			SetAnim();
		}
	}

	// イントロ終了
	if (type == INTRO && play_anim == 0.0f) {
		type = WALK;
		// アニメーション関係の設定
		SetAnim();
	}

	// エフェクト更新
	for (int i = 0; i < effect.size(); i++) {
		effect[i]->Update();
		DeleteEffect(i);
	}
	
	
}
//---------------------------------------------
// 描画
//---------------------------------------------
void Enemy2::Draw()
{
	// モデル描画
	DrawModel();

	// エフェクト描画
	for (int i = 0; i < effect.size(); i++) {
		effect[i]->Draw();
	}

#if(NDEBUG)
	
	DrawSphere3D(pos.GetV(), radius + attack_range_radius, 20, SKYBLUE, SKYBLUE, false);
	
	DrawSphere3D(pos.GetV(), radius + range_radius, 20, YELLOW, YELLOW, false);

	DrawCapsule3D(GetCapsuleUp(pos.GetV(), 5.0f), GetCapsuleDown(pos.GetV()), GetRadius(), 20, GREEN, GREEN, FALSE);
#endif
}
//---------------------------------------------
// モデルハンドルを設定
//---------------------------------------------
void Enemy2::SetModelHandle(string str)
{
	string data = "data/enemys/";
	string path = data + str;
	model_type.push_back(resource::LoadModel(path.c_str()));
}
//---------------------------------------------
// テクスチャーを適用
//---------------------------------------------
void Enemy2::ApplyTexture()
{
	for (int i = 0; i < texture_handle.size(); i++) {
		for (int j = 0; j < model_type.size();j++) {
			int num = MV1GetMaterialDifMapTexture(model_type[j], i);
			MV1SetTextureGraphHandle(model_type[j], num, texture_handle[i], FALSE);
		}
	}
	texture_handle.clear();
}
//---------------------------------------------
// アニメーションを設定
//---------------------------------------------
void Enemy2::SetAnim()
{
	if (type == old_type) { return; }
	play_anim = 0.0f;
	// 今までアタッチしていたアニメーションのデタッチ
	MV1DetachAnim(model_type[type], anim_num);
	// モデルの anim_num 番目のアニメーションをアタッチする
	attach_anim = MV1AttachAnim(model_type[type], anim_num, model_type[type], TRUE);
	// アタッチしたアニメーションの総再生時間を取得する
	anim_time = MV1GetAttachAnimTotalTime(model_type[type], attach_anim);
}
//---------------------------------------------
// モデルの描画
//---------------------------------------------
void Enemy2::DrawModel()
{
	// modelのポジションを設定
	MV1SetPosition(model_type[type], pos.GetV());
	// modelの向きを設定
	MV1SetRotationXYZ(model_type[type], rot.GetV());
	// modelのサイズを設定
	MV1SetScale(model_type[type], scale.GetV());
	// アニメーション
	MV1SetAttachAnimTime(model_type[type], attach_anim, play_anim);
	// npc.xモデルの描画
	MV1DrawModel(model_type[type]);
}
