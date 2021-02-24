#include "MouseInput.h"
// �}�E�X���͊Ǘ�

#define MAX_MOUSE_BUTTON	8

static int mouse_x;
static int mouse_y;

static int gMouse[MAX_MOUSE_BUTTON];

static bool IsOverMouseNum(int mouse_id);


// ������
void MouseInputInit()
{
	mouse_x = 0;
	mouse_y = 0;

	for (int i = 0; i < MAX_MOUSE_BUTTON; ++i) {
		gMouse[i] = 0;
	}
}

// �X�V
void MouseInputUpdate()
{
	int mouse_state = GetMouseInput();
	GetMousePoint(&mouse_x, &mouse_y);

	for (int i = 0; i < MAX_MOUSE_BUTTON; ++i) {
		if (mouse_state - 1 == i) {
			++gMouse[i];
			if (gMouse[i] >= INT_MAX)	gMouse[i] = INT_MAX;
		}
		else {
			gMouse[i] = 0;
		}
	}
}

// �I��
void MouseInputExit()
{
	;
}

// 1�񂾂�
bool IsMouseOn(int mouse_id)
{
	int tmp_mouse_id = mouse_id - 1;
	if (IsOverMouseNum(tmp_mouse_id))	return false;
	return (gMouse[tmp_mouse_id] == 1);
}

// �����ꂽ
bool IsMouseRelease(int mouse_id)
{
	int tmp_mouse_id = mouse_id - 1;
	if (IsOverMouseNum(tmp_mouse_id))	return false;
	return (gMouse[tmp_mouse_id] == 0);

}

// ������
bool IsMouseRepeate(int mouse_id)
{
	int tmp_mouse_id = mouse_id - 1;
	if (IsOverMouseNum(tmp_mouse_id))	return false;
	return (gMouse[tmp_mouse_id] > 1);
}

// �}�E�X��X���W �擾
int GetMouseX()
{
	return mouse_x;
}

// �}�E�X��Y���W �K��
int GetMouseY()
{
	return mouse_y;
}

bool CheckMouseInput(void)
{
	if (GetMouseInput() & MOUSE_INPUT_LEFT) {
		return true;
	}
	return false;
}

// �}�E�X�̔z�񌟏ؗp
bool IsOverMouseNum(int mouse_id)
{
	return (mouse_id >= MAX_MOUSE_BUTTON) || (mouse_id < 0);
}