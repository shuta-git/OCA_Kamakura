#include "EnemyBase.h"

//---------------------------------------------
// コンストラクタ
//---------------------------------------------
EnemyBase::EnemyBase()
{
	model = 0;
	recast_time = 0;

	player_pos = { 0.0f,0.0f,0.0f };
	goal_pos = { 0.0f,0.0f,30.0f };
	int num = GetRand(1);
	if (num == 0) {
		goal_pos.x = (float)GetRand(7);
	}
	else {
		goal_pos.x = (float)GetRand(11);
		goal_pos.x *= -1;
	}
	goal = false;
	death_anim = false;
	range_radius = 0.0f;
	attack_range_radius = 0.0f;
}
//---------------------------------------------
// デストラクタ
//---------------------------------------------
EnemyBase::~EnemyBase()
{
}
//---------------------------------------------
// アニメーションを設定
//---------------------------------------------
void EnemyBase::SetAnim()
{
	if (anim_num == old_anim_num) { return; }
	play_anim = 0.0f;
	// 今までアタッチしていたアニメーションのデタッチ
	MV1DetachAnim(model, anim_num);
	// モデルの anim_num 番目のアニメーションをアタッチする
	attach_anim = MV1AttachAnim(model,anim_num,model,TRUE);
	// アタッチしたアニメーションの総再生時間を取得する
	anim_time = MV1GetAttachAnimTotalTime(model,attach_anim);
}
//---------------------------------------------
// アニメーションの更新
//---------------------------------------------
void EnemyBase::UpdateAnim()
{
	play_anim += play_anim_speed;
	// 再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す
	if (play_anim >= anim_time)
	{
		play_anim = 0.0f;
	}
}
//---------------------------------------------
// プレイヤー座標を設定
//---------------------------------------------
void EnemyBase::SetPlayerPos(VECTOR player_pos_)
{
	player_pos = player_pos_;
}
//---------------------------------------------
// old 変数設定
//---------------------------------------------
void EnemyBase::SetOld()
{
	old_pos = pos;
	old_anim_num = anim_num;
}
//---------------------------------------------
// テクスチャーのパスを取得
//---------------------------------------------
void EnemyBase::SetTextureHandle(string str)
{
	// パスを設定
	string data = "data/enemys/textures/";
	string path = data + str;
	texture_handle.push_back(resource::LoadGraph(path.c_str()));
}
//---------------------------------------------
// テクスチャーを適用
//---------------------------------------------
void EnemyBase::ApplyTexture()
{
	for (int i = 0; i < texture_handle.size(); i++) {
		int num = MV1GetMaterialDifMapTexture(model, i);
		MV1SetTextureGraphHandle(model, num, texture_handle[i], FALSE);
	}

	texture_handle.clear();
}
//---------------------------------------------
// 近づく  目的地の座標を指定
//---------------------------------------------
void EnemyBase::Approach(VECTOR goal_)
{
	VECTOR goal;
	// 目的地
	goal = goal_;

	VECTOR sub;
	sub.x = goal.x - pos.x;
	sub.z = goal.z - pos.z;

	float rot_y;
	rot_y = atan2f(sub.z, sub.x);
	rot.y = atan2f(sub.x, sub.z);

	pos.x += speed * cosf(rot_y);
	pos.z += speed * sinf(rot_y);
}
//---------------------------------------------
// ゴールチェック
//---------------------------------------------
void EnemyBase::CheckGoal()
{
	if (goal_pos.z <= pos.z && -11.5f < pos.x && 7.5f > pos.x) {
		goal = true;
	}
}
//---------------------------------------------
// 目標地点についた時の処理
//---------------------------------------------
bool EnemyBase::GetGoal()
{
	return goal;
}
//---------------------------------------------
// モデルハンドルを設定
//---------------------------------------------
void EnemyBase::SetModelHandle(string str)
{
	string data = "data/enemys/";
	string path = data + str;
	model = MV1DuplicateModel(resource::LoadModel(path.c_str()));
}


