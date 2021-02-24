//===========================================================================
//!
//!	@file	Matrix.h
//!	@brief	行列
//!
//===========================================================================
#pragma once

struct Matrix
{
    // デフォルトコンストラクタ
    Matrix() {}

    //! コンストラクタ
    Matrix(float m11, float m12, float m13, float m14,
           float m21, float m22, float m23, float m24,
           float m31, float m32, float m33, float m34,
           float m41, float m42, float m43, float m44)
    {
        m_[0][0] = m11;
        m_[0][1] = m12;
        m_[0][2] = m13;
        m_[0][3] = m14;

        m_[1][0] = m21;
        m_[1][1] = m22;
        m_[1][2] = m23;
        m_[1][3] = m24;

        m_[2][0] = m31;
        m_[2][1] = m32;
        m_[2][2] = m33;
        m_[2][3] = m34;

        m_[3][0] = m41;
        m_[3][1] = m42;
        m_[3][2] = m43;
        m_[3][3] = m44;
    }

    // 行列 x 行列
    Matrix operator*(const Matrix& other)
    {
        Matrix result;

        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < 4; ++j) {
                result.m_[i][j] = (m_[i][0] * other.m_[0][j]) +
                                  (m_[i][1] * other.m_[1][j]) +
                                  (m_[i][2] * other.m_[2][j]) +
                                  (m_[i][3] * other.m_[3][j]);
            }
        }

        return result;
    }

    // 単位行列
    static Matrix identity()
    {
        return Matrix(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    }

    // 平行移動
    static Matrix translate(const Vector3& v)
    {
        return Matrix(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            v.x_, v.y_, v.z_, 1.0f);
    }

    // 拡大縮小
    static Matrix scale(const Vector3& s)
    {
        return Matrix(
            s.x_, 0.0f, 0.0f, 0.0f,
            0.0f, s.y_, 0.0f, 0.0f,
            0.0f, 0.0f, s.z_, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    }

    // X軸中心の回転
    static Matrix rotateX(float radian)
    {
        float s = sinf(radian);
        float c = cosf(radian);

        return Matrix(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, c, s, 0.0f,
            0.0f, -s, c, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    }

    // Y軸中心の回転
    static Matrix rotateY(float radian)
    {
        float s = sinf(radian);
        float c = cosf(radian);

        return Matrix(
            c, 0.0f, -s, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            s, 0.0f, c, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    }

    // Z軸中心の回転
    static Matrix rotateZ(float radian)
    {
        float s = sinf(radian);
        float c = cosf(radian);

        return Matrix(
            c, s, 0.0f, 0.0f,
            -s, c, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    }

    // 任意軸中心の回転
    static Matrix rotateAxis(
        const Vector3& v,   // 中心軸
        float          radian)       // 回転角
    {
        float s = sinf(radian);
        float c = cosf(radian);

        Vector3 axis = v.normalize();
        float   x    = axis.x_;
        float   y    = axis.y_;
        float   z    = axis.z_;

        Matrix matrix;
        matrix.m_[0][0] = x * x * (1.0f - c) + c;
        matrix.m_[0][1] = x * y * (1.0f - c) + z * s;
        matrix.m_[0][2] = x * z * (1.0f - c) - y * s;
        matrix.m_[0][3] = 0.0f;

        matrix.m_[1][0] = x * y * (1.0f - c) - z * s;
        matrix.m_[1][1] = y * y * (1.0f - c) + c;
        matrix.m_[1][2] = y * z * (1.0f - c) + x * s;
        matrix.m_[1][3] = 0.0f;

        matrix.m_[2][0] = x * z * (1.0f - c) + y * s;
        matrix.m_[2][1] = y * z * (1.0f - c) - x * s;
        matrix.m_[2][2] = z * z * (1.0f - c) + c;
        matrix.m_[2][3] = 0.0f;

        matrix.m_[3][0] = 0.0f;
        matrix.m_[3][1] = 0.0f;
        matrix.m_[3][2] = 0.0f;
        matrix.m_[3][3] = 1.0f;

        return matrix;
    }

    const Vector3 axisX() const
    {
        return Vector3(m_[0][0], m_[0][1], m_[0][2]);
    }

    const Vector3 axisY() const
    {
        return Vector3(m_[1][0], m_[1][1], m_[1][2]);
    }

    const Vector3 axisZ() const
    {
        return Vector3(m_[2][0], m_[2][1], m_[2][2]);
    }

    const Vector3 translate() const
    {
        return Vector3(m_[3][0], m_[3][1], m_[3][2]);
    }

    float m_[4][4];
};

//===========================================================================
//	EOF
//===========================================================================
