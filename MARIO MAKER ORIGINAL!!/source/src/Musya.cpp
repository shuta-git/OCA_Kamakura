#include "Musya.h"
#include "WinMain.h"

//-----------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------
Musya::Musya()
{
}
//-----------------------------------------------------------
// �����t���R���X�g���N�^
//-----------------------------------------------------------
Musya::Musya(float x_, float y_,bool dir_,float draw_x_)
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
Musya::~Musya()
{
	Exit();
}

//-----------------------------------------------------------
//�@���������� 
//-----------------------------------------------------------
void Musya::Init()
{
	ReloadGraph();

	ptr = 0;
	speed = MUSYA_SPEED;
	gspd = MUSYA_G;
	jspd = MUSYA_JUMP_POWER;
}
//-----------------------------------------------------------
// �I������
//-----------------------------------------------------------
void Musya::Exit()
{
	for (int i = 0; i < MUSYA_PTR_MAX; i++) {
		DeleteGraph(image[i]);
	}
}
//-----------------------------------------------------------
// �X�V����
//-----------------------------------------------------------
void Musya::Update()
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
void Musya::Draw()
{
	switch (SetDraw())
	{
	case MUSYA_NORMAL:
		NormalDraw();
		break;
	case MUSYA_DASH:
		DashDraw();
		break;
	default:
		break;
	}
}
//-----------------------------------------------------------
// �摜�̓ǂݍ��݊֐�
//-----------------------------------------------------------
void Musya::ReloadGraph()
{
	image[MUSYA_PTR_01] = LoadGraph("data/player/musya/musya01.png");
}


//-----------------------------------------------------------
// �_�b�V���`��
//-----------------------------------------------------------
void Musya::DashDraw()
{
	if (dir) {
		DrawRectGraphF(x, y, (ptr + 3)* MUSYA_X_SIZE, 4 * MUSYA_Y_SIZE - 27,
			MUSYA_X_SIZE, MUSYA_Y_SIZE, image[MUSYA_PTR_01], TRUE, FALSE);
	}
	else {
		DrawRectGraphF(x, y, (ptr + 3)* MUSYA_X_SIZE, 4 * MUSYA_Y_SIZE - 27,
			MUSYA_X_SIZE, MUSYA_Y_SIZE, image[MUSYA_PTR_01], TRUE, TRUE);
	}
}
//-----------------------------------------------------------
// �ʏ펞�`��
//-----------------------------------------------------------
void Musya::NormalDraw()
{
	if (dir) {
		DrawRectGraphF(x, y, ptr * MUSYA_X_SIZE, 2 * MUSYA_Y_SIZE - 37,
			MUSYA_X_SIZE, MUSYA_Y_SIZE, image[MUSYA_PTR_01], TRUE, FALSE);
	}
	else {
		DrawRectGraphF(x,y, ptr * MUSYA_X_SIZE, 2 * MUSYA_Y_SIZE - 37,
			MUSYA_X_SIZE, MUSYA_Y_SIZE, image[MUSYA_PTR_01], TRUE, TRUE);
	}
}
//-----------------------------------------------------------
// ����`�悷�邩��ݒ�
//-----------------------------------------------------------
int Musya::SetDraw()
{
	if (CheckHitKey(KEY_INPUT_LSHIFT)) {
		return MUSYA_DASH;
	}

	return MUSYA_NORMAL;
}
