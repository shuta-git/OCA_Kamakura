//===========================================================================
//!
//!	@file	gmTypedef.h
//!	@brief	型宣言
//!
//===========================================================================
#pragma once

#define GM_ASSERT_MESSAGE(a, b) assert(a&& b)

//===========================================================================
//!	基本型
//===========================================================================
using s8  = std::int8_t;     //!< 符号あり 8bit整数
using u8  = std::uint8_t;    //!< 符号なし 8bit整数
using s16 = std::int16_t;    //!< 符号あり16bit整数
using u16 = std::uint16_t;   //!< 符号なし16bit整数
using s32 = std::int32_t;    //!< 符号あり32bit整数
using u32 = std::uint32_t;   //!< 符号なし32bit整数
using s64 = std::int64_t;    //!< 符号あり64bit整数
using u64 = std::uint64_t;   //!< 符号なし64bit整数
using f32 = float;           //!< 単精度浮動小数点数
using f64 = double;          //!< 倍精度浮動小数点数

//===========================================================================
// 中継ポインタ
//===========================================================================
template<typename T>
class ptr
{
public:
    ptr() = default;

    // コンストラクタ(ポインタ)
    ptr(T* p)
    : ptr_(p)
    {
    }

    // コンストラクタ(実体)
    ptr(T& p)
    : ptr_(&p)
    {
    }

    // コンストラクタ unique_ptr<T>
    template<typename U>
    ptr(std::unique_ptr<U>& p)
    : ptr_(p.get())
    {
    }

    //! ポインタ演算
    T* operator->() const { return ptr_; }

    //! ポインタ演算
    T& operator*() const { return ptr_; }

    //! キャスト
    operator T*() const { return ptr_; }

private:
    T* ptr_;
};

//! 安全な解放（二重解放・無効ポインタ防止）
#define GM_SAFE_RELEASE(PTR_) \
    if(PTR_) {                \
        PTR_->Release();      \
        PTR_ = nullptr;       \
    }

//! 安全なdelete（二重解放・無効ポインタ防止）
// #define GM_DELETE(PTR_) if( PTR_ ) { delete PTR_; PTR_ = nullptr; }

template<typename T>
void GM_DELETE(T*& p)
{
    if(p) {
        delete p;
        p = nullptr;
    }
}

//==============================================================
//!	カラー
//==============================================================
struct Color
{
    Color() {}

    Color(u8 r, u8 g, u8 b, u8 a = 255)
    {
        r_ = r;
        g_ = g;
        b_ = b;
        a_ = a;
    }

    Color(const Color& color)
    {
        value_ = color.value_;
    }

    const Color& operator=(const Color& color)
    {
        value_ = color.value_;
        return *this;
    }

    union
    {
        struct
        {
            u8 r_;
            u8 g_;
            u8 b_;
            u8 a_;
        };
        u32 value_;
    };
};

//--------------------------------------------------------------
//	頂点構造体
//--------------------------------------------------------------
struct Vertex
{
    XMFLOAT3 position_;   //!< ローカル座標
    Color    color_;      //!< カラー
    XMFLOAT3 normal_;     //!< 法線
    XMFLOAT3 tangent_;    //!< 接ベクトル
    XMFLOAT3 binormal_;   //!< 従法線ベクトル
    XMFLOAT2 uv_;         //!< テクスチャUV座標
};



//===========================================================================
// END OF FILE
//===========================================================================
