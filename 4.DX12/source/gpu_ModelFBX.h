//---------------------------------------------------------------------------
//!	@file	gpu_ModelFBX.h
//!	@brief	GPU FBX
//---------------------------------------------------------------------------
#pragma once

namespace gpu {

//===========================================================================
//! PMD
//===========================================================================
class FBX
{
public:

    virtual ~FBX() = default;

protected:

    FBX() = default;

};

//! PMDをファイルから作成
//! @parm [in] path ファイルパス
std::shared_ptr<gpu::FBX> createFBXFromFile(const char* fileName);


}   // namespace gpu