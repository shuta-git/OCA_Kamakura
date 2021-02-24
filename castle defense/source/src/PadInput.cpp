#include "PadInput.h"
// �p�b�h���͊Ǘ�

#define MAX_PAD_NUM		14			// �㉺���E/A B C X Y Z L R START M�{�^��
#define MAX_PAD_TYPE	PAD_NO_MAX	// PAD�ڑ��ő吔

static unsigned int gPad[MAX_PAD_TYPE][MAX_PAD_NUM];
static bool IsOverPadNum(int pad_id);
static XINPUT_STATE pad;

const XINPUT_STATE GetPadInput()
{
	return pad;
}

// ������
void PadInputInit()
{
	for (int j = 0; j < MAX_PAD_TYPE; ++j) {
		for (int i = 0; i < MAX_PAD_NUM; ++i) {
			gPad[j][i] = 0;
		}
	}
}
// �X�V
void PadInputUpdate()
{

	GetJoypadXInputState(DX_INPUT_PAD1, &pad);

	static const int PAD_BUTTONS[MAX_PAD_NUM] = {
		PAD_INPUT_UP, PAD_INPUT_DOWN, PAD_INPUT_LEFT, PAD_INPUT_RIGHT,
		PAD_INPUT_A, PAD_INPUT_B, PAD_INPUT_C, PAD_INPUT_X, PAD_INPUT_Y,
		PAD_INPUT_Z, PAD_INPUT_L, PAD_INPUT_R, PAD_INPUT_START, PAD_INPUT_M
	};
	static const int PAD_TYPES[MAX_PAD_TYPE] = {
		DX_INPUT_PAD1, DX_INPUT_PAD2, DX_INPUT_PAD3, DX_INPUT_PAD4
	};

	for (int j = 0; j < MAX_PAD_TYPE; ++j) {
		int tmp_pad = GetJoypadInputState(PAD_TYPES[j]);
		for (int i = 0; i < MAX_PAD_NUM; ++i) {
			if (tmp_pad & PAD_BUTTONS[i]) {
				++gPad[j][i];
				if (gPad[j][i] > UINT_MAX)	gPad[j][i] = 1;
			}
			else {
				gPad[j][i] = 0;
			}
		}
	}


}
// �I��
void PadInputExit()
{
	;
}

// ����p�b�h�{�^����1�񂾂���������
bool IsPadOn(int pad_id, PAD_NO pad_no)
{
	if (IsOverPadNum(pad_id))	return false;
	return (gPad[pad_no][pad_id] == 1);
}
// ����p�b�h�{�^���̉������
bool IsPadRelease(int pad_id, PAD_NO pad_no)
{
	if (IsOverPadNum(pad_id))	return false;
	return (gPad[pad_no][pad_id] == 0);
}
// ����p�b�h�{�^���̒���������
bool IsPadRepeat(int pad_id, PAD_NO pad_no)
{
	if (IsOverPadNum(pad_id))	return false;
	return (gPad[pad_no][pad_id] > 1);
}
// gPad�z�񌟏ؗp
bool IsOverPadNum(int pad_id)
{
	return (pad_id >= MAX_PAD_NUM) || (pad_id < 0);
}