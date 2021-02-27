#include "Vector3.h"

//---------------------------------------------
// コンストラクタ
//---------------------------------------------
Vector3::Vector3()
{
}
Vector3::Vector3(float x_, float y_, float z_)
{
	x = x_;
	y = y_;
	z = z_;
}
Vector3::Vector3(VECTOR v_) 
{
	x = v_.x;
	y = v_.y;
	z = v_.z;
}
//Vector3::Vector3(const Vector3 & v_)
//{
//	x = v_.x;
//	y = v_.y;
//	z = v_.z;
//}
//---------------------------------------------
// デストラクタ
//---------------------------------------------
Vector3::~Vector3()
{
}
//---------------------------------------------
// VECTOR型で座標を取得
//---------------------------------------------
VECTOR Vector3::GetV() const{
	VECTOR v;
	v.x = x;
	v.y = y;
	v.z = z;

	return v;
}
//---------------------------------------------
// ゼロ初期化
//---------------------------------------------
void Vector3::Clear()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}
//---------------------------------------------
// 座標設定
//---------------------------------------------
void Vector3::Set(float x_, float y_, float z_)
{
	x = x_;
	y = y_;
	z = z_;
}
//---------------------------------------------
// Vectorで座標設定
//---------------------------------------------
void Vector3::Set(Vector3& v_)
{
	x = v_.x;
	y = v_.y;
	z = v_.z;
}
//---------------------------------------------
// 加算・減算
//---------------------------------------------
void Vector3::Translate(float x_, float y_, float z_)
{
	x += x_;
	y += y_;
	z += z_;
}
//---------------------------------------------
// 加算
//---------------------------------------------
void Vector3::add(VECTOR  v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}
//---------------------------------------------
// 加算
//---------------------------------------------
void Vector3::add(VECTOR  v0, VECTOR  v1)
{
	x = v0.x + v1.x;
	y = v0.y + v1.y;
	z = v0.z + v1.z;
}
//---------------------------------------------
// 減算
//---------------------------------------------
void Vector3::subtract(VECTOR  v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}
//---------------------------------------------
// 減算
//---------------------------------------------
void Vector3::subtract(VECTOR  v0, VECTOR  v1)
{
	x = v0.x - v1.x;
	y = v0.y - v1.y;
	z = v0.z - v1.z;
}
//---------------------------------------------
// 掛け算
//---------------------------------------------
void Vector3::scale(float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
}
//---------------------------------------------
// 加算
//---------------------------------------------
Vector3 Vector3::operator+(const Vector3 & v) const
{
	Vector3 result;
	result.x = x + v.x;
	result.y = y + v.y;
	result.z = z + v.z;

	return result;
}
//---------------------------------------------
// 減算
//---------------------------------------------
Vector3 Vector3::operator-(const Vector3 & v) const
{
	Vector3 result;
	result.x = x - v.x;
	result.y = y - v.y;
	result.z = z - v.z;

	return result;
}
//---------------------------------------------
// スケーリング*
//---------------------------------------------
Vector3 Vector3::operator*(float scale) const
{
	Vector3 result;
	result.x = x * scale;
	result.y = y * scale;
	result.z = z * scale;
	return result;
}
//---------------------------------------------
// スケーリング/
//---------------------------------------------
Vector3 Vector3::operator/(float scale) const
{
	Vector3 result;
	float invScale = 1.0f / scale;
	result.x = x * invScale;
	result.y = y * invScale;
	result.z = z * invScale;

	return result;
}
//---------------------------------------------
// 長さの2乗
//---------------------------------------------
float Vector3::LengthSq() const
{
	return x * x + y * y + z * z;
}
//---------------------------------------------
// ベクトルの長さ
//---------------------------------------------
float Vector3::Length() const
{
	float L = LengthSq(); // 長さの2乗を求める
	if (L == 0.0f) {
		return 0.0f;
	}
	return sqrtf(L);
}
//---------------------------------------------
// 長さの設定
//---------------------------------------------
void Vector3::Length(float len)
{
	normalize();
	x = x * len;
	y = y * len;
	z = z * len;
}
//---------------------------------------------
// 正規化
//---------------------------------------------
Vector3 Vector3::normalize() const
{
	float L = LengthSq();	// 長さの2乗を求める
	if (L == 0.0f) {
		return *this;
	}
	return *this / sqrtf(L);
}
//---------------------------------------------
// 内積
//---------------------------------------------
float Vector3::dot(VECTOR a, VECTOR b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}
//---------------------------------------------
// 内積
//---------------------------------------------
float Vector3::dot(Vector3 a, Vector3 b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}
//---------------------------------------------
// 外積
//---------------------------------------------
Vector3 Vector3::cross(VECTOR a, VECTOR b)
{
	Vector3 result;
	result.x = (a.y * b.z) - (a.z * b.y);
	result.y = (a.z * b.x) - (a.x * b.z);
	result.z = (a.x * b.y) - (a.y * b.x);

	return result;
}
//---------------------------------------------
// 二点間の距離を取得
//---------------------------------------------
float Vector3::GetSqrtf(Vector3 a, Vector3 b) const
{
	float result;
	
	result = sqrtf((a.x - b.x) * (a.x - b.x) + (a.z - b.z) * (a.z - b.z));

	return result;
}
//---------------------------------------------
// 二点間の角度を取得
//---------------------------------------------
float Vector3::GetRadian(Vector3 a, Vector3 b) const
{
	float result;

	result = atan2(b.z - a.z,b.x - x);

	result = result * 180.0f / DX_PI_F;

	return result;
}
//---------------------------------------------
// VECTOR型で座標設定
//---------------------------------------------
void Vector3::Set(VECTOR v_)
{
	x = v_.x;
	y = v_.y;
	z = v_.z;
}
//---------------------------------------------
// VECTOR型で加算・減算
//---------------------------------------------
void Vector3::TranslateV(VECTOR v_)
{
	x += v_.x;
	y += v_.y;
	z += v_.z;
}
