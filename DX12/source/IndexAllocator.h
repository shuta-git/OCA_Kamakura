//---------------------------------------------------------------------------
//!	@file	IndexAllocator.h
//!	@brief	インデックス番号管理
//---------------------------------------------------------------------------
#pragma once

class IndexAllocator
{
public:
    static constexpr u32 INVALID_VALUE = 0;

    IndexAllocator() = default;

    // サイズ指定で初期化
    //! @param  [in]    capacity    配列個数
    IndexAllocator(size_t capacity);

    //! インデックス番号を確保
    //! @return インデックス番号 (INVALID_VALUEなら失敗)
    u32 alloc();

    //! インデックス番号を解放
    void free(u32 index);

    //! 最大確保可能個数を取得
    size_t capacity() const { return indices_.capacity(); }

private:
    std::vector<u32> indices_;   // 配列リスト
};
