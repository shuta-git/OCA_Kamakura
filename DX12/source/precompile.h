//---------------------------------------------------------------------------
//!	@file	precompile.h
//!	@brief	プリコンパイルヘッダー
//---------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------
//!	@defgroup	Windowsヘッダー
//--------------------------------------------------------------
//@{

#define NOMINMAX   // std::min std::maxとWindowsSDKのmin/maxマクロが衝突するためWindowsSDK側を無効化
#define STRICT     // Windowsオブジェクトの型を厳密に扱う

#include <windows.h>
#include <wrl.h>   // ComPtr用

//@}
//--------------------------------------------------------------
//!	@defgroup	STL(Standard Template Library)
//--------------------------------------------------------------
//@{

#include <array>
#include <string>
#include <string_view>

// マルチスレッド関連
#include <thread>
#include <atomic>
#include <mutex>

//@}
//--------------------------------------------------------------
//!	@defgroup	DirectX関連
//--------------------------------------------------------------
//@{

#include<tchar.h>
#include<d3d12.h>
#include<d3dx12.h>

#include<dxgi1_6.h>	// DXGI を制御するために必要
#include<D3Dcompiler.h>
#include<DirectXMath.h>
#include "d3dx12.h"

#include<vector>


#ifdef _DEBUG
#include<iostream>
#endif

// DirectXツールキット
#include "DirectXTK12/Inc/GamePad.h"
#include "DirectXTex/DirectXTex/DirectXTex.h"

using namespace DirectX;

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "winmm.lib")


#pragma warning(push)
#pragma warning(disable : 26495)

// ベクトル演算ライブラリ hlslpp
#include "hlslpp/include/hlsl++.h"
using namespace hlslpp;


#pragma warning(pop)

//@}
//--------------------------------------------------------------
//!	@defgroup	imgui関連
//--------------------------------------------------------------
//@{

#include"imgui/imgui.h"
#include"imgui/imgui_impl_win32.h"
#include"imgui/imgui_impl_dx12.h"

//@}

//--------------------------------------------------------------
//!	@defgroup	アプリケーション
//--------------------------------------------------------------
//@{
#include "typedef.h"	  // 型定義
#include "debug.h"		  // デバッグ関連
#include "memory.h"       // メモリ

#include "Vector.h"
#include "Matrix.h"

#include "StringEncode.h" // 文字列UTF8 ←→ UNICODE 相互変換
#include "math_Matrix.h"  // 行列ユーティリティー

#include "camera.h"   // カメラ

#include "gpu_Fence.h"			  // GPU フェンス
#include "gpu_Shader.h" 		  // GPU シェーダー
#include "gpu_DescriptorHeap.h"   // GPU Descriptorヒープ管理
#include "gpu_Render.h"			  // GPU レンダリング管理

#include "gpu_Texture.h"          // GPU テクスチャー
#include "gpu_PMD.h"		      // GPU PMD
#include "gpu_ModelFBX.h"

#include "gpu_Buffer.h"			  // GPU バッファ
#include "gpu_SwapChain.h"		  // GPU スワップチェイン

#include"main.h"	// アプリケーション開始

//@}s