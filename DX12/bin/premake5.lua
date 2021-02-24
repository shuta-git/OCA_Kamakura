
require "setup"

--============================================================================
-- ソリューションファイル
--============================================================================
config_solution("DX12Project")
    startproject "DX12Project"		-- スタートアッププロジェクト名



--============================================================================
-- 外部のプロジェクトファイル
--============================================================================
group "OpenSource"

-----------------------------------------------------------------
-- DirectXTex
-----------------------------------------------------------------
config_project("DirectXTex", "StaticLib")

	characterset      "UNICODE"		-- UNICODE文字セット

	local SOURCE_PATH = "../opensource/DirectXTex/DirectXTex"

	-- 追加するソースコード
	-- *=フォルダ内 **=フォルダ内とその階層下サブフォルダ内
    files {
		path.join(SOURCE_PATH, "*.cpp"),
		path.join(SOURCE_PATH, "*.h"),
		path.join(SOURCE_PATH, "*.inl"),
	}
	
	-- "" インクルードパス
	includedirs {
		SOURCE_PATH,
	}

	-- プリプロセッサ #define
    defines {
		"_WIN32_WINNT=0x0A00",
		"_CRT_STDIO_ARBITRARY_WIDE_SPECIFIERS",
	}

	-- プリコンパイル済ヘッダー
	pchheader( "DirectXTexP.h" )
	pchsource( path.join(SOURCE_PATH, "DirectXTexUtil.cpp") )
	forceincludes ( "DirectXTexP.h" )

	-- フォルダ分け
	vpaths {
		["ヘッダー ファイル"] = { "../**.h", "../**.hxx", "../**.hpp", "../**.inl" },
		["ソース ファイル"]   = { "../**.c", "../**.cxx", "../**.cpp" },
	}

-----------------------------------------------------------------
-- DirectXTK12
-----------------------------------------------------------------
config_project("DirectXTK12", "StaticLib")

	characterset      "UNICODE"		-- UNICODE文字セット

	local SOURCE_PATH = "../opensource/DirectXTK12"

	-- 追加するソースコード
	-- *=フォルダ内 **=フォルダ内とその階層下サブフォルダ内
    files {
		path.join(SOURCE_PATH, "Inc/**.cpp"),
		path.join(SOURCE_PATH, "Inc/**.h"),
		path.join(SOURCE_PATH, "Inc/**.inl"),
		path.join(SOURCE_PATH, "Src/**.cpp"),
		path.join(SOURCE_PATH, "Src/**.h"),
		path.join(SOURCE_PATH, "Src/**.inl"),
	}
	removefiles {
		path.join(SOURCE_PATH, "Src/XboxDDSTextureLoader.cpp"),
	}

	-- "" インクルードパス
	includedirs {
		path.join(SOURCE_PATH, "Inc"),
		path.join(SOURCE_PATH, "Src"),
	}

	-- プリプロセッサ #define
    defines {
		"_WIN32_WINNT=0x0A00",
		"_CRT_STDIO_ARBITRARY_WIDE_SPECIFIERS",
	}

	-- プリコンパイル済ヘッダー
	pchheader( "pch.h" )
	pchsource( path.join(SOURCE_PATH, "Src/pch.cpp") )
	forceincludes ( "pch.h" )


-----------------------------------------------------------------
-- hlslpp
-----------------------------------------------------------------
config_project("hlsl++", "StaticLib")

	local SOURCE_PATH = "../opensource/hlslpp"

	files
	{
		path.join(SOURCE_PATH, "include/*.h"),
		path.join(SOURCE_PATH, "include/*.natvis"),
	}
-----------------------------------------------------------------
-- imgui
-----------------------------------------------------------------
config_project("imgui", "StaticLib")

	local SOURCE_PATH = "../opensource/imgui"

	files
	{
		path.join(SOURCE_PATH, "*.h"),
		path.join(SOURCE_PATH, "*.cpp"),
	}
	
--============================================================================
-- プロジェクトファイル
--============================================================================
group ""

config_project("DX12Project", "WindowedApp")

	-- ソースコード
	local SOURCE_PATH = "../source"
	-- OpenFBXソースコード
	local OPENFBX_SOURCE_PATH = "../opensource/OpenFBX/src"

	debugdir  "../resource"			-- 実行開始時のカレントディレクトリ

	-- 追加するソースコード
	-- *=フォルダ内 **=フォルダ内とその階層下サブフォルダ内
    files {
		path.join(SOURCE_PATH, "**.h"),		   -- ソースコード
		path.join(SOURCE_PATH, "**.cpp"),	   -- ソースコード
		path.join(OPENFBX_SOURCE_PATH , "*.h"),   -- OpenFBX
		path.join(OPENFBX_SOURCE_PATH , "*.cpp"), -- OpenFBX
		path.join("../resource", "**.fx"),	-- シェーダー
		path.join("../resource", "**.hlsl"),	-- シェーダー
	}
	
	-- "" インクルードパス
	includedirs {
		SOURCE_PATH,
		"../opensource",	-- オープンソース
	}

	-- 依存ライブラリ・プロジェクト
	links {
		"d3dcompiler",
		"dxguid",
		"d3d12",
		"dxgi",
	}

	-- プリプロセッサ #define
    defines {
	--	"_CRT_SECURE_NO_WARNINGS",
	}

	-- プリコンパイル済ヘッダー
	pchheader "precompile.h"
	pchsource (path.join(SOURCE_PATH, "precompile.cpp"))
	forceincludes "precompile.h"

	-- フォルダ分け
	vpaths {
		["OpenFBX"] = {OPENFBX_SOURCE_PATH },
		["ヘッダー ファイル"] = { "../**.h", "../**.hxx", "../**.hpp", "../**.inl" },
		["ソース ファイル"]   = { "../**.c", "../**.cxx", "../**.cpp" },
	}

	-- プロジェクトファイルの依存関係
	links {
		"DirectXTex",
		"DirectXTK12",
		"imgui",
	}
