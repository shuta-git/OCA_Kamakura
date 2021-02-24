#include "Player.h"

//----------------------------------------------------------------------
// コンストラクタ
//----------------------------------------------------------------------
Player::Player(PhysicsWorld w)
{
	p_pos = Vec2(PLAYER_POS_X, PLAYER_PO_Y);
	m_pos = Vec2(0, 0);
	p_rot = PLAYER_ROT;
	world = w;
	player = world.createCircle(Vec2(p_pos.x, p_pos.y), Circle(p_rot)),none,none, PhysicsBodyType::Static;
	
	jump_f = true;
}

//----------------------------------------------------------------------
// デストラクタ
//----------------------------------------------------------------------
Player::~Player()
{
}
//----------------------------------------------------------------------
// 初期化処理
//----------------------------------------------------------------------
void Player::Init()
{
	p_pos = Vec2(PLAYER_POS_X, PLAYER_PO_Y);
	m_pos = Vec2(0, 0);
	player = world.createCircle(Vec2(p_pos.x, p_pos.y), Circle(p_rot)), none, none, PhysicsBodyType::Static;
	jump_f = true;
}
//----------------------------------------------------------------------
// 更新処理
//----------------------------------------------------------------------
void Player::Update()
{
#if ! defined(NDEBUG)
#else
	
#endif

	// プレイヤーのポジションに座標を設定
	p_pos = player.getPos();

	// ジャンプ
	JumpCon();
	JumpKey();
	// 移動
	MoveCon();
	MoveKey();
	// 重力
	Gravity();

	// 常に移動数をプレイヤーの座標に足す
	p_pos.y += m_pos.y;
	p_pos.x += m_pos.x;
	
	// ボールの座標に設定
	player.setPos(p_pos);
}

//----------------------------------------------------------------------
// 描画処理
//----------------------------------------------------------------------
void Player::Draw()
{
	// プレイヤー本体を描画
	player.draw();

	//Println(p_pos.x);
}
//----------------------------------------------------------------------
// m_pos.yを初期化
//----------------------------------------------------------------------
void Player::InitM_Pos()
{
	m_pos.y = 0;
}
//----------------------------------------------------------------------
// ジャンプフラグを変える
//----------------------------------------------------------------------
void Player::CahegFlag()
{
	if (jump_f == true) {
		jump_f = false;
	}
}
//----------------------------------------------------------------------
// コントローラー
//----------------------------------------------------------------------
//-----------------------------------
// 移動
//-----------------------------------
void Player::MoveCon()
{
	m_pos.x = controller.leftThumbX * 7.0;

	if (controller.buttonRight.pressed) {
		m_pos.x = 6;
	}
	else if(controller.buttonLeft.pressed){
		m_pos.x = -6;
	}
}
//-----------------------------------
// ジャンプ
//-----------------------------------
void Player::JumpCon()
{
	if (controller.buttonA.clicked && jump_f == false) {
		m_pos.y += 10;
		jump_f = true;
	}
}
//----------------------------------------------------------------------
//　キーボード
//----------------------------------------------------------------------
//-----------------------------------
// 移動
//-----------------------------------
void Player::MoveKey()
{
	if (Input::KeyShift.pressed) {
		if (Input::KeyD.pressed) {
			m_pos.x = 6;
		}
		else if (Input::KeyA.pressed) {
			m_pos.x = -6;
		}
		
		

	}
	else {
		if (Input::KeyD.pressed) {
			m_pos.x = 3;
		}
		else if (Input::KeyA.pressed) {
			m_pos.x = -3;
		}
	}
}
//-----------------------------------
// ジャンプ
//-----------------------------------
void Player::JumpKey()
{
	if (Input::KeySpace.clicked && jump_f == false) {
		m_pos.y = 0;
		m_pos.y += 10;
		jump_f = true;
	}

}
//-----------------------------------
// 重力
//-----------------------------------
void Player::Gravity()
{
	m_pos.y -= 0.3;
}


