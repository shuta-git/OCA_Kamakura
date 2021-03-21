// �L�[���͊Ǘ�
#include "KeyInput.h"

#define MAX_KEY_NUM 256

static char gKey[MAX_KEY_NUM];

static bool IsOverKeyNum(int key_id);	// �z��̌��ؗp



// ������
void KeyInputInit()
{
	for (int i = 0; i < MAX_KEY_NUM; ++i) {
		gKey[i] = 0;
	}
}
// �X�V
int KeyInputUpdate()
{
	char tmp_key[MAX_KEY_NUM];
	int ret = GetHitKeyStateAll(tmp_key);

	for (int i = 0; i < MAX_KEY_NUM; ++i) {
		if (tmp_key[i] != 0) {
			gKey[i]++;
		}
		else {
			gKey[i] = 0;
		}
	}
	return ret;
}
// �I��
void KeyInputExit()
{
	;
}

// ����L�[��1�񂾂���������
bool IsKeyOn(int key_id)
{
	if (IsOverKeyNum(key_id)) return false;
	return (gKey[key_id] == 1);
}
// ����L�[�̉������
bool IsKeyRelese(int key_id)
{
	if (IsOverKeyNum(key_id)) return false;
	return (gKey[key_id] == 0);
}
// ����L�[�̒���������
bool IsKeyRepeat(int key_id)
{
	if (IsOverKeyNum(key_id)) return false;
	return (gKey[key_id] > 1);
}
// gKey�z�񌟏ؗp
bool IsOverKeyNum(int key_id)
{
	return (key_id >= MAX_KEY_NUM) || (key_id < 0);
}
//---------------------------------------------------------------------------------
//	�}�E�X��������Ă��邩���擾����
//---------------------------------------------------------------------------------
bool CheckMouseInput(void)
{
	if (GetMouseInput() & MOUSE_INPUT_LEFT) {
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------------
//	�}�E�X�������ꂽ�u�Ԃ��擾����
//---------------------------------------------------------------------------------
//bool PushMouseInput(void)
//{
//	if (GetMouseInput() & MOUSE_INPUT_LEFT) {
//		if (MouseBefore == 0) {
//			return true;
//		}
//	}
//	return false;
//}
//---------------------------------------------------------------------------------
//	�}�E�X�̍��W���擾����
//---------------------------------------------------------------------------------
int GetMouseX(void)
{
	int mouse_x;
	int mouse_y;
	GetMousePoint(&mouse_x, &mouse_y);
	return mouse_x;
}
int GetMouseY(void)
{
	int mouse_x;
	int mouse_y;
	GetMousePoint(&mouse_x, &mouse_y);
	return mouse_y;
}


