//---------------------------------------------------------------------------
//!	@file	DelayedRelease.cpp
//!	@brief	遅延解放
//---------------------------------------------------------------------------
#include "DelayedRelease.h"

DelayedRelease delayedRelease_;

//---------------------------------------------------------------------------
//! デストラクタ
//---------------------------------------------------------------------------
DelayedRelease ::~DelayedRelease()
{
    for(auto& list : releaseList_) {
        for(auto& p : list) {
            p->Release();
        }
    }
}

//---------------------------------------------------------------------------
//! 解放リストに登録
//---------------------------------------------------------------------------
void DelayedRelease::emplace_back(com_ptr<ID3D12Resource>&& resource)
{
    std::lock_guard<std::mutex> lock(mutex_);

    releaseList_[currentIndex].emplace_back(std::move(resource));
}

//---------------------------------------------------------------------------
// 遅延解放実行
//---------------------------------------------------------------------------
void DelayedRelease::execute()
{
    std::lock_guard<std::mutex> lock(mutex_);

    // インデックス番号を進める
    currentIndex = (currentIndex + 1) % FRAME_COUNT;

    //----------------------------------------------------------
    // もっとも古い解放リストを全解放
    //----------------------------------------------------------
    auto& list = releaseList_[currentIndex];
    for(auto& p : list) {
        p->Release();
    }
}
