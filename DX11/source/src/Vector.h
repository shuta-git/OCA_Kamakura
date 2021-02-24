//===========================================================================
//!
//!	@file	Vector.h
//!	@brief	ベクトル
//!
//===========================================================================
#pragma once

// プロトタイプ宣言
struct Vector4;
struct Matrix;

struct Float2
{
	f32	x_;
	f32	y_;
};

struct Float3
{
	f32	x_;
	f32	y_;
	f32	z_;
};

struct Float4
{
	f32	x_;
	f32	y_;
	f32	z_;
	f32	w_;
};



//===========================================================================
//!	３要素ベクトル構造体
//===========================================================================
struct Vector3
{
    //------------------------------------------------------------
    //!	@name 初期化
    //------------------------------------------------------------
    //@{

    //! デフォルトコンストラクタ
    Vector3();

    //! コンストラクタ (xyz指定)
    Vector3(f32 x, f32 y, f32 z);

    //! コンストラクタ
    Vector3(const Vector3& v);

    //! コンストラクタ
    Vector3(const Vector4& v);

    //@}
    //-------------------------------------------------------------
    //!	@name 代入
    //-------------------------------------------------------------
    //@{

    //! 代入
    Vector3& operator=(const Vector3& v);

    //@}
    //-------------------------------------------------------------
    //!	@name 単項演算子
    //-------------------------------------------------------------
    //@{

    //! 正の値
    Vector3 operator+() const;

    //! 負の値
    Vector3 operator-() const;

    //@}
    //------------------------------------------------------------
    //!	@name 演算
    //------------------------------------------------------------
    //@{

    //! 加算
    Vector3 operator+(const Vector3& v) const;

    //! 減算
    Vector3 operator-(const Vector3& v) const;

    //! スケーリング
    Vector3 operator*(f32 scale) const;

    //! スケーリング
    Vector3 operator/(f32 scale) const;

    //! 加算
    Vector3& operator+=(const Vector3& v);

    //! 減算
    Vector3& operator-=(const Vector3& v);

    //! スケーリング
    Vector3& operator*=(f32 scale);

    //! スケーリング
    Vector3& operator/=(f32 scale);

    //! 長さの２乗
    f32 lengthSq() const;

    //! ベクトルの長さ
    f32 length() const;

    //! 正規化
    Vector3 normalize() const;

    //! 2点間の距離を取得
    float sqrtf(Vector3 a, Vector3 b) const;

    //! 2点間の角度を取得
    float radian(Vector3 a, Vector3 b) const;

    //@}

    //! 座標変換(平行移動あり)
    Vector4 transform(const Matrix& matrix);

    //! 座標変換(ベクトルの回転。平行移動なし)
    Vector3 transformNormal(const Matrix& matrix);

    //!	内積
    //!	@param	a	ベクトルa
    //!	@param	b	ベクトルb
    //!	@return	内積値
    static f32 dot(Vector3 a, Vector3 b);

    //	外積
    //!	@param	a	ベクトルa
    //!	@param	b	ベクトルb
    //!	@return	外積値
    static Vector3 cross(Vector3 a, Vector3 b);

    f32 x_;
    f32 y_;
    f32 z_;
};

//===========================================================================
//!	4要素ベクトル
//===========================================================================
struct Vector4
{
    //! デフォルトコンストラクタ
    Vector4()
    {
    }

    //! コンストラクタ(4要素)
    Vector4(f32 x, f32 y, f32 z, f32 w)
    {
        x_ = x;
        y_ = y;
        z_ = z;
        w_ = w;
    }

    //! コンストラクタ(Vector3 + W値)
    Vector4(const Vector3& v, f32 w)
    {
        x_ = v.x_;
        y_ = v.y_;
        z_ = v.z_;
        w_ = w;
    }

    union
    {
        struct
        {
            f32 x_;
            f32 y_;
            f32 z_;
            f32 w_;
        };
        struct
        {
            f32 r_;
            f32 g_;
            f32 b_;
            f32 a_;
        };
    };
};

//===========================================================================
//	EOF
//===========================================================================
