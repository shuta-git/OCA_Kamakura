#pragma once

//---------------------------------------------
// SceneManager�N���X
//---------------------------------------------
class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void Update();
	void Draw();

private:

	int scene_type;		// �V�[���̎��
	int old_scene;		// ��O�̃V�[��

	SceneBase* scenes;
	SceneBase* operation;
	SceneBase* pause;

	// scenes�̃f���[�g����
	void ScenesDelete();
	// operation�̃f���[�g����
	void OperationDelete();
	// pause�̃f���[�g����
	void PauseDelete();
	// �V�[���؂�ւ�
	void ChangeScene();
	// �V�[���𐶐�
	void CreatScene();
	// �V�[�����ς���Ă邩�`�F�b�N
	void CheckChangeScene(SceneBase *b, int i);
};

