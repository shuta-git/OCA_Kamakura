// キー入力管理

#define MAX_KEY_NUM 256

unsigned char gKey[MAX_KEY_NUM];

bool IsOverKeyNum(int key_id);	// 配列の検証用


// 初期化
void KeyInputInit()
{
	for (int i = 0; i < MAX_KEY_NUM; ++i) {
		gKey[i] = 0;
	}
}
// 更新
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
// 終了
void KeyInputExit()
{
	;
}

// 特定キーの1回だけ押下検証
bool IsKeyOn(int key_id)
{
	if (IsOverKeyNum(key_id)) return false;
	return (gKey[key_id] == 1);
}
// 特定キーの解放検証
bool IsKeyRelease(int key_id)
{
	if (IsOverKeyNum(key_id)) return false;
	return (gKey[key_id] == 0);
}
// 特定キーの長押し検証
bool IsKeyRepeat(int key_id)
{
	if (IsOverKeyNum(key_id)) return false;
	return (gKey[key_id] > 1);
}
// gKey配列検証用
bool IsOverKeyNum(int key_id)
{
	return (key_id >= MAX_KEY_NUM) || (key_id < 0);
}