#include "Ninin.h"
#include "WinMain.h"


//-----------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------
Ninin::Ninin()
{
}
//-----------------------------------------------------------
// �����t���R���X�g���N�^
//-----------------------------------------------------------
Ninin::Ninin(float x_, float y_, bool dir_,float draw_x_)
{
	x = x_;
	y = y_;
	dir = dir_;
	draw_x = draw_x_;
	Init();
}
//-----------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------
Ninin::~Ninin()
{
	Exit();
}

//-----------------------------------------------------------
//�@���������� 
//-----------------------------------------------------------
void Ninin::Init()
{
	ReloadGraph();

	ptr = 0;
	speed = NININ_SPEED;
	gspd = NININ_G;
	jspd = NININ_JUMP_POWER;
}
//-----------------------------------------------------------
// �I������
//-----------------------------------------------------------
void Ninin::Exit()
{
	for (int i = 0; i < NININ_PTR_MAX; i++) {
		DeleteGraph(image[i]);
	}
}
//-----------------------------------------------------------
// �X�V����
//-----------------------------------------------------------
void Ninin::Update()
{
	SetOld();
	if (MoveCheckRange()) {
		Move();
	}
	Animation();
	Dir_Change();
	Jump();
	Gravity();
}
//-----------------------------------------------------------
// �`�揈��
//-----------------------------------------------------------
void Ninin::Draw()
{
	switch (SetDraw())
	{
	case NININ_NORMAL:
		NormalDraw();
		break;
	case NININ_DASH:
		DashDraw();
		break;
	default:
		break;
	}
}
//-----------------------------------------------------------
// �摜�̓ǂݍ��݊֐�
//-----------------------------------------------------------
void Ninin::ReloadGraph()
{
	image[NININ_PTR_01] = LoadGraph("data/player/ninin/ninin01.png");
}


//-----------------------------------------------------------
// �_�b�V���`��
//-----------------------------------------------------------
void Ninin::DashDraw()
{
	if (dir) {
		DrawRectGraphF(x, y, (ptr + 3)* NININ_X_SIZE, 4 * NININ_Y_SIZE - 27,
			NININ_X_SIZE, NININ_Y_SIZE, image[NININ_PTR_01], TRUE, FALSE);
	}
	else {
		DrawRectGraphF(x, y, (ptr + 3)* NININ_X_SIZE, 4 * NININ_Y_SIZE - 27,
			NININ_X_SIZE, NININ_Y_SIZE, image[NININ_PTR_01], TRUE, TRUE);
	}
}
//-----------------------------------------------------------
// �ʏ펞�`��
//-----------------------------------------------------------
void Ninin::NormalDraw()
{
	if (dir) {
		DrawRectGraphF(x,y, ptr * NININ_X_SIZE, 2 * NININ_Y_SIZE - 37,
			NININ_X_SIZE, NININ_Y_SIZE, image[NININ_PTR_01], TRUE, FALSE);
	}
	else {
		DrawRectGraphF(x,y, ptr * NININ_X_SIZE, 2 * NININ_Y_SIZE - 37,
			NININ_X_SIZE, NININ_Y_SIZE, image[NININ_PTR_01], TRUE, TRUE);
	}
}
//-----------------------------------------------------------
// ����`�悷�邩��ݒ�
//-----------------------------------------------------------
int Ninin::SetDraw()
{
	if (CheckHitKey(KEY_INPUT_LSHIFT)) {
		return NININ_DASH;
	}

	return NININ_NORMAL;
}
