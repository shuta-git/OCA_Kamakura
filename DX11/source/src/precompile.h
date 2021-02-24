//===========================================================================
//!
//!	@file	precompile.h
//!	@brief	�v���R���p�C���w�b�_�[
//!
//===========================================================================
#pragma once

#include <SDKDDKVer.h>

//===========================================================================
// Windows�w�b�_�[�t�@�C��
//===========================================================================
#define NOMINMAX

#include <windows.h>

#include <wrl/client.h>

template<typename T>
using com_ptr = Microsoft::WRL::ComPtr<T>;

//===========================================================================
// C�����^�C���w�b�_�[�t�@�C��
//===========================================================================
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//===========================================================================
// STL�֘A
//===========================================================================
#include <algorithm>
#include <array>
#include <exception>
#include <malloc.h>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <fstream>
#include <sstream>

//---- VisualStudio2010 ��stdint.h��intsafe.h���Փ˂��邽�߂̌x���΍�
#pragma warning(push)
#pragma warning(disable : 4005)
#include <stdint.h>
#include <intsafe.h>
#pragma warning(pop)

//===========================================================================
// DirectX�֘A
//===========================================================================
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d2d1_1.h>

// ���C�u�����t�@�C���̃����N�w��
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

//---- DirectX Math
#include <DirectXMath.h>

//---- DirectX Tex
#include<../DirectXTex/DirectXTex/DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "OpenSource/DirectXTex/DirectXTex/Bin/Desktop_2017_Win10/x64/Debug/DirectXTex.lib")
#else
#pragma comment(lib, "OpenSource/DirectXTex/DirectXTex/Bin/Desktop_2017_Win10/x64/Release/DirectXTex.lib")
#endif

//---- DirectX ToolKit
//#include <OpenSource/DirectXTK/Inc/Audio.h>
#include <../DirectXTK/Inc/CommonStates.h>
#include <../DirectXTK/Inc/DDSTextureLoader.h>
#include <../DirectXTK/Inc/DirectXHelpers.h>
#include <../DirectXTK/Inc/Effects.h>
#include <../DirectXTK/Inc/GeometricPrimitive.h>
#include <../DirectXTK/Inc/Model.h>
#include <../DirectXTK/Inc/PrimitiveBatch.h>
#include <../DirectXTK/Inc/ScreenGrab.h>
#include <../DirectXTK/Inc/SimpleMath.h>
#include <../DirectXTK/Inc/SpriteBatch.h>
#include <../DirectXTK/Inc/SpriteFont.h>
#include <../DirectXTK/Inc/VertexTypes.h>
#include <../DirectXTK/Inc/WICTextureLoader.h>

#ifdef _DEBUG
#pragma comment(lib, "OpenSource/DirectXTK/Bin/Desktop_2017/x64/Debug/DirectXTK.lib")
#else
#pragma comment(lib, "OpenSource/DirectXTK/Bin/Desktop_2017/x64/Release/DirectXTK.lib")
#endif

using namespace DirectX;

//===========================================================================
//	imgui
//===========================================================================
#include <../imgui/imgui.h>
#include <../imgui/examples/imgui_impl_win32.h>
#include <../imgui/examples/imgui_impl_dx11.h>

//===========================================================================
//src
//===========================================================================
#include "gmTypedef.h"

#include "Math.h"
#include "Vector.h"
#include "Matrix.h"

#include "Handle.h"        //!< �n���h���Ǘ�
#include "FileWatcher.h"   //!< �t�@�C���Ď�
#include "Resource.h"	   //!< ���\�[�X���
#include "FPSCounter.h"	   //!< FPS�J�E���^�[

#include "GpuBuffer.h"        //!< �o�b�t�@
#include "GpuTexture.h"       //!< �e�N�X�`��
#include "GpuInputLayout.h"   //!< ���̓��C�A�E�g
#include "GpuShader.h"        //!< �V�F�[�_�[
#include "GpuCommand.h"       //!< GPU�R�}���h

#include "Primitive.h"        //!< �v���~�e�B�u�`��
#include "Camera.h"           //!< �J����
#include "CameraDebug.h"      //!< �f�o�b�O�J����
#include "CameraPlayer.h"	  //!< �v���C���[�J����
#include "CameraTitle.h"	  //!< �^�C�g���J����

#include "Shadowmap.h"        //!< �V���h�E�}�b�v�Ǘ�
#include "GBuffer.h"          //!< GBuffer�Ǘ�
#include "FilterGlare.h"	  //!< �O���A�t�B���^�[

#include "Pass.h"             //!< �����_�����O�p�X
#include "PassShadow.h"       //!< �V���h�E�����p�X
#include "PassGbuffer.h"      //!< Gbuffer�����p�X
#include "PassLighting.h"     //!< �Ɩ��p�X
#include "PassFilterHDR.h"    //!< HDR�t�B���^�[
#include "SystemRender.h"     //!< �����_�����O�V�X�e��

#include "Text.h"			  //!< �e�L�X�g

#include "ModelMQO.h"		  //!< MQO���f��
#include "ModelFBX.h"		  //!< FBX���f��

							  
#include "Base.h"			  //!< ���
							  
#include "CollisionBase.h"    //!< �R���W�������
							  
#include "Collider.h"		  //!< �R���C�_�[���
#include "SphereCollider.h"	  //!< �X�t�B�A�R���C�_�[
#include "CapsuleCollider.h"  //!< �J�v�Z���R���C�_�[
#include "BoxCollider.h"	  //!< �{�b�N�X�R���C�_�[

#include "ResourceManager.h"	//!< ���f�����\�[�X

#include "CollisionDetection.h" //!< �����蔻��

#include "Map.h"			  //!< �}�b�v

#include "PlayerBase.h"		  //!< �v���C���[���
#include "Player.h"			  //!< �v���C���[
#include "EnemyBase.h"		  //!< �G�l�~�[���
#include "Pumpkin.h"		  
							  
#include "ManagerBase.h"      //!< �}�l�W���[���
#include "EnemyManager.h"	  //!< �G�l�~�[�}�l�[�W���[
							  
#include "Scene.h"			  //!< �V�[�����
#include "SceneMain.h"		  //!< ���C���V�[��
#include "LoadScene.h"
#include "TitleScene.h"		  //!< �^�C�g���V�[��
#include "GameScene.h"		  //!< �Q�[���V�[��
							  
#include "SceneController.h"  //!< �V�[���R���g���[���[

#include "main.h"

//===========================================================================
//	END OF FILE
//===========================================================================
