#include "Map.h"

//----------------------------------------------------------------------
// デストラクタ
//----------------------------------------------------------------------
Map::~Map()
{
}
//----------------------------------------------------------------------
// ブロックの処理
//----------------------------------------------------------------------
void Map::HitCheckPB(Block & block)
{
	//-------------------
	// 当たり判定
	//-------------------
	// 下
	if (Circle(player->GetPosX(), player->GetPosY() - player->GetRot(), 1).intersects(block.GetRect())) {
		player->InitM_Pos();
		player->CahegFlag();
	}
}
//----------------------------------------------------------------------
// トラップの処理
//----------------------------------------------------------------------
void Map::HitCheckPT(Trap & trap)
{
	t_drop_pos = trap.GetDropTrapPos();
	//-------------------
	// 当たり判定
	//-------------------
	// トラップと当たったらplayerが死亡
	if (GetPlayer().intersects(trap.GetTriangle())) {
		p_death = true;
	}
	// 飛び出るトラップの範囲
	if (GetPlayer().intersects(trap.GetRangeTRIANGLE_OUT())) {
		is_tout_range = true;
	}
	//　ドロップトラップの範囲
	if (GetPlayer().intersects(trap.GetRangeTRIANGLE_DROP())) {
		trap.Is_DropChange();
	}
	// ドロップトラップの上辺
	if (Circle(player->GetPosX(), player->GetPosY() - player->GetRot(), 1).intersects(trap.GetOnTriagle_Drop())) {
		player->InitM_Pos();
		player->CahegFlag();
	}
}
//----------------------------------------------------------------------
// is_tout_rangeを設定(TRIANGLE_OUT)
//----------------------------------------------------------------------
void Map::SetTOutRange()
{
	if (is_tout_range) {
		is_tout_range = false;
	}
}
//----------------------------------------------------------------------
// 更新処理
//----------------------------------------------------------------------
void Map::Update()
{
#if ! defined(NDEBUG)
	// Rキーを押したら復活
	if (Input::KeyR.clicked || controller.buttonY.clicked && p_death) {
		player->Init();
		p_death = false;
	}
#else
#endif

	// 物理演算を更新
	world.update();

	if (player->GetPosY() < 0) {
		p_death = true;
	}

	if (p_death == false) {
		// プレイヤーの更新
		player->Update();
	}
}
//----------------------------------------------------------------------
// 描画処理
//----------------------------------------------------------------------
void Map::Draw()
{
	
		// プレイヤーの描画
		player->Draw();
	
	
}

