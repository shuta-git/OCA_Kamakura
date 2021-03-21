#pragma once

//---------------------------------------------
// PlayerManager�N���X
//---------------------------------------------
class PlayerManager
{
public:
	PlayerManager();
	~PlayerManager();

	// �X�V
	void Update();
	// �`��
	void Draw();

	//=======================================
	// Get�֐�
	//=======================================

	// Player�^�Ńv���C���[��Ԃ�
	Player* GetPlayer()const;

private:

	Camera* camera = nullptr;	// �J�����쐬
	Player* player = nullptr;	// �v���C���[�쐬

};

