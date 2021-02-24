//---------------------------------------------------------------------------
//!	@file	precompile.h
//!	@brief	�v���R���p�C���w�b�_�[
//---------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------
//!	@defgroup	Windows�w�b�_�[
//--------------------------------------------------------------
//@{

#define NOMINMAX   // std::min std::max��WindowsSDK��min/max�}�N�����Փ˂��邽��WindowsSDK���𖳌���
#define STRICT     // Windows�I�u�W�F�N�g�̌^�������Ɉ���

#include <windows.h>
#include <wrl.h>   // ComPtr�p

//@}
//--------------------------------------------------------------
//!	@defgroup	STL(Standard Template Library)
//--------------------------------------------------------------
//@{

#include <array>
#include <string>
#include <string_view>

// �}���`�X���b�h�֘A
#include <thread>
#include <atomic>
#include <mutex>

//@}
//--------------------------------------------------------------
//!	@defgroup	DirectX�֘A
//--------------------------------------------------------------
//@{

#include<tchar.h>
#include<d3d12.h>
#include<d3dx12.h>

#include<dxgi1_6.h>	// DXGI �𐧌䂷�邽�߂ɕK�v
#include<D3Dcompiler.h>
#include<DirectXMath.h>
#include "d3dx12.h"

#include<vector>


#ifdef _DEBUG
#include<iostream>
#endif

// DirectX�c�[���L�b�g
#include "DirectXTK12/Inc/GamePad.h"
#include "DirectXTex/DirectXTex/DirectXTex.h"

using namespace DirectX;

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "winmm.lib")


#pragma warning(push)
#pragma warning(disable : 26495)

// �x�N�g�����Z���C�u���� hlslpp
#include "hlslpp/include/hlsl++.h"
using namespace hlslpp;


#pragma warning(pop)

//@}
//--------------------------------------------------------------
//!	@defgroup	imgui�֘A
//--------------------------------------------------------------
//@{

#include"imgui/imgui.h"
#include"imgui/imgui_impl_win32.h"
#include"imgui/imgui_impl_dx12.h"

//@}

//--------------------------------------------------------------
//!	@defgroup	�A�v���P�[�V����
//--------------------------------------------------------------
//@{
#include "typedef.h"	  // �^��`
#include "debug.h"		  // �f�o�b�O�֘A
#include "memory.h"       // ������

#include "Vector.h"
#include "Matrix.h"

#include "StringEncode.h" // ������UTF8 ���� UNICODE ���ݕϊ�
#include "math_Matrix.h"  // �s�񃆁[�e�B���e�B�[

#include "camera.h"   // �J����

#include "gpu_Fence.h"			  // GPU �t�F���X
#include "gpu_Shader.h" 		  // GPU �V�F�[�_�[
#include "gpu_DescriptorHeap.h"   // GPU Descriptor�q�[�v�Ǘ�
#include "gpu_Render.h"			  // GPU �����_�����O�Ǘ�

#include "gpu_Texture.h"          // GPU �e�N�X�`���[
#include "gpu_PMD.h"		      // GPU PMD
#include "gpu_ModelFBX.h"

#include "gpu_Buffer.h"			  // GPU �o�b�t�@
#include "gpu_SwapChain.h"		  // GPU �X���b�v�`�F�C��

#include"main.h"	// �A�v���P�[�V�����J�n

//@}s