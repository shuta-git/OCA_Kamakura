//===========================================================================
//!	@file	Text.h
//!	@brief	テキスト
//===========================================================================
#pragma once

namespace gpu{

	//! テキスト生成
	//! @parm [in] x	 x座標
	//! @parm [in] y	 y座標
	//! @parm [in] scale テキストサイズ
	//! @parm [in] text  テキスト
	void createText(f32 x, f32 y, f32 scale, const char* text, ...);
		
	//! 次のテキスト座標を設定
	//! @parm [in] x	 x座標
	//! @parm [in] y	 y座標
	void nextTextPos(f32 x,f32 y);

}


