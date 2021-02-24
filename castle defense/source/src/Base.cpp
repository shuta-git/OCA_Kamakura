#include "Base.h"

//---------------------------------------------
// �R���X�g���N�^
//---------------------------------------------
Base::Base()
{
	pos = Vector3( 0.0f,0.0f,0.0f );
	old_pos = Vector3(0.0f, 0.0f, 0.0f);
	rot = Vector3( 0.0f,0.0f,0.0f );
	scale = Vector3(1.0f, 1.0f, 1.0f);

	mat = MGetIdent();
	mat_trans = MGetIdent();
	mat_rot = MGetIdent();

	speed = 0.0f;
}
//---------------------------------------------
// �f�X�g���N�^
//---------------------------------------------
Base::~Base()
{
}
void Base::MatrixInit()
{
	mat = MGetIdent();
	mat_trans = MGetIdent();
	mat_rot = MGetIdent();

}
void Base::RotMatrixInit()
{
	mat_rot = MGetIdent();
}
void Base::TransMatrixInit()
{
	mat_trans = MGetIdent();
}
//---------------------------------------------
// �J�v�Z���̏㕔��
//---------------------------------------------
VECTOR Base::GetCapsuleUp(VECTOR pos_,float size_) const
{
	VECTOR result;

	result.x = pos_.x;
	result.y = pos_.y + size_;
	result.z = pos_.z;

	return result;
}
//---------------------------------------------
// �J�v�Z���̉�����
//---------------------------------------------
VECTOR Base::GetCapsuleDown(VECTOR pos_) const
{
	VECTOR result;

	result.x = pos_.x;
	result.y = pos_.y + radius;
	result.z = pos_.z;

	return result;
}
//---------------------------------------------
// �J�v�Z���̑O����
//---------------------------------------------
VECTOR Base::GetCapsuleTip(VECTOR pos_, float size_) const
{
	VECTOR result;

	result.x = pos_.x;
	result.y = pos_.y;
	result.z = pos_.z + size_;

	return result;
}
//---------------------------------------------
// �J�v�Z���̌�땔��
//---------------------------------------------
VECTOR Base::GetCapsuleRoot(VECTOR pos_) const
{
	VECTOR result;

	result.x = pos_.x;
	result.y = pos_.y;
	result.z = pos_.z;

	return result;
}
//---------------------------------------------
// �G�t�F�N�g���~�܂��Ă�����f���[�g
//---------------------------------------------
void Base::DeleteEffect(int i)
{
	if (effect[i]->GetD()) {
		// ���ԁiGetPlayingEffect�j�̃G�t�F�N�g���~�܂������𔻒�
		if (IsEffekseer3DEffectPlaying(effect[i]->GetPlayingEffect()) == 0)
		{
			return;
		}
	}
	else {
		// ���ԁiGetPlayingEffect�j�̃G�t�F�N�g���~�܂������𔻒�
		if (IsEffekseer2DEffectPlaying(effect[i]->GetPlayingEffect()) == 0)
		{
			return;
		}
	}

	// �~�܂����ԍ��̃G�t�F�N�g���f���[�g�A������
	delete effect[i];
	effect[i] = nullptr;

	// �G�t�F�N�g���~�܂��Ă�����
	// �~�܂����G�t�F�N�g���T�C�Y���k�߂�
	EraseEffect();
}
//---------------------------------------------
// �G�t�F�N�g���~
//---------------------------------------------
void Base::StopEffect(int i)
{
	if (effect[i]->GetD()) {
		StopEffekseer3DEffect(effect[i]->GetPlayingEffect());
	}
	else {
		StopEffekseer2DEffect(effect[i]->GetPlayingEffect());
	}
}
//---------------------------------------------
// vector�̒��g��nullptr�̎�����
//---------------------------------------------
void Base::EraseEffect()
{
	std::vector<Effect*>::iterator it = effect.begin();
	while (it != effect.end())
	{
		if (*it == nullptr) {
			it = effect.erase(it);
		}
		else {
			it++;
		}
	}
}


