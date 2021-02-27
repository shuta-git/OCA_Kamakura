#include "EnemyBase.h"

//---------------------------------------------
// �R���X�g���N�^
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
// �f�X�g���N�^
//---------------------------------------------
EnemyBase::~EnemyBase()
{
}
//---------------------------------------------
// �A�j���[�V������ݒ�
//---------------------------------------------
void EnemyBase::SetAnim()
{
	if (anim_num == old_anim_num) { return; }
	play_anim = 0.0f;
	// ���܂ŃA�^�b�`���Ă����A�j���[�V�����̃f�^�b�`
	MV1DetachAnim(model, anim_num);
	// ���f���� anim_num �Ԗڂ̃A�j���[�V�������A�^�b�`����
	attach_anim = MV1AttachAnim(model,anim_num,model,TRUE);
	// �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
	anim_time = MV1GetAttachAnimTotalTime(model,attach_anim);
}
//---------------------------------------------
// �A�j���[�V�����̍X�V
//---------------------------------------------
void EnemyBase::UpdateAnim()
{
	play_anim += play_anim_speed;
	// �Đ����Ԃ��A�j���[�V�����̑��Đ����ԂɒB������Đ����Ԃ��O�ɖ߂�
	if (play_anim >= anim_time)
	{
		play_anim = 0.0f;
	}
}
//---------------------------------------------
// �v���C���[���W��ݒ�
//---------------------------------------------
void EnemyBase::SetPlayerPos(VECTOR player_pos_)
{
	player_pos = player_pos_;
}
//---------------------------------------------
// old �ϐ��ݒ�
//---------------------------------------------
void EnemyBase::SetOld()
{
	old_pos = pos;
	old_anim_num = anim_num;
}
//---------------------------------------------
// �e�N�X�`���[�̃p�X���擾
//---------------------------------------------
void EnemyBase::SetTextureHandle(string str)
{
	// �p�X��ݒ�
	string data = "data/enemys/textures/";
	string path = data + str;
	texture_handle.push_back(resource::LoadGraph(path.c_str()));
}
//---------------------------------------------
// �e�N�X�`���[��K�p
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
// �߂Â�  �ړI�n�̍��W���w��
//---------------------------------------------
void EnemyBase::Approach(VECTOR goal_)
{
	VECTOR goal;
	// �ړI�n
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
// �S�[���`�F�b�N
//---------------------------------------------
void EnemyBase::CheckGoal()
{
	if (goal_pos.z <= pos.z && -11.5f < pos.x && 7.5f > pos.x) {
		goal = true;
	}
}
//---------------------------------------------
// �ڕW�n�_�ɂ������̏���
//---------------------------------------------
bool EnemyBase::GetGoal()
{
	return goal;
}
//---------------------------------------------
// ���f���n���h����ݒ�
//---------------------------------------------
void EnemyBase::SetModelHandle(string str)
{
	string data = "data/enemys/";
	string path = data + str;
	model = MV1DuplicateModel(resource::LoadModel(path.c_str()));
}


