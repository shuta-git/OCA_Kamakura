#include "SceneBase.h"

//---------------------------------------------
// �R���X�g���N�^
//---------------------------------------------
SceneBase::SceneBase()
{
	x = 0;
	y = 0;
	size = 1.0f;
}
//---------------------------------------------
// �f�X�g���N�^
//---------------------------------------------
SceneBase::~SceneBase()
{
}
//---------------------------------------------
// �V�[���̎�ނ��擾
//---------------------------------------------
int SceneBase::GetSceneType() const
{
	return scene_type;
}
//---------------------------------------------
// �V�[���̎�ނ�ݒ�
//---------------------------------------------
void SceneBase::SetSceneType(int scene_type_)
{
	scene_type = scene_type_;
}
//---------------------------------------------
// �O���t�B�b�N�̃p�X��ݒ�
// "data/"�@��������p�X�����
//---------------------------------------------
void SceneBase::SetGraphHandle(string str, int x_, int y_, float size_)
{
	// �p�X��ݒ�
	string data = "data/scene/";
	string path = data + str;
	// �O���t�B�b�N�n���h�������Ă���
	graph_handle.push_back(resource::LoadGraph(path.c_str()));
	// �n���h���̐ݒ�
	handle_path.push_back(path);
	// ���W�ݒ�
	v_x.push_back(x_);
	v_y.push_back(y_);
	// �T�C�Y�̐ݒ�
	v_size.push_back(size_);
	// �`�悷��
	v_active.push_back(true);
	// �����x��L���ɂ��邩
	v_trans.push_back(TRUE);
}
//---------------------------------------------
// �摜�̍��W��ݒ�
//---------------------------------------------
void SceneBase::DrawSetPosition(string str, int x_, int y_)
{
	// �p�X��ݒ�
	string data = "data/scene/";
	string path = data + str;
	for (int i = 0; i < graph_handle.size(); i++) {
		if (handle_path[i] == path) {
			v_x[i] = x_;
			v_y[i] = y_;
		}
	}
}
//---------------------------------------------
// �����x�̐ݒ�
//---------------------------------------------
void SceneBase::SetTrans(string str, bool trans)
{
	// �p�X��ݒ�
	string data = "data/scene/";
	string path = data + str;
	for (int i = 0; i < graph_handle.size(); i++) {
		if (handle_path[i] == path) {
			v_trans[i] = trans;
		}
	}
}
//---------------------------------------------
// �摜��`�悷�邩�w��
//---------------------------------------------
void SceneBase::DrawSetActive(string str, bool active)
{
	// �p�X��ݒ�
	string data = "data/scene/";
	string path = data + str;
	for (int i = 0; i < graph_handle.size(); i++) {
		if (handle_path[i] == path) {
			v_active[i] = active;
		}
	}
}
//---------------------------------------------
// �O���t�B�b�N�n���h���̕`��
//---------------------------------------------
void SceneBase::DrawGraphHandle()
{
	for (int i = 0; i < graph_handle.size(); i++) {
		if (v_active[i]) {
			DrawRotaGraph(v_x[i], v_y[i], v_size[i], 0, graph_handle[i], v_trans[i], FALSE);
		}
	}
}
//---------------------------------------------
// �G�t�F�N�g���~�܂��Ă�����f���[�g
//---------------------------------------------
void SceneBase::DeleteEffect(int i)
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
void SceneBase::StopEffect(int i)
{
	if (effect[i]->GetD()) {
		StopEffekseer3DEffect(effect[i]->GetPlayingEffect());
	}
	else {
		StopEffekseer2DEffect(effect[i]->GetPlayingEffect());
	}
}
//---------------------------------------------
// ���������ŕ`��
// �ǂ̈ʒu���璆���������邩�@Y���W�@
// �o�͂��������� �t�H���g�n���h�� �F
//---------------------------------------------
void SceneBase::DrawCenterString(int x_,int y_, string str_, unsigned int color_, int font_handle_)
{
	int x = GetDrawStringWidthToHandle(str_.c_str(), (int)strlen(str_.c_str()), font_handle_) / 2;

	DrawStringToHandle(x_ - x,y_,str_.c_str(), color_,font_handle_);
}
//---------------------------------------------
// ���������Ńt�H�[�}�b�g�w��`��
// �ǂ̈ʒu���璆���������邩�@Y���W�@
// �F �t�H���g�n���h�� �o�͂��������� �ϐ�
//---------------------------------------------
void SceneBase::DrawCenterFormatString(int x_, int y_, unsigned int color_, int font_handle_, string str_, int num)
{
	int x = GetDrawStringWidthToHandle(str_.c_str(), (int)strlen(str_.c_str()), font_handle_) / 2;

	DrawFormatStringToHandle(x_ - x, y_, color_,font_handle_,str_.c_str(),num);
}
//---------------------------------------------
// �E�l�߂ŕ`��
// Y���W�@�o�͂���������
// �t�H���g�n���h�� �F
//---------------------------------------------
void SceneBase::DrawRightString(int y_, string str_, unsigned int color_, int font_handle_)
{
	int x = GetDrawStringWidthToHandle(str_.c_str(), (int)strlen(str_.c_str()), font_handle_);

	DrawStringToHandle( WINDOW_W - x, y_, str_.c_str(), color_, font_handle_);
}
//---------------------------------------------
// �E�l�߂ăt�H�[�}�b�g�w��`��
// Y���W�@�F �t�H���g�n���h��
// �o�͂��������� �ϐ�
//---------------------------------------------
void SceneBase::DrawRightFormatString(int y_, unsigned int color_, int font_handle_, string str_, int num)
{
	int x = GetDrawStringWidthToHandle(str_.c_str(), (int)strlen(str_.c_str()), font_handle_);

	DrawFormatStringToHandle(WINDOW_W - x, y_, color_, font_handle_, str_.c_str(), num);
}
//---------------------------------------------
// �����̐擪��X���W���擾
//---------------------------------------------
int SceneBase::GetStringPosX(int x_, string str_, int font_handle_)
{
	int x = GetDrawStringWidthToHandle(str_.c_str(), (int)strlen(str_.c_str()), font_handle_) / 2;

	return x_ - x;
}
//---------------------------------------------
// �t�H���g�n���h��������
//---------------------------------------------
void SceneBase::DeleteFontHandle()
{
	for (int i = 0; i < font_handle.size();i++) {
		DeleteFontToHandle(font_handle[i]);
	}
}
//---------------------------------------------
// vector�̒��g��nullptr�̎�����
//---------------------------------------------
void SceneBase::EraseEffect()
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