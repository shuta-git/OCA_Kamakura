#pragma once
//---------------------------------------------------------------------------
//!	@file	main.h
//!	@brief	アプリケーションメイン
//---------------------------------------------------------------------------
#pragma once

namespace application {

//! ウィンドウハンドルを取得
HWND hwnd();

//! アプリケーションインスタンスハンドルを取得
HINSTANCE instance();

//! マウスフォーカスを持っているかどうか
bool hasMouseFocus();

}   // namespace application
