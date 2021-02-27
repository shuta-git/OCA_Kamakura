//===========================================================================
//!	@file   Resource.h
//!	@brief	リソース管理
//===========================================================================
#pragma once

//===========================================================================
//! リソース基底
//===========================================================================
class Resource
{
public:
	//! コンストラクタ
	Resource();

	//! デストラクタ
	virtual ~Resource();

	//-------------------------------------------------------------
	//!	@name 継承実装が必要な関数
	//-------------------------------------------------------------
	//@{

	//! リソースのリロード
	virtual	bool	reload()=0;

	//@}
	//-------------------------------------------------------------
	//!	@name アクセサ
	//-------------------------------------------------------------
	//@{

	//! リソースパスの設定
	void	setPath(const std::wstring& path) { path_ = path; }

	//! リソースパスの取得
	const std::wstring&	getPath() const { return path_; }

	//! 拡張子の設定
	void	setExt(const std::wstring& ext) { ext_=ext; }

	//! 拡張子の取得
	const std::wstring&	getExt() const { return ext_;}

	//@}
private:
	std::wstring	path_;	// リソースパス
	std::wstring	ext_;	// ファイルの拡張子

};

//! ファイルパスからリソースパス生成
std::wstring makeResourcePath(const char* fileName);

//! リソースパスからリソースを検索
//! @return すでに読み込まれているリソースのポインタ(nullptrなら検出されず)
//Resource*	findResource(const std::wstring& resourcePath);

//! リソースをリロード
void	reloadResource(const std::wstring& resourcePath);
