#pragma once

//---------------------------------------------
// クラス
//---------------------------------------------
class Camera : public Base
{
public:
	Camera();
	~Camera();

	// 更新
	void Update() override;	
	// 描画
	void Draw() override;						

	//=======================================
	// Set関数
	//=======================================

	// ターゲットの設定
	void SetTarget(Vector3 target_pos);	
	// Y軸の値を設定
	void SetRot(Vector3 rot_);

	//=======================================
	// Get関数
	//=======================================

	// ポジションの取得
	Vector3 GetPos()const { return pos; }		
	// 向きの取得
	Vector3 GetRot()const { return rot; }		
	// カメラの行列を取得
	MATRIX GetMat()const { return mat; }		

private:

	// マウスの動ける範囲を指定
	void MauseMoveRange();			
	// Oldの変数を代入
	void OldSet();								

	//=======================================
	// 変数
	//=======================================
	int mausex, mausey;			// マウス座標

	Vector3 look_pos;			// ターゲット座標
	Vector3 player_rot;			// プレイヤーの角度

	float Camera_VRot;			// カメラ垂直方向用
	float Camera_HRot;			// カメラ水平方向用
	float Camera_TRot;			// カメラ捻り方向用

	float move_camera = 2.0f;	// カメラの速度

	float Mause_VRot;			// マウスの垂直方向
	float Mause_HRot;			// マウスに水平方向
	float Old_Mause_VRot;		// １フレーム前の値を保存する変数
	float Old_Mause_HRot;		// １フレーム前の値を保存する変数

	float length;				// プレイヤーからカメラの距離
	float old_length;			// １フレーム前の値を保存
};

