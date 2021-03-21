#include "Fiona.h"
#include "WinMain.h"

//-----------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------
Fiona::Fiona()
{
	
}
//-----------------------------------------------------------
// �����t���R���X�g���N�^
//-----------------------------------------------------------
Fiona::Fiona(float x_, float y_,bool dir_,float draw_x_)
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
Fiona::~Fiona()
{
	Exit();
}

//-----------------------------------------------------------
//�@���������� 
//-----------------------------------------------------------
void Fiona::Init()
{
	ReloadGraph();

	ptr = 0;
	speed = FIONA_SPEED;
	gspd = FIONA_G;
	jspd = FIONA_JUMP_POWER;
}
//-----------------------------------------------------------
// �I������
//-----------------------------------------------------------
void Fiona::Exit()
{
	for (int i = 0; i < FIONA_PTR_MAX; i++) {
		DeleteGraph(image[i]);
	}
}
//-----------------------------------------------------------
// �X�V����
//-----------------------------------------------------------
void Fiona::Update()
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
void Fiona::Draw() {
	switch (SetDraw())
	{
	case FIONA_NORMAL:
		NormalDraw();
		break;
	case FIONA_DASH:
		DashDraw();
		break;
	default:
		break;
	}
	//printfDx("speed fiona %f", speed);
	//printfDx("draw_x fiona %f", draw_x);

}//-----------------------------------------------------------
// �摜�̓ǂݍ��݊֐�
//-----------------------------------------------------------
void Fiona::ReloadGraph()
{
	image[FIONA_PTR_01] = LoadGraph("data/player/fiona/fiona01.png");
	image[FIONA_PTR_02] = LoadGraph("data/player/fiona/fiona02.png");
	image[FIONA_PTR_03] = LoadGraph("data/player/fiona/fiona03.png");
	image[FIONA_PTR_04] = LoadGraph("data/player/fiona/fiona04.png");
}


//-----------------------------------------------------------
// �_�b�V���`��
//-----------------------------------------------------------
void Fiona::DashDraw()
{
	if (dir) {
		DrawRectGraphF(x, y, (ptr + 3)* FIONA_X_SIZE, 2 * FIONA_Y_SIZE - 37,
			FIONA_X_SIZE, FIONA_Y_SIZE, image[FIONA_PTR_01], TRUE, FALSE);
	}
	else {
		DrawRectGraphF(x, y, (ptr + 3)* FIONA_X_SIZE, 2 * FIONA_Y_SIZE - 37,
			FIONA_X_SIZE, FIONA_Y_SIZE, image[FIONA_PTR_01], TRUE, TRUE);
	}
}
//-----------------------------------------------------------
// �ʏ펞�`��
//-----------------------------------------------------------
void Fiona::NormalDraw()
{
	if (dir) {
		DrawRectGraphF(x,y, ptr * FIONA_X_SIZE, 2 * FIONA_Y_SIZE - 37,
			FIONA_X_SIZE, FIONA_Y_SIZE, image[FIONA_PTR_01], TRUE, FALSE);
	}
	else {
		DrawRectGraphF(x, y, ptr * FIONA_X_SIZE, 2 * FIONA_Y_SIZE - 37,
			FIONA_X_SIZE, FIONA_Y_SIZE, image[FIONA_PTR_01], TRUE, TRUE);
	}
}
//-----------------------------------------------------------
// ����`�悷�邩��ݒ�
//-----------------------------------------------------------
int Fiona::SetDraw()
{
	if ( CheckHitKey(KEY_INPUT_LSHIFT)) {
		return FIONA_DASH;
	}

	return FIONA_NORMAL;
}


