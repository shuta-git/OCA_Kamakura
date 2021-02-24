#include "Effect.h"

//---------------------------------------------
// �R���X�g���N�^
//---------------------------------------------
Effect::Effect()
{

}
//---------------------------------------------
// �R���X�g���N�^
// �t�@�C���p�X�@�|�W�V�����@true�RD�@false2D
//---------------------------------------------
Effect::Effect(string str, Vector3 pos_, bool d)
{
	playingEffectHandle = -1;

	pos = pos_;
	rot = Vector3(0.0f, 0.0f, 0.0f);
	scale = Vector3(1.0f, 1.0f, 1.0f);
	speed = 1.0f;

	// �p�X��ݒ�
	string data = "data/effect/";
	string path = data + str;
	save_path = path;
	// �n���h���Ƀp�X��ݒ�
	handle = resource::LoadEffekseerEffect(path.c_str());
	// �RD���QD
	D = d;

	if (D) {
		// �G�t�F�N�g���Đ�����B
		playingEffectHandle = PlayEffekseer3DEffect(handle);
	}
	else {
		// �G�t�F�N�g���Đ�����B
		playingEffectHandle = PlayEffekseer2DEffect(handle);
	}
}
//---------------------------------------------
// �f�X�g���N�^
//---------------------------------------------
Effect::~Effect()
{
}
//---------------------------------------------
// �X�V
//---------------------------------------------
void Effect::Update()
{
	if (D) {
		// �|�W�V������ݒ�
		SetPosPlayingEffekseer3DEffect(playingEffectHandle, pos.x, pos.y, pos.z);
		// �����̐ݒ�
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle, rot.x, rot.y, rot.z);
		// �T�C�Y�̐ݒ�
		SetScalePlayingEffekseer3DEffect(playingEffectHandle, scale.x, scale.y, scale.z);
		// �Đ����x
		SetSpeedPlayingEffekseer3DEffect(playingEffectHandle, speed);
	}
	else {
		// �|�W�V������ݒ�
		SetPosPlayingEffekseer2DEffect(playingEffectHandle, pos.x, pos.y, pos.z);
		// �����̐ݒ�
		SetRotationPlayingEffekseer2DEffect(playingEffectHandle, rot.x, rot.y, rot.z);
		// �T�C�Y�̐ݒ�
		SetScalePlayingEffekseer2DEffect(playingEffectHandle, scale.x, scale.y, scale.z);
		// �Đ����x
		SetSpeedPlayingEffekseer2DEffect(playingEffectHandle, speed);
	}
}
//---------------------------------------------
// �G�t�F�N�g���Đ������擾
//---------------------------------------------
int  Effect::IsEffectPlaying(string str)
{
	if (CheckPath(str)) {
		return IsEffekseer3DEffectPlaying(playingEffectHandle);
	}
}
//---------------------------------------------
// ���W�̐ݒ�
//---------------------------------------------
void Effect::SetPos(Vector3 pos_)
{
	pos = pos_;
}
//---------------------------------------------
// ���x�̐ݒ�
//---------------------------------------------
void Effect::SetSpeed(float speed_)
{
	speed = speed_;
}
//---------------------------------------------
// �X�P�[���̐ݒ�
//---------------------------------------------
void Effect::SetScale(Vector3 scale_)
{
	scale = scale_;
}
//---------------------------------------------
// �����̐ݒ�
//---------------------------------------------
void Effect::SetRot(Vector3 rot_)
{
	rot = rot_;
}
//---------------------------------------------
// �`��
//---------------------------------------------
void Effect::Draw()
{
	if (D) {
		// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
		DrawEffekseer3D();
	}
	else {
		// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
		DrawEffekseer2D();
	}
}
//---------------------------------------------
// �p�X���m�F
//---------------------------------------------
bool Effect::CheckPath(string str)
{
	string data = "data/effect/";
	string path = data + str;
	if (save_path == path) {
		return true;
	}
	return false;
}
