#pragma once
#include "PlayerBase.h"

//-----------------------------------------------------------
// �萔
//-----------------------------------------------------------
#define FIONA_SPEED   5.0F			// �L�����̈ړ��X�s�[�h
#define FIONA_SIZE    64			// �L�����̃T�C�Y
#define FIONA_X_SIZE  40			// �L������X�T�C�Y
#define FIONA_Y_SIZE  48			// �L������Y�T�C�Y
#define FIONA_JUMP_POWER	10.0F   // �L�����̃W�����v��
#define FIONA_G       0.6F

//-----------------------------------------------------------
// �N���X
//-----------------------------------------------------------
class Fiona : public PlayerBase
{
public:
	Fiona();
	Fiona(float x_, float y_,bool dir_,float draw_x_);
	~Fiona();
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
enum Fiona_Dir {
	FIONA_PTR_01 = 0,
	FIONA_PTR_02,
	FIONA_PTR_03,
	FIONA_PTR_04,

	FIONA_PTR_MAX,
};
enum Fiona_Draw {
	FIONA_NORMAL = 0,
	FIONA_DASH ,
	
};








