// �L�[���͊Ǘ�

#define MAX_KEY_NUM 256

unsigned char gKey[MAX_KEY_NUM];

bool IsOverKeyNum(int key_id);	// �z��̌��ؗp


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
	unsigned char tmp_key[MAX_KEY_NUM];
	GetHitKeyStateAll((char*)tmp_key);

	for (int i = 0; i < MAX_KEY_NUM; ++i) {
		if (tmp_key[i] != 0) {
			gKey[i]++;
		}
		else {
			gKey[i] = 0;
		}
	}
	return 0;
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
bool IsKeyRelease(int key_id)
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