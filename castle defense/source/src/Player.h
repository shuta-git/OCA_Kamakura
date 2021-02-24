#pragma once

//---------------------------------------------
// Playerクラス　継承Base
//---------------------------------------------
class Player : public CollisionBase
{
public:
	Player();
	~Player();

	// 更新
	void Update() override;
	// 描画
	void Draw() override;				

	//=======================================
	// Get関数
	//=======================================
	// 剣の先端の座標を取得
	VECTOR GetSwordTipPos()const;
	// 剣の持ち手の座標を取得
	VECTOR GetSwordRootPos()const;
	// 特殊攻撃の座標
	VECTOR GetSpacialPos() const { return sp_pos.GetV(); }
	// 攻撃フラグを取得
	bool GetAttack() const { return attack; }	
	// 特殊攻撃フラグを取得
	bool GetSpacialAttack()const { return spacial_attack; }
	// 特殊攻撃時間の取得
	int GetSpacialTimer() const { return sp_timer; }
	// 特殊攻撃の最大時間の取得
	int GetSpacialMaxTimer() const;
	
	//=======================================
	// Set関数
	//=======================================
	// 特殊攻撃フラグをfalseにする
	void SetFalseSpacialAttack() { spacial_attack = false; }
	// カメラ回転用を設定
	void SetCameraRot(Vector3 rot_);				
	// MPをプラス
	void AddMp(int num);
	// MPを減らす
	void SubMp(int num);

private:

	// カメラの全体行列
	MATRIX camera_mat;

	//=======================================
	// 変数
	//=======================================
	int old_dir;		// 一つ前の方向
	float old_rot_y;	// rot.y を保存しておく
	int dir;			// 方向用
	int attack_time;	// 攻撃時間
	float rot_speed;	// 回転速度
	int mp;				// 魔法発動用
	int old_mp;			// mp保存用
	bool now_attack;	// 攻撃中か
	bool spacial_attack;// 特殊攻撃用
	int stamina;		// スタミナ
	bool stamina_out;	// スタミナ切れ
	int sp_timer;		// 特殊攻撃時間用

	Vector3 sp_pos;					// 特殊攻撃用の座標
	Vector3 camera_rot;				// カメラ回転用
	std::vector<int>texture_handle;	// テクスチャーハンドル

	//  ポイントライト用
	float Range, Atten0, Atten1, Atten2;

	//=======================================
	// 関数
	//=======================================
	void Init();			// 初期化
	void SetMatRot();		// 回転行列に値を設定
	void Move();			// 移動
	void MoveRot();			// 回転
	void AttackAction();	// 攻撃アクション

	// カメラの回転範囲の処理
	void CameraDirRotRange(float p_rot_range);	
	// カメラの向き範囲を設定
	void DirSetRotRange();										
	// 向き範囲を設定
	void SetRotRange(float d1,float d2,float d3,float d4);	
	// マイナスに回るかプラスに回るか判定		
	void SetRotRange(float d1, float d2, float d3, float d4,	
						float d5,float d6,float d7,float d8);	
	// 回転スピード
	void RotSpeed(int dir_,int old_,float DIR);		
	// ゲージ
	void SetDrawGeuge(int size_x,int size_y,int x,int y, int type, int old_type , unsigned int color);
	// 攻撃モーション時間の更新
	void AttackAnimTimeUpdate(int attack_type,int finish_time);
	// ゲージの描画
	void DrawGeuge();
	// ゲージの更新処理
	void GeugeUpdate();

	// ポイントライトを動かす更新処理
	void PointLightUpdate();

	//=======================================
	// テクスチャー
	//=======================================
	// テクスチャーのパスを取得
	void SetTextureHandle(string str);
	// テクスチャーを適用
	void ApplyTexture();

	//=======================================
	// 列挙体
	//=======================================

	// 方向の種類
	enum Dir {								
		RIGHT = 0,
		LEFT,
		UP,
		DOWN,
		RIGHT_UP,
		RIGHT_DOWN,
		LEFT_UP,
		LEFT_DOWN,

		DIR_MAX,
	};

	enum Anim {
		ATTACK = 0,
		MAGIC,

		ANIM_MAX,
	};
};

