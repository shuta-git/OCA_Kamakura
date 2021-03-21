#include "PlayerManager.h"

//---------------------------------------------
// �R���X�g���N�^
//---------------------------------------------
PlayerManager::PlayerManager()
{
	// �J�����̏�����
	if (camera == nullptr) { camera = new Camera(); }
	// �v���C���[�̏�����
	if (player == nullptr) { player = new Player(); }
}
//---------------------------------------------
// �f�X�g���N�^
//---------------------------------------------
PlayerManager::~PlayerManager()
{
	if (camera != nullptr) {
		delete camera;
		camera = nullptr;
	}
	if (player != nullptr) {
		delete player;
		player = nullptr;
	}
}
//---------------------------------------------
// �X�V
//---------------------------------------------
void PlayerManager::Update()
{
	// �v���C���[�ɃJ������]Vector��ݒ�
	if (player != nullptr) {
		player->SetCameraRot(camera->GetRot());
	}
	// �v���C���[�X�V
	if (player != nullptr) {
		player->Update();
	}
	// �J�����Ƀv���C���[��Vector���W�Ɖ�]�p��Vector��ݒ�
	if (camera != nullptr) {
		camera->SetTarget(player->GetPos());
		camera->SetRot(player->GetRot());
	}
	// �J�����̍X�V
	if (camera != nullptr) {
		camera->Update();
	}
}
//---------------------------------------------
// �`��
//---------------------------------------------
void PlayerManager::Draw()
{
	// �J�����`��
	if (camera != nullptr) { camera->Draw(); }
	//�@�v���C���[�`��
	if (player != nullptr) { player->Draw(); }
}
//---------------------------------------------
// Player�^�Ńv���C���[��Ԃ�
//---------------------------------------------
Player * PlayerManager::GetPlayer() const
{
	if (player == nullptr)return nullptr;
	return (Player*)player;
}

