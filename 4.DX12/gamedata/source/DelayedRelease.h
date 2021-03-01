//---------------------------------------------------------------------------
//!	@file	DelayedRelease.h
//!	@brief	遅延解放
//---------------------------------------------------------------------------
#pragma once

//===========================================================================
//! 遅延解放
//  解放リクエストを登録。登録されたリソースは2フレーム後に実際の解放を実行。
//	スレッドセーフなAPIです。
//===========================================================================
class DelayedRelease final
{
public:
    //! デフォルトコンストラクタ
    DelayedRelease() = default;

    //! デストラクタ
    ~DelayedRelease();

    //! 解放リストに登録
    void emplace_back(com_ptr<ID3D12Resource>&& resource);

    // 遅延解放実行
    void execute();

private:
    // copy禁止/move禁止
    DelayedRelease(const DelayedRelease& rhs) = delete;
    DelayedRelease(DelayedRelease&& lhs)      = delete;
    DelayedRelease& operator=(const DelayedRelease& rhs) = delete;
    DelayedRelease& operator=(DelayedRelease&& lhs) = delete;

private:
    static constexpr u32         FRAME_COUNT = 3;             //!< フレーム数
    std::mutex                   mutex_;                       //!< スレッドセーフ用
    std::vector< com_ptr<ID3D12Resource> > releaseList_[FRAME_COUNT];   //!< 解放リスト
    u32                          currentIndex = 0;            //!< 現在の使用中のリスト
};

extern DelayedRelease delayedRelease_;





