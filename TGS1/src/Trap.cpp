#include "Trap.h"


//----------------------------------------------------------------------
// �f�X�g���N�^ 
//----------------------------------------------------------------------
Trap::~Trap()
{
}
//----------------------------------------------------------------------
// �X�V����
//----------------------------------------------------------------------
void Trap::Update()
{
	//t_drop_pos = ground.getPos();
	switch (type)
	{
	case TRIANGLE_DROP:
		DropTriangle();
		ground.setPos(t_drop_pos);
		break;
	default:
		break;
	}
}
//----------------------------------------------------------------------
// �`�揈��
//----------------------------------------------------------------------
void Trap::Draw()
{
#if ! defined(NDEBUG)
	switch (type)
	{
	case TRIANGLE_OUT:
		Circle(t_pos.x + MAP_SIZE / 2, t_pos.y + MAP_SIZE / 2, MAP_SIZE + 20).drawFrame();
		break;
	case TRIANGLE_DROP:
		Triangle(t_pos.x + MAP_SIZE / 2, t_pos.y - MAP_SIZE * 22 , t_pos.x + MAP_SIZE + MAP_SIZE/2, t_pos.y + MAP_SIZE , t_pos.x - MAP_SIZE / 2, t_pos.y + MAP_SIZE ).drawFrame();
		break;
	default:
		break;
	}

#else
#endif
	// �O���E���h�̕`��
	ground.draw();
}
//----------------------------------------------------------------------
// �O�p�`���擾
//----------------------------------------------------------------------
Triangle Trap::GetTriangle() const
{
	int size = 20;
	switch (type)
	{
	case TRIANGLE_UP:
		return Triangle(t_pos.x + MAP_SIZE / 2, t_pos.y + MAP_SIZE, t_pos.x, t_pos.y, t_pos.x + MAP_SIZE, t_pos.y); 		
		break;
	case TRIANGLE_DOWN:
		return Triangle(t_pos.x + MAP_SIZE / 2, t_pos.y, t_pos.x + MAP_SIZE, t_pos.y + MAP_SIZE, t_pos.x, t_pos.y + MAP_SIZE);
		break;
	case TRIANGLE_RIGHT:
		return Triangle(t_pos.x + MAP_SIZE, t_pos.y + MAP_SIZE / 2, t_pos.x, t_pos.y + MAP_SIZE, t_pos.x, t_pos.y);
		break;
	case TRIANGLE_LEFT:
		return Triangle(t_pos.x, t_pos.y + MAP_SIZE / 2, t_pos.x + MAP_SIZE, t_pos.y, t_pos.x + MAP_SIZE, t_pos.y + MAP_SIZE);
		break;
	case TRIANGLE_OUT:
		return Triangle(t_pos.x + MAP_SIZE / 2, t_pos.y + MAP_SIZE / 2, t_pos.x + 5, t_pos.y, t_pos.x + MAP_SIZE - 5, t_pos.y);
		break;
	case TRIANGLE_DROP:
		return Triangle(t_pos.x + MAP_SIZE / 2, t_pos.y - MAP_SIZE / 2 + t_speed.y , t_pos.x + MAP_SIZE - size, t_pos.y + MAP_SIZE  + t_speed.y - size, t_pos.x + size, t_pos.y + MAP_SIZE + t_speed.y - size);
		break;
	case TRIANGLE_OUT_1:
		return Triangle(t_pos.x + MAP_SIZE / 2, t_pos.y + MAP_SIZE + MAP_SIZE / 2, t_pos.x + 5, t_pos.y, t_pos.x + MAP_SIZE - 5, t_pos.y);
		break;
	default:
		break;
	}
	return Triangle();
}
//----------------------------------------------------------------------
// TRIANGLE_OUT�͈̔͂̓����蔻��
//----------------------------------------------------------------------
Circle Trap::GetRangeTRIANGLE_OUT() const
{
	
	switch (type)
	{
	case TRIANGLE_OUT:
		return Circle(t_pos.x + MAP_SIZE / 2, t_pos.y + MAP_SIZE / 2, MAP_SIZE + 20);
		break;
	default:
		return Circle();
		break;
	}
	
}
//----------------------------------------------------------------------
// TRIANGLE_DROP�͈̔͂̓����蔻��
//----------------------------------------------------------------------
Triangle Trap::GetRangeTRIANGLE_DROP() const
{
	switch (type)
	{
	case TRIANGLE_DROP:
		return Triangle(t_pos.x + MAP_SIZE / 2, t_pos.y - MAP_SIZE * 22, t_pos.x + MAP_SIZE + MAP_SIZE / 2, t_pos.y + MAP_SIZE, t_pos.x - MAP_SIZE / 2, t_pos.y + MAP_SIZE);
		break;
	default:
		return Triangle();
		break;
	}
}
//----------------------------------------------------------------------
// �h���b�v�g���b�v�̒��_�ɓ����蔻�������
//----------------------------------------------------------------------
Triangle Trap::GetTriangle_Drop() const
{
	switch (type)
	{
	case TRIANGLE_DROP:
		return Triangle(t_pos.x + MAP_SIZE / 2, t_pos.y - MAP_SIZE / 2 + t_speed.y, t_pos.x + MAP_SIZE, t_pos.y + MAP_SIZE + t_speed.y - 3 , t_pos.x, t_pos.y + MAP_SIZE + t_speed.y - 3);
		break;
	default:
		return Triangle();
		break;
	}
}
//----------------------------------------------------------------------
// �h���b�v�g���b�v�̏�̕ӂ̓����蔻����擾
//----------------------------------------------------------------------
Triangle Trap::GetOnTriagle_Drop() const
{
	return Triangle(t_pos.x + MAP_SIZE / 2, t_pos.y + MAP_SIZE - 10 + t_speed.y,
		t_pos.x + MAP_SIZE + 1, t_pos.y + MAP_SIZE + 1 + t_speed.y,
		t_pos.x - 1, t_pos.y + MAP_SIZE + 1 + t_speed.y);
}
//----------------------------------------------------------------------
// �u���b�N�������֐�
//----------------------------------------------------------------------
void Trap::SetDestroy()
{
	//����
	this->Destroy();
}
//----------------------------------------------------------------------
// TRIANGLE_DROP��t_pos.y���}�C�i�X�ɉ����Ă���
//----------------------------------------------------------------------
void Trap::DropTriangle()
{
	if (is_droptrap) {
		t_speed.y -= TRAP_DROP_SPEED;
		t_drop_pos.y -= TRAP_DROP_SPEED;
	}
}
//----------------------------------------------------------------------
// �h���b�v�g���b�v�̂����蔻���TRUE�ɂ���
//----------------------------------------------------------------------
void Trap::Is_DropChange()
{
	if (is_droptrap == false) {
		is_droptrap = true;
		is_droptrap_block = true;
	}
}
//----------------------------------------------------------------------
// �u���b�N�ƃg���b�v�̓����蔻��
//----------------------------------------------------------------------
void Trap::HitCheckBT(Block & block)
{
	if (GetTriangle_Drop().intersects(block.GetRect())) {
		is_droptrap = false;
	}
}
