#pragma once

class Vector3
{
public:

	float x, y, z;							// �ϐ�

	//===================
	// �R���X�g���N�^
	//===================
	Vector3();
	Vector3(float x_,float y_ ,float z_);
	Vector3(VECTOR v_);
	//Vector3(const Vector3& v_);

	//===================
	// �f�X�g���N�^
	//===================
	~Vector3();


	// �[��������
	void Clear();

	//===================
	// Get�֐�
	//===================
	float GetX() const { return x; }
    float GetY() const { return y; }
	float GetZ() const { return z; }
	// VECTOR�^�ō��W���擾
	VECTOR GetV() const;

	//===================
	// Set�֐�
	//===================
	
	void SetX(float x_) { x = x_; }
	void SetY(float y_) { y = y_; }
	void SetZ(float z_) { z = z_; }
	// VECTOR�^�ō��W�ݒ�
	void Set(VECTOR v_);
	// ���W�ݒ�
	void Set(float x_, float y_, float z_);
	// Vector ��ݒ�
	void Set(Vector3 &v_);
	// ���Z�E���Z
	void Translate(float x_, float y_, float z_);
	// VECTOR�^�ŉ��Z�E���Z
	void TranslateV(VECTOR v_);
	// ���Z
	void add(VECTOR v);
	// ���Z
	void add(VECTOR v0, VECTOR v1);
	// ���Z
	void subtract(VECTOR v);
	// ���Z
	void subtract(VECTOR v0, VECTOR v1);
	// �|���Z
	void scale(float scale);
	// ���Z
	Vector3 operator+(const Vector3& v)const;
	// ���Z
	Vector3 operator-(const Vector3& v)const;
	// �X�P�[�����O*
	Vector3 operator*(float scale)const;
	// �X�P�[�����O/
	Vector3 operator/(float scale)const;
	// ������2��
	float LengthSq()const;
	// �x�N�g���̒���
	float Length()const;
	// �����̐ݒ�
	void Length(float len);
	// ���K��
	Vector3 normalize()const;
	// ����
	float dot(VECTOR a, VECTOR b);
	// ����
	float dot(Vector3 a, Vector3 b);
	// �O��
	Vector3 cross(VECTOR a, VECTOR b);
	// ��_�Ԃ̋������擾
	float GetSqrtf(Vector3 a , Vector3 b) const;
	// ��_�Ԃ̊p�x���擾
	float GetRadian(Vector3 a, Vector3 b)const;


};

