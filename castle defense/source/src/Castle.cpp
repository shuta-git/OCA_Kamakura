#include "Castle.h"
//---------------------------------------------
// 定数の宣言
//---------------------------------------------
const int	MAX_HP = 450;	// 城の最大HP

//---------------------------------------------
//　コンストラクタ
//---------------------------------------------
Castle::Castle()
{
	SetModelHandle("Castle.mv1");
	
	SetTextureHandle("Castle Exterior Texture.jpg");
	SetTextureHandle("Towers Doors and Windows Texture.jpg");
	SetTextureHandle("Ground and Fountain Texture.jpg");
	SetTextureHandle("Castle Interior Texture.jpg");

	ApplyTexture();

	pos = { 0.0f,-1.5f,0.0f };
	scale = { 5.0f,5.0f,5.0f };

	castle_pos1 = { -10.5f,0.0f,30.0f };
	castle_pos2 = { 7.0f,0.0f,30.0f };

	hp = MAX_HP;
	old_hp = MAX_HP;

	SetValue();
}
//---------------------------------------------
// デストラクタ
//---------------------------------------------
Castle::~Castle()
{
}
//---------------------------------------------
// 更新
//---------------------------------------------
void Castle::Update()
{
	if (old_hp != hp) {
		old_hp--;
	}
}
//---------------------------------------------
// 描画
//---------------------------------------------
void Castle::Draw()
{
	// モデル描画
	for (int i = 0; i < model_handle.size(); i++) {
		MV1DrawModel(model_handle[i]);
	}



#if(NDEBUG)
	DrawLine3D(castle_pos1, castle_pos2,SKYBLUE);
#endif
}
//---------------------------------------------
// 最大HP
//---------------------------------------------
int Castle::GetMaxHp() const
{
	return MAX_HP;
}
