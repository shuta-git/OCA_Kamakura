//---------------------------------------------
// 定数の宣言
//---------------------------------------------
const float MAX_RANGE_RADIUS = 10.0f;	// 最大範囲
const float MAX_SPEED = 0.1f;			// 最大移動速度
const int DAMAGE = 135;					// ダメージ数

//---------------------------------------------
// コンストラクタ
//---------------------------------------------
Enemy3::Enemy3( Vector3 pos_, Vector3 rot_, Vector3 scale_)
{
	pos = pos_;
	rot = rot_;
	scale = scale_;
	radius = 0.5f;
	range_radius = MAX_RANGE_RADIUS;
	speed = MAX_SPEED;
	damage = DAMAGE;
	SetModelHandle("npc.x");
}
//---------------------------------------------
// デストラクタ
//---------------------------------------------
Enemy3::~Enemy3()
{
	MV1DeleteModel(model);
}
//---------------------------------------------
// 更新
//---------------------------------------------
void Enemy3::Update()
{
	old_pos = pos;
	if (range) {
		// player に向かう
		Approach(GetPlayerPos());
	}
	if (!range && !GetGoal()) {
		// 城に向かう
		Approach(goal_pos);
	}

}
//---------------------------------------------
// 描画
//---------------------------------------------
void Enemy3::Draw()
{// modelのポジションを設定
	MV1SetPosition(model, pos.GetV());
	// modelの向きを設定
	MV1SetRotationXYZ(model, rot.GetV());
	// modelのサイズを設定
	MV1SetScale(model, scale.GetV());
	// noc.xモデルの描画
	MV1DrawModel(model);

#if(NDEBUG)
	DrawSphere3D(pos.GetV(), radius, 20, YELLOW, YELLOW, false);
#endif
}
