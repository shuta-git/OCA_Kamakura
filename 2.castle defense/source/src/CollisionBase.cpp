#include "CollisionBase.h"

//---------------------------------------------
// コンストラクタ
//---------------------------------------------
CollisionBase::CollisionBase()
{
	hp = 0;
	old_hp = 0;
	damage = 0;
	invincible_timer = 0;
	old_anim_num = 100;
	anim_num = 0;
	attach_anim = 0;
	score = 0;
	
	anim_time = 0.0f;
	play_anim = 0.0f;

	death = false;
	range = false;
	attack_range = false;
	invincible = false;

	radius = 0.0f;

	play_anim_speed = 1.0f;
}
//---------------------------------------------
// デストラクタ
//---------------------------------------------
CollisionBase::~CollisionBase()
{
}
//---------------------------------------------
//  HP の処理
//	一つ前のHPを保存
//	HP を減らす
//---------------------------------------------
void CollisionBase::HpProcessing(int hp_)
{
	old_hp = hp;
	hp -= hp_; 
}
//---------------------------------------------
// 無敵処理
//---------------------------------------------
void CollisionBase::Invincible()
{
	invincible_timer++;
	invincible_timer %= MAX_INVUNCIBLE_TIME;
	if (invincible_timer == 0) {
		invincible = false;
		invincible_timer = 0;
	}
}


