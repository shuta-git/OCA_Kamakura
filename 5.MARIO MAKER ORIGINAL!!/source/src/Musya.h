#pragma once
#include "PlayerBase.h"

//-----------------------------------------------------------
// �萔
//-----------------------------------------------------------
#define MUSYA_SPEED   5.0F			// �L�����̈ړ��X�s�[�h
#define MUSYA_SIZE    64			// �L�����̃T�C�Y
#define MUSYA_X_SIZE  40			// �L������X�T�C�Y
#define MUSYA_Y_SIZE  48			// �L������Y�T�C�Y
#define MUSYA_JUMP_POWER	8.0F   // �L�����̃W�����v��
#define MUSYA_G       0.6F

//-----------------------------------------------------------
// �N���X
//-----------------------------------------------------------
class Musya : public PlayerBase
{
public:
	Musya();
	Musya(float x_, float y_,bool dir_,float draw_x_);
	~Musya();
	void Init() override;				// ����������
	void Exit() override;				// �I������
	void Update() override;				// �X�V����
	void Draw() override;				// �`�揈��
	void ReloadGraph() override;		// �摜�̓ǂݍ��݊֐�

private:

	void DashDraw() override;
	void NormalDraw() override;
	int SetDraw() override;
};

//-----------------------------------------------------------
// �񋓑�
//-----------------------------------------------------------
enum Musya_Dir {
	MUSYA_PTR_01 = 0,
	//MUSYA_PTR_02,
	//MUSYA_PTR_03,
	//MUSYA_PTR_04,

	MUSYA_PTR_MAX,
};
enum Musya_Draw {
	MUSYA_NORMAL = 0,
	MUSYA_DASH,

};













