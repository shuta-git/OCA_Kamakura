//---------------------------------------------------------------------------
//!	@file	gpu_PMD.h
//!	@brief	GPU PMD
//---------------------------------------------------------------------------
#pragma once

namespace gpu {

//===========================================================================
//! PMD
//===========================================================================
class PMD
{
public:

    virtual ~PMD() = default;

    //! VMDファイル読み込み
    virtual void LoadVMDFile(const char* filepath) = 0;

    //! モーション更新
    virtual void MotionUpdate() = 0; 

    //! pmd描画
    virtual void PMDRender(ID3D12GraphicsCommandList* c) = 0;

protected:

    PMD() = default;

};

//! PMDをファイルから作成
//! @parm [in] path ファイルパス
std::shared_ptr<gpu::PMD> createPMDFromFile(std::string path);


}   // namespace gpu