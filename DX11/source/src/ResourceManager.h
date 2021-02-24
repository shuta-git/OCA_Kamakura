//===========================================================================
//!	@file	ResourceManager.h
//!	@brief	リソースマネージャー
//===========================================================================
#pragma once

namespace gpu {

	//! FBXをファイルから作成
	//! @parm [in] filePath ファイルパス
	//! @parm [in] scale    サイズ
	std::shared_ptr<ModelFBX> loadFBX(char* filePath, f32 scale = 1.0f);



}


