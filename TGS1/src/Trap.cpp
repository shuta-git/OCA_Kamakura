#include "Trap.h"


//----------------------------------------------------------------------
// デストラクタ 
//----------------------------------------------------------------------
Trap::~Trap()
{
}
//----------------------------------------------------------------------
// 更新処理
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
// 描画処理
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
	// グラウンドの描画
	ground.draw();
}
//----------------------------------------------------------------------
// 三角形を取得
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
// TRIANGLE_OUTの範囲の当たり判定
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
// TRIANGLE_DROPの範囲の当たり判定
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
// ドロップトラップの頂点に当たり判定をつける
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
// ドロップトラップの上の辺の当たり判定を取得
//----------------------------------------------------------------------
Triangle Trap::GetOnTriagle_Drop() const
{
	return Triangle(t_pos.x + MAP_SIZE / 2, t_pos.y + MAP_SIZE - 10 + t_speed.y,
		t_pos.x + MAP_SIZE + 1, t_pos.y + MAP_SIZE + 1 + t_speed.y,
		t_pos.x - 1, t_pos.y + MAP_SIZE + 1 + t_speed.y);
}
//----------------------------------------------------------------------
// ブロックを消す関数
//----------------------------------------------------------------------
void Trap::SetDestroy()
{
	//消去
	this->Destroy();
}
//----------------------------------------------------------------------
// TRIANGLE_DROPのt_pos.yをマイナスに下げていく
//----------------------------------------------------------------------
void Trap::DropTriangle()
{
	if (is_droptrap) {
		t_speed.y -= TRAP_DROP_SPEED;
		t_drop_pos.y -= TRAP_DROP_SPEED;
	}
}
//----------------------------------------------------------------------
// ドロップトラップのあたり判定をTRUEにする
//----------------------------------------------------------------------
void Trap::Is_DropChange()
{
	if (is_droptrap == false) {
		is_droptrap = true;
		is_droptrap_block = true;
	}
}
//----------------------------------------------------------------------
// ブロックとトラップの当たり判定
//----------------------------------------------------------------------
void Trap::HitCheckBT(Block & block)
{
	if (GetTriangle_Drop().intersects(block.GetRect())) {
		is_droptrap = false;
	}
}
