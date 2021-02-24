#include "CollisionBase.h"

//---------------------------------------------
// �R���X�g���N�^
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
// �f�X�g���N�^
//---------------------------------------------
CollisionBase::~CollisionBase()
{
}
//---------------------------------------------
//  HP �̏���
//	��O��HP��ۑ�
//	HP �����炷
//---------------------------------------------
void CollisionBase::HpProcessing(int hp_)
{
	old_hp = hp;
	hp -= hp_; 
}
//---------------------------------------------
// ���G����
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


