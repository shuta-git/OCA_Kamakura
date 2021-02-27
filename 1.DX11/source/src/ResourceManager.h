//===========================================================================
//!	@file	ResourceManager.h
//!	@brief	���\�[�X�}�l�[�W���[
//===========================================================================
#pragma once

namespace gpu {

	//! FBX���t�@�C������쐬
	//! @parm [in] filePath �t�@�C���p�X
	//! @parm [in] scale    �T�C�Y
	std::shared_ptr<ModelFBX> loadFBX(char* filePath, f32 scale = 1.0f);



}


