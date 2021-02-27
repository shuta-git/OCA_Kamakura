//---------------------------------------------
// 定数の宣言
//---------------------------------------------
const float MIN_CAMERA_POS_Y = 0.6f;	// カメラのポジションYの最小値
const float MAX_LENGTH = -15.0f;		// lengthの最大値
const float MIN_LENGTH = -2.0f;			// lengthの最小値
const float TARGET_POS_Y_ADD = 5.0f;	// ターゲットのポジションからY軸にプラスする値

//---------------------------------------------
// コンストラクタ
//---------------------------------------------
Camera::Camera()
{
	// 行列の初期化
	MatrixInit();
	// マウス座標の取得
	GetMousePoint(&mausex, &mausey);
	// 回転用
	rot.Set(0.0f, 0.0f, 0.0f);
	// カメラポジション
	pos.Set(0.0f, 0.0f, 0.0f);
	// ターゲットポジション
	look_pos.Set(0.0f, 0.0f, 0.0f);

	// マウスの回転用の変数初期化
	Mause_VRot = 0.0f;
	Mause_HRot = 0.0f;
	Old_Mause_VRot = 0.0f;
	Old_Mause_HRot = 0.0f;

	// ターゲットからカメラの距離を指定
	length = MAX_LENGTH;
}
//---------------------------------------------
// デストラクタ
//---------------------------------------------
Camera::~Camera()
{
}
//---------------------------------------------
// 更新
//---------------------------------------------
void Camera::Update()
{
	// Oldの変数を代入
	OldSet();

	// マウス座標の取得
	GetMousePoint(&mausex, &mausey);


	if (GetPadInput().ThumbRX == 32767) {
		Mause_HRot+= (DX_PI_F / 45.0f);
	}
	if (GetPadInput().ThumbRX == -32768) {
		Mause_HRot -= (DX_PI_F / 45.0f);
	}
	if (GetPadInput().ThumbRY == 32767) {
		Mause_VRot -= (DX_PI_F / 45.0f);
	}
	if (GetPadInput().ThumbRY == -32768) {
		Mause_VRot += (DX_PI_F / 45.0f);
	}


	// マウス座標が中心からどれだけ動いたか取得
	// マウスXの値
	Mause_HRot += (mausex - WINDOW_W / 2) / 1000.0f;
	// マウスYの値
	Mause_VRot += (mausey - WINDOW_H / 2) / 1000.0f;

	// マウス座標を中心に修正
	SetMousePoint(WINDOW_W / 2, WINDOW_H / 2);


	// カメラ座標にターゲット座標を設定
	pos.Set(look_pos);

	// カメラのリセット
	if (IsKeyOn(KEY_INPUT_R) || IsPadRepeat(PAD_L)) {
		Mause_HRot = player_rot.y;
	}

	// マウスの動ける範囲を指定
	MauseMoveRange();
	
	// Yを軸に回転
	rot.y = Mause_HRot;
	// Xを軸に回転
	rot.x = Mause_VRot;

	
	// カメラの移動
	pos.x += length * sinf(rot.y);
	pos.z += length * cosf(rot.y);
	pos.y -= length * sinf(rot.x);

	// カメラのポジションYが 0.6f よ小さくなったら
	if (pos.y <= MIN_CAMERA_POS_Y) {
		Mause_VRot = Old_Mause_VRot;
		pos.y = MIN_CAMERA_POS_Y;
		old_pos.y = pos.y;
	}


}
//---------------------------------------------
// 描画
//---------------------------------------------
void Camera::Draw()
{
	// カメラを設定
	SetCameraPositionAndTarget_UpVecY(pos.GetV(), look_pos.GetV());

#if(NDEBUG)
	//printfDx("Mause_HRot  %f \n" , Mause_HRot);
	//printfDx("Mause_VRot  %f \n", Mause_VRot);
	//printfDx("pos.y  %f \n", pos.y);
	//printfDx("length %f \n", length);
#endif
}
//---------------------------------------------
// ターゲット設定
//---------------------------------------------
void Camera::SetTarget(Vector3 target_pos)
{
	look_pos.Set(target_pos.x,target_pos.y + TARGET_POS_Y_ADD,target_pos.z);
}
//---------------------------------------------
// Y軸の値を設定
//---------------------------------------------
void Camera::SetRot(Vector3 rot_)
{
	player_rot = rot_;
}
//---------------------------------------------
// マウスの動ける範囲を指定
//---------------------------------------------
void Camera::MauseMoveRange()
{
	if (Mause_VRot > (DX_PI_F / 2) || Mause_VRot < -(DX_PI_F / 2)) {
		Mause_VRot = Old_Mause_VRot;
	}
	if (Mause_VRot > (DX_PI_F * 2) || Mause_VRot < -(DX_PI_F * 2)) {
		Mause_VRot = 0.0f;
	}
}
//---------------------------------------------
// Oldの変数を代入
// old_pos,Old_Mause_VRot,Old_Mause_HRot
//---------------------------------------------
void Camera::OldSet()
{
	// 前のフレーム座標を保存
	old_pos = pos;
	old_length = length;
	Old_Mause_VRot = Mause_VRot;
	Old_Mause_HRot = Mause_HRot;
}
