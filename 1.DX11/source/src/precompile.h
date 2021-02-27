//===========================================================================
//!
//!	@file	precompile.h
//!	@brief	プリコンパイルヘッダー
//!
//===========================================================================
#pragma once

#include <SDKDDKVer.h>

//===========================================================================
// Windowsヘッダーファイル
//===========================================================================
#define NOMINMAX

#include <windows.h>

#include <wrl/client.h>

template<typename T>
using com_ptr = Microsoft::WRL::ComPtr<T>;

//===========================================================================
// Cランタイムヘッダーファイル
//===========================================================================
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//===========================================================================
// STL関連
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

//---- VisualStudio2010 のstdint.hとintsafe.hが衝突するための警告対策
#pragma warning(push)
#pragma warning(disable : 4005)
#include <stdint.h>
#include <intsafe.h>
#pragma warning(pop)

//===========================================================================
// DirectX関連
//===========================================================================
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d2d1_1.h>

// ライブラリファイルのリンク指定
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

#include "Handle.h"        //!< ハンドル管理
#include "FileWatcher.h"   //!< ファイル監視
#include "Resource.h"	   //!< リソース基底
#include "FPSCounter.h"	   //!< FPSカウンター

#include "GpuBuffer.h"        //!< バッファ
#include "GpuTexture.h"       //!< テクスチャ
#include "GpuInputLayout.h"   //!< 入力レイアウト
#include "GpuShader.h"        //!< シェーダー
#include "GpuCommand.h"       //!< GPUコマンド

#include "Primitive.h"        //!< プリミティブ描画
#include "Camera.h"           //!< カメラ
#include "CameraDebug.h"      //!< デバッグカメラ
#include "CameraPlayer.h"	  //!< プレイヤーカメラ
#include "CameraTitle.h"	  //!< タイトルカメラ

#include "Shadowmap.h"        //!< シャドウマップ管理
#include "GBuffer.h"          //!< GBuffer管理
#include "FilterGlare.h"	  //!< グレアフィルター

#include "Pass.h"             //!< レンダリングパス
#include "PassShadow.h"       //!< シャドウ生成パス
#include "PassGbuffer.h"      //!< Gbuffer生成パス
#include "PassLighting.h"     //!< 照明パス
#include "PassFilterHDR.h"    //!< HDRフィルター
#include "SystemRender.h"     //!< レンダリングシステム

#include "Text.h"			  //!< テキスト

#include "ModelMQO.h"		  //!< MQOモデル
#include "ModelFBX.h"		  //!< FBXモデル

							  
#include "Base.h"			  //!< 基底
							  
#include "CollisionBase.h"    //!< コリジョン基底
							  
#include "Collider.h"		  //!< コライダー基底
#include "SphereCollider.h"	  //!< スフィアコライダー
#include "CapsuleCollider.h"  //!< カプセルコライダー
#include "BoxCollider.h"	  //!< ボックスコライダー

#include "ResourceManager.h"	//!< モデルリソース

#include "CollisionDetection.h" //!< 当たり判定

#include "Map.h"			  //!< マップ

#include "PlayerBase.h"		  //!< プレイヤー基底
#include "Player.h"			  //!< プレイヤー
#include "EnemyBase.h"		  //!< エネミー基底
#include "Pumpkin.h"		  
							  
#include "ManagerBase.h"      //!< マネジャー基底
#include "EnemyManager.h"	  //!< エネミーマネージャー
							  
#include "Scene.h"			  //!< シーン基底
#include "SceneMain.h"		  //!< メインシーン
#include "LoadScene.h"
#include "TitleScene.h"		  //!< タイトルシーン
#include "GameScene.h"		  //!< ゲームシーン
							  
#include "SceneController.h"  //!< シーンコントローラー

#include "main.h"

//===========================================================================
//	END OF FILE
//===========================================================================
