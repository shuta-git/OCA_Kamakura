#include "Map.h"

//----------------------------------------------------------------------
// �f�X�g���N�^
//----------------------------------------------------------------------
Map::~Map()
{
}
//----------------------------------------------------------------------
// �u���b�N�̏���
//----------------------------------------------------------------------
void Map::HitCheckPB(Block & block)
{
	//-------------------
	// �����蔻��
	//-------------------
	// ��
	if (Circle(player->GetPosX(), player->GetPosY() - player->GetRot(), 1).intersects(block.GetRect())) {
		player->InitM_Pos();
		player->CahegFlag();
	}
}
//----------------------------------------------------------------------
// �g���b�v�̏���
//----------------------------------------------------------------------
void Map::HitCheckPT(Trap & trap)
{
	t_drop_pos = trap.GetDropTrapPos();
	//-------------------
	// �����蔻��
	//-------------------
	// �g���b�v�Ɠ���������player�����S
	if (GetPlayer().intersects(trap.GetTriangle())) {
		p_death = true;
	}
	// ��яo��g���b�v�͈̔�
	if (GetPlayer().intersects(trap.GetRangeTRIANGLE_OUT())) {
		is_tout_range = true;
	}
	//�@�h���b�v�g���b�v�͈̔�
	if (GetPlayer().intersects(trap.GetRangeTRIANGLE_DROP())) {
		trap.Is_DropChange();
	}
	// �h���b�v�g���b�v�̏��
	if (Circle(player->GetPosX(), player->GetPosY() - player->GetRot(), 1).intersects(trap.GetOnTriagle_Drop())) {
		player->InitM_Pos();
		player->CahegFlag();
	}
}
//----------------------------------------------------------------------
// is_tout_range��ݒ�(TRIANGLE_OUT)
//----------------------------------------------------------------------
void Map::SetTOutRange()
{
	if (is_tout_range) {
		is_tout_range = false;
	}
}
//----------------------------------------------------------------------
// �X�V����
//----------------------------------------------------------------------
void Map::Update()
{
#if ! defined(NDEBUG)
	// R�L�[���������畜��
	if (Input::KeyR.clicked || controller.buttonY.clicked && p_death) {
		player->Init();
		p_death = false;
	}
#else
#endif

	// �������Z���X�V
	world.update();

	if (player->GetPosY() < 0) {
		p_death = true;
	}

	if (p_death == false) {
		// �v���C���[�̍X�V
		player->Update();
	}
}
//----------------------------------------------------------------------
// �`�揈��
//----------------------------------------------------------------------
void Map::Draw()
{
	
		// �v���C���[�̕`��
		player->Draw();
	
	
}

