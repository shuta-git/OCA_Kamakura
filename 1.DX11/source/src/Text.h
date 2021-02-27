//===========================================================================
//!	@file	Text.h
//!	@brief	�e�L�X�g
//===========================================================================
#pragma once

namespace gpu{

	//! �e�L�X�g����
	//! @parm [in] x	 x���W
	//! @parm [in] y	 y���W
	//! @parm [in] scale �e�L�X�g�T�C�Y
	//! @parm [in] text  �e�L�X�g
	void createText(f32 x, f32 y, f32 scale, const char* text, ...);
		
	//! ���̃e�L�X�g���W��ݒ�
	//! @parm [in] x	 x���W
	//! @parm [in] y	 y���W
	void nextTextPos(f32 x,f32 y);

}


