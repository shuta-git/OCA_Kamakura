#pragma once

class Vector3
{
public:

	float x, y, z;							// 変数

	//===================
	// コンストラクタ
	//===================
	Vector3();
	Vector3(float x_,float y_ ,float z_);
	Vector3(VECTOR v_);
	//Vector3(const Vector3& v_);

	//===================
	// デストラクタ
	//===================
	~Vector3();


	// ゼロ初期化
	void Clear();

	//===================
	// Get関数
	//===================
	float GetX() const { return x; }
    float GetY() const { return y; }
	float GetZ() const { return z; }
	// VECTOR型で座標を取得
	VECTOR GetV() const;

	//===================
	// Set関数
	//===================
	
	void SetX(float x_) { x = x_; }
	void SetY(float y_) { y = y_; }
	void SetZ(float z_) { z = z_; }
	// VECTOR型で座標設定
	void Set(VECTOR v_);
	// 座標設定
	void Set(float x_, float y_, float z_);
	// Vector を設定
	void Set(Vector3 &v_);
	// 加算・減算
	void Translate(float x_, float y_, float z_);
	// VECTOR型で加算・減算
	void TranslateV(VECTOR v_);
	// 加算
	void add(VECTOR v);
	// 加算
	void add(VECTOR v0, VECTOR v1);
	// 減算
	void subtract(VECTOR v);
	// 減算
	void subtract(VECTOR v0, VECTOR v1);
	// 掛け算
	void scale(float scale);
	// 加算
	Vector3 operator+(const Vector3& v)const;
	// 減算
	Vector3 operator-(const Vector3& v)const;
	// スケーリング*
	Vector3 operator*(float scale)const;
	// スケーリング/
	Vector3 operator/(float scale)const;
	// 長さの2乗
	float LengthSq()const;
	// ベクトルの長さ
	float Length()const;
	// 長さの設定
	void Length(float len);
	// 正規化
	Vector3 normalize()const;
	// 内積
	float dot(VECTOR a, VECTOR b);
	// 内積
	float dot(Vector3 a, Vector3 b);
	// 外積
	Vector3 cross(VECTOR a, VECTOR b);
	// 二点間の距離を取得
	float GetSqrtf(Vector3 a , Vector3 b) const;
	// 二点間の角度を取得
	float GetRadian(Vector3 a, Vector3 b)const;


};

