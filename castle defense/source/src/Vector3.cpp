#include "Vector3.h"

//---------------------------------------------
// �R���X�g���N�^
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
// �f�X�g���N�^
//---------------------------------------------
Vector3::~Vector3()
{
}
//---------------------------------------------
// VECTOR�^�ō��W���擾
//---------------------------------------------
VECTOR Vector3::GetV() const{
	VECTOR v;
	v.x = x;
	v.y = y;
	v.z = z;

	return v;
}
//---------------------------------------------
// �[��������
//---------------------------------------------
void Vector3::Clear()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}
//---------------------------------------------
// ���W�ݒ�
//---------------------------------------------
void Vector3::Set(float x_, float y_, float z_)
{
	x = x_;
	y = y_;
	z = z_;
}
//---------------------------------------------
// Vector�ō��W�ݒ�
//---------------------------------------------
void Vector3::Set(Vector3& v_)
{
	x = v_.x;
	y = v_.y;
	z = v_.z;
}
//---------------------------------------------
// ���Z�E���Z
//---------------------------------------------
void Vector3::Translate(float x_, float y_, float z_)
{
	x += x_;
	y += y_;
	z += z_;
}
//---------------------------------------------
// ���Z
//---------------------------------------------
void Vector3::add(VECTOR  v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}
//---------------------------------------------
// ���Z
//---------------------------------------------
void Vector3::add(VECTOR  v0, VECTOR  v1)
{
	x = v0.x + v1.x;
	y = v0.y + v1.y;
	z = v0.z + v1.z;
}
//---------------------------------------------
// ���Z
//---------------------------------------------
void Vector3::subtract(VECTOR  v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}
//---------------------------------------------
// ���Z
//---------------------------------------------
void Vector3::subtract(VECTOR  v0, VECTOR  v1)
{
	x = v0.x - v1.x;
	y = v0.y - v1.y;
	z = v0.z - v1.z;
}
//---------------------------------------------
// �|���Z
//---------------------------------------------
void Vector3::scale(float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
}
//---------------------------------------------
// ���Z
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
// ���Z
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
// �X�P�[�����O*
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
// �X�P�[�����O/
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
// ������2��
//---------------------------------------------
float Vector3::LengthSq() const
{
	return x * x + y * y + z * z;
}
//---------------------------------------------
// �x�N�g���̒���
//---------------------------------------------
float Vector3::Length() const
{
	float L = LengthSq(); // ������2������߂�
	if (L == 0.0f) {
		return 0.0f;
	}
	return sqrtf(L);
}
//---------------------------------------------
// �����̐ݒ�
//---------------------------------------------
void Vector3::Length(float len)
{
	normalize();
	x = x * len;
	y = y * len;
	z = z * len;
}
//---------------------------------------------
// ���K��
//---------------------------------------------
Vector3 Vector3::normalize() const
{
	float L = LengthSq();	// ������2������߂�
	if (L == 0.0f) {
		return *this;
	}
	return *this / sqrtf(L);
}
//---------------------------------------------
// ����
//---------------------------------------------
float Vector3::dot(VECTOR a, VECTOR b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}
//---------------------------------------------
// ����
//---------------------------------------------
float Vector3::dot(Vector3 a, Vector3 b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}
//---------------------------------------------
// �O��
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
// ��_�Ԃ̋������擾
//---------------------------------------------
float Vector3::GetSqrtf(Vector3 a, Vector3 b) const
{
	float result;
	
	result = sqrtf((a.x - b.x) * (a.x - b.x) + (a.z - b.z) * (a.z - b.z));

	return result;
}
//---------------------------------------------
// ��_�Ԃ̊p�x���擾
//---------------------------------------------
float Vector3::GetRadian(Vector3 a, Vector3 b) const
{
	float result;

	result = atan2(b.z - a.z,b.x - x);

	result = result * 180.0f / DX_PI_F;

	return result;
}
//---------------------------------------------
// VECTOR�^�ō��W�ݒ�
//---------------------------------------------
void Vector3::Set(VECTOR v_)
{
	x = v_.x;
	y = v_.y;
	z = v_.z;
}
//---------------------------------------------
// VECTOR�^�ŉ��Z�E���Z
//---------------------------------------------
void Vector3::TranslateV(VECTOR v_)
{
	x += v_.x;
	y += v_.y;
	z += v_.z;
}
