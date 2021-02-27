#include "SceneManager.h"

//---------------------------------------------
// �R���X�g���N�^
//---------------------------------------------
SceneManager::SceneManager()
{
	scenes = new TitleScene();
	operation = nullptr;
}
//---------------------------------------------
// �f�X�g���N�^
//---------------------------------------------
SceneManager::~SceneManager()
{
}
//---------------------------------------------
// �X�V����
//---------------------------------------------
void SceneManager::Update()
{
	// �V�[����؂�ւ�
	ChangeScene();
	// �؂�ւ��Ă����ɃV�[���𐶐�
	CreatScene();

	if (scenes != nullptr) {
		// �V�[�����|�[�Y�������������Ȃ�������
		if (scene_type != PAUSE &&
			scene_type != OPERATION)
		{
			// �X�V����
			scenes->Update();
		}
	}
	// �X�V����
	if (operation != nullptr) {
		operation->Update();
	}
	if (pause != nullptr) {
		pause->Update();
	}

	// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
	UpdateEffekseer2D();
}
//---------------------------------------------
// �`�揈��
//---------------------------------------------
void SceneManager::Draw()
{
	if (scenes != nullptr) {
		scenes->Draw();
	}
	if (operation != nullptr) {
		operation->Draw();
	}
	if (pause != nullptr) {
		pause->Draw();
	}
}
//---------------------------------------------
// �V�[���̐؂�ւ�
//---------------------------------------------
void SceneManager::ChangeScene()
{
	switch (scene_type)
	{
		// �^�C�g��
	case TITLE:
		CheckChangeScene(scenes, TITLE);
		break;
		// �Z���N�g
	case SELECT:
		CheckChangeScene(scenes, SELECT);
		break;
		// ���[�h
	case LOAD:
		CheckChangeScene(scenes, LOAD);
		break;
		// �Q�[�����C��
	case GAME_MAIN:
		if (scenes->GetSceneType() == PAUSE) {
			scene_type = scenes->GetSceneType();
			break;
		}
		CheckChangeScene(scenes, GAME_MAIN);
		break;
		// �������
	case OPERATION:
		CheckChangeScene(operation, OPERATION);
		break;
		// �|�[�Y
	case PAUSE:
		CheckChangeScene(pause, PAUSE);
		break;
		// ���U���g
	case RESULT:
		CheckChangeScene(scenes, RESULT);
		break;
	}
}
//---------------------------------------------
// �V�[���𐶐�
//---------------------------------------------
void SceneManager::CreatScene()
{
	switch (scene_type)
	{
		// �^�C�g��
	case TITLE:
		if (scenes == nullptr) {
			scenes = new TitleScene();
		}
		break;
		// �Z���N�g
	case SELECT:
		if (scenes == nullptr) {
			scenes = new SelectScene();
		}
		break;
		// ���[�h
	case LOAD:
		if (scenes == nullptr) {
			scenes = new LoadScene();
		}
		break;
		// �Q�[�����C��
	case GAME_MAIN:
		if (scenes == nullptr) {
			scenes = new GameMain();
		}
		break;
		// �������
	case OPERATION:
		if (operation == nullptr) {
			operation = new OperationScene(old_scene);
		}
		break;
		// �|�[�Y
	case PAUSE:
		if (pause == nullptr) {
			pause = new PauseScene();
		}
		break;
		// ���U���g
	case RESULT:
		if (scenes == nullptr) {
			scenes = new ResultScene();
		}
		break;
	}
}
//---------------------------------------------
// �V�[�����ς���Ă邩�`�F�b�N
//---------------------------------------------
void SceneManager::CheckChangeScene(SceneBase *b, int i)
{
	// �V�[���������������炱�̊֐��𔲂���
	if (b->GetSceneType() == i) { return; }

	// �V�[�������[�h����Ȃ��Ƃ�
	if (scene_type != LOAD && scene_type != PAUSE && scene_type != OPERATION) {
		// resource�ɕۑ����Ă���f�[�^�����ׂď���
		resource::DeleteHandleTable();
	}

	// �V�[����ۑ�
	old_scene = scene_type;
	// ���̃V�[�����V�[���^�C�v�ɓ����
	scene_type = b->GetSceneType();

	// �i�V�[��
	if (b == scenes) {
		ScenesDelete();
	}
	// ����
	if (b == operation) {
		OperationDelete();
	}
	// �|�[�Y
	if (b == pause) {
		// �V�[�������C����������
		if (pause->GetSceneType() == GAME_MAIN) {
			// scenes�ɃV�[���^�C�v��ݒ�
			scenes->SetSceneType(GAME_MAIN);
			// �|�[�Y�̏���
			PauseDelete();
			return;
		}
		// �|�[�Y���̃V�[���������ʂɂȂ�����
		if (pause->GetSceneType() == OPERATION) {
			// �|�[�Y��ʂ̏���
			PauseDelete();
			return;
		}
		PauseDelete();
		ScenesDelete();
	}
}
//---------------------------------------------
// scenes�̃f���[�g����
//---------------------------------------------
void SceneManager::ScenesDelete()
{
	if (scenes == nullptr) { return; }

	scenes->DeleteFontHandle();

	delete scenes;
	scenes = nullptr;
}
//---------------------------------------------
// operation�̃f���[�g����
//---------------------------------------------
void SceneManager::OperationDelete()
{
	if (operation == nullptr) { return; }

	operation->DeleteFontHandle();

	delete operation;
	operation = nullptr;
}
//---------------------------------------------
// pause�̃f���[�g����
//---------------------------------------------
void SceneManager::PauseDelete()
{
	if (pause == nullptr) { return; }

	pause->DeleteFontHandle();

	delete pause;
	pause = nullptr;
}
