#pragma once
#include "PlayerBase.h"

//-----------------------------------------------------------
// �萔
//-----------------------------------------------------------
#define NININ_SPEED   5.0F			// �L�����̈ړ��X�s�[�h
#define NININ_SIZE    64			// �L�����̃T�C�Y
#define NININ_X_SIZE  40			// �L������X�T�C�Y
#define NININ_Y_SIZE  48			// �L������Y�T�C�Y
#define NININ_JUMP_POWER	9.0F   // �L�����̃W�����v��
#define NININ_G       0.6F

//-----------------------------------------------------------
// �N���X
//-----------------------------------------------------------
class Ninin : public PlayerBase
{
public:
	Ninin();
	Ninin(float x_, float y_, bool dir_,float draw_x_);
	~Ninin();
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
enum Ninin_Dir {
	NININ_PTR_01 = 0,
	//MUSYA_PTR_02,
	//MUSYA_PTR_03,
	//MUSYA_PTR_04,

	NININ_PTR_MAX,
};
enum Ninin_Draw {
	NININ_NORMAL = 0,
	NININ_DASH,

};
