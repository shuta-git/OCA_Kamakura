//===========================================================================
//!
//!	@file	gmTypedef.h
//!	@brief	�^�錾
//!
//===========================================================================
#pragma once

#define GM_ASSERT_MESSAGE(a, b) assert(a&& b)

//===========================================================================
//!	��{�^
//===========================================================================
using s8  = std::int8_t;     //!< �������� 8bit����
using u8  = std::uint8_t;    //!< �����Ȃ� 8bit����
using s16 = std::int16_t;    //!< ��������16bit����
using u16 = std::uint16_t;   //!< �����Ȃ�16bit����
using s32 = std::int32_t;    //!< ��������32bit����
using u32 = std::uint32_t;   //!< �����Ȃ�32bit����
using s64 = std::int64_t;    //!< ��������64bit����
using u64 = std::uint64_t;   //!< �����Ȃ�64bit����
using f32 = float;           //!< �P���x���������_��
using f64 = double;          //!< �{���x���������_��

//===========================================================================
// ���p�|�C���^
//===========================================================================
template<typename T>
class ptr
{
public:
    ptr() = default;

    // �R���X�g���N�^(�|�C���^)
    ptr(T* p)
    : ptr_(p)
    {
    }

    // �R���X�g���N�^(����)
    ptr(T& p)
    : ptr_(&p)
    {
    }

    // �R���X�g���N�^ unique_ptr<T>
    template<typename U>
    ptr(std::unique_ptr<U>& p)
    : ptr_(p.get())
    {
    }

    //! �|�C���^���Z
    T* operator->() const { return ptr_; }

    //! �|�C���^���Z
    T& operator*() const { return ptr_; }

    //! �L���X�g
    operator T*() const { return ptr_; }

private:
    T* ptr_;
};

//! ���S�ȉ���i��d����E�����|�C���^�h�~�j
#define GM_SAFE_RELEASE(PTR_) \
    if(PTR_) {                \
        PTR_->Release();      \
        PTR_ = nullptr;       \
    }

//! ���S��delete�i��d����E�����|�C���^�h�~�j
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
//!	�J���[
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
//	���_�\����
//--------------------------------------------------------------
struct Vertex
{
    XMFLOAT3 position_;   //!< ���[�J�����W
    Color    color_;      //!< �J���[
    XMFLOAT3 normal_;     //!< �@��
    XMFLOAT3 tangent_;    //!< �ڃx�N�g��
    XMFLOAT3 binormal_;   //!< �]�@���x�N�g��
    XMFLOAT2 uv_;         //!< �e�N�X�`��UV���W
};



//===========================================================================
// END OF FILE
//===========================================================================
