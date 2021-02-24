//===========================================================================
//!	@file   Resource.cpp
//!	@brief	リソース管理
//===========================================================================
#include "Resource.h"

namespace {
std::set<Resource*> resourceList;   // 読み込まれたリソース一覧
}

//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
Resource::Resource()
{
    resourceList.insert(this);
}

//---------------------------------------------------------------------------
//! デストラクタ
//---------------------------------------------------------------------------
Resource::~Resource()
{
    resourceList.erase(this);
}

//---------------------------------------------------------------------------
//! ファイルパスからリソースパス生成
//---------------------------------------------------------------------------
std::wstring makeResourcePath(const char* fileName)
{
    std::wstring resourcePath;

    // 必要な文字数を事前にカウント
    auto size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, fileName, -1, nullptr, 0);
    resourcePath.resize(size);   // null終端を含めた文字数
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, fileName, -1, resourcePath.data(), resourcePath.size());

    // すべて小文字に変更
    _wcslwr_s(resourcePath.data(), resourcePath.size());   // 小文字

    // バックスラッシュをスラッシュに変更
    for(auto& x : resourcePath) {
        if(x == '\\')
            x = '/';
    }

    return resourcePath;
}

//---------------------------------------------------------------------------
//! リソースをリロード
//---------------------------------------------------------------------------
void reloadResource(const std::wstring& resourcePath)
{
    for(auto& x : resourceList) {
        if(x->getPath() != resourcePath)
            continue;
        x->reload();
    }
}
