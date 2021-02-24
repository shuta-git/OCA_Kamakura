#pragma once

class Base 
{
public:
	Base();						// コンストラクタ
	~Base();					// デストラクタ

	virtual void Update() = 0;
	virtual void Draw() = 0;
	// 行列をすべて初期化
	void MatrixInit();	
	// 回転行列を初期化
	void RotMatrixInit();		
	// 座標行列を初期化
	void TransMatrixInit();		

	//=======================================
	// Get関数
	//=======================================

	// 座標を取得
	Vector3 GetPos() const { return pos; }	
	// 一つ前の座標を取得
	Vector3 GetOldPos() const { return old_pos; }
 	// 向きを取得
	Vector3 GetRot() const { return rot; }		
	// サイズを取得
	Vector3 GetScale() const { return scale; }	
	// 座標を取得
	VECTOR GetVPos() const { return pos.GetV(); }
	// 一つ前の座標を取得
	VECTOR GetOldVPos() const { return old_pos.GetV(); }
	// 向きを取得
	// 向きを取得
	VECTOR GetVRot()const { return rot.GetV(); }		
	// サイズを取得
	VECTOR GetVScale() const{ return scale.GetV(); }			
	// カプセルの上部分の座標を取得
	VECTOR GetCapsuleUp(VECTOR pos_,float size_) const;
	// カプセルの下部分の座標を取得
	VECTOR GetCapsuleDown(VECTOR pos_)const;
	// カプセルの先端部分の座標を取得
	VECTOR GetCapsuleTip(VECTOR pos_,float size_ = 0.0f)const;
	// カプセルの根本部分の座標を取得
	VECTOR GetCapsuleRoot(VECTOR pos_)const;


protected:

	Vector3 pos;				// Vectorクラス
	Vector3 old_pos;			// １フレーム前の座標
	Vector3 rot;				// 向き
	Vector3 scale;				// サイズ

	MATRIX mat;					// 行列
	MATRIX mat_trans;			// 座標行列
	MATRIX mat_rot;				// 回転行列

	float  radius;				// 半径
	float  speed;				// 速度
	
	std::vector<Effect*>effect;	// エフェクト

	// エフェクトを消す
	void DeleteEffect(int i);
	// エフェクトを止める
	void StopEffect(int i);

private:

	// effectのサイズを縮める
	void EraseEffect();
};

