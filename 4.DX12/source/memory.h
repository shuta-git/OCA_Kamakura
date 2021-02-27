//---------------------------------------------------------------------------
//!	@file	memory.h
//!	@brief	メモリ関連
//---------------------------------------------------------------------------

namespace memory {

// Non-temporalメモリコピー
// @note 通常のメモリコピーで一回限りの単純コピーを行うとCPUキャッシュ領域が
//       無駄に追い出されてしまうため低速化します。
//       この関数はキャッシュの追い出しを行わない高速コピー
// @attention メモリは16バイト境界でコピーサイズも16バイトの倍数
inline void fastCopy(void* dest, const void* src, size_t size)
{
    //assert(size % 16 == 0); // 16バイトの倍数になっていない場合はエラー

    if(size & 0xf) {   // 16バイトの倍数になっていない場合は通常のコピー
        memcpy(dest, src, size);
        return;
    }

    // 16バイト単位のコピー
    std::uintptr_t s = reinterpret_cast<std::uintptr_t>(src);
    std::uintptr_t d = reinterpret_cast<std::uintptr_t>(dest);

    size_t count = size >> 4;   // ÷16
    for(u32 i = 0; i < count; ++i) {
        __m128i x = _mm_load_si128(reinterpret_cast<const __m128i*>(s));   // 読み
        _mm_stream_si128(reinterpret_cast<__m128i*>(d), x);                // 書き

        s += 16;
        d += 16;
    }
}

}   // namespace memory
