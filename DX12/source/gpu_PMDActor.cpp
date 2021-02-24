 //---------------------------------------------------------------------------
//!	@file   gpu_PMDActor.cpp
//!	@brief	GPU PMDActor
//---------------------------------------------------------------------------
#include"gpu_PMDActor.h"

namespace gpu {
 
//---------------------------------------------------------------------------
//! 白テクスチャを作成
//! @return 白テクスチャバッファ
//---------------------------------------------------------------------------
ID3D12Resource* CreateWhiteTexture()
{
    // テクスチャバッファ用のヒーププロパティ作成
    auto texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
    // テクスチャバッファ用のリソース詳細設定
    auto resDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, 4, 4);

    //----------------------------------------------------------
    //! テクスチャバッファの作成
    //----------------------------------------------------------
    ID3D12Resource* whiteBuff;

    if(FAILED(d3dDevice()->CreateCommittedResource(
           &texHeapProp,
           D3D12_HEAP_FLAG_NONE,
           &resDesc,
           D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
           nullptr,
           IID_PPV_ARGS(&whiteBuff)))) {
        return false;
    }

    std::vector<unsigned char> data(4 * 4 * 4);
    std::fill(data.begin(), data.end(), 0xff);   // 全部255で埋める

    //----------------------------------------------------------
    // WriteToSubresourceの作成
    // テクスチャ―情報(texbuff)をGPUに転送
    //----------------------------------------------------------
    if(FAILED(whiteBuff->WriteToSubresource(
           0,
           nullptr,
           data.data(),
           4 * 4,
           data.size()))) {
        return false;
    }

    return whiteBuff;
}

//---------------------------------------------------------------------------
// 黒テクスチャを作成
//---------------------------------------------------------------------------
ID3D12Resource* CreateBlackTexture()
{
    // テクスチャバッファ用のヒーププロパティ作成
    auto texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
    // テクスチャバッファ用のリソース詳細設定
    auto resDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, 4, 4);

    //-----------------------------------------------------------------------
    //! テクスチャバッファの作成
    //-----------------------------------------------------------------------
    ID3D12Resource* blackBuff;
    if(FAILED(d3dDevice()->CreateCommittedResource(
           &texHeapProp,
           D3D12_HEAP_FLAG_NONE,
           &resDesc,
           D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
           nullptr,
           IID_PPV_ARGS(&blackBuff)))) {
        return false;
    }

    std::vector<unsigned char> data(4 * 4 * 4);
    std::fill(data.begin(), data.end(), 0x00);

    if(FAILED(blackBuff->WriteToSubresource(
           0,
           nullptr,
           data.data(),
           4 * 4,
           data.size()))) {
        return false;
    }

    return blackBuff;
}

//---------------------------------------------------------------------------
//! トゥーンのためのグラデーションテクスチャ
//! @return  グラデーションバッファ
//---------------------------------------------------------------------------
ID3D12Resource* CreateGrayGradationTexture()
{
    //----------------------------------------------------------
    // グラデーションテクスチャバッファの作成
    //----------------------------------------------------------
    // テクスチャバッファ用のヒーププロパティ作成
    auto texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
    // テクスチャバッファ用のリソース詳細設定
    auto resDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, 4, 256);

    // グラデーションテクスチャバッファの作成
    ID3D12Resource* gradBuff;
    if(FAILED(d3dDevice()->CreateCommittedResource(
           &texHeapProp,
           D3D12_HEAP_FLAG_NONE,
           &resDesc,
           D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
           nullptr,
           IID_PPV_ARGS(&gradBuff)))) {
        return false;
    }

    //----------------------------------------------------------
    // 上が白くて下が黒いテクスチャデータを作成
    //----------------------------------------------------------
    std::vector<unsigned int> data(4 * 256);
    auto                      it = data.begin();
    unsigned int              c  = 0xff;
    for(; it != data.end(); it += 4) {
        auto col = (c << 0xff) | (c << 16) | (c << 8) | c;
        std::fill(it, it + 4, col);
        --c;
    }

    //----------------------------------------------------------
    // WriteToSubresourceの作成
    // テクスチャ―情報(texbuff)をGPUに転送
    //----------------------------------------------------------
    if(FAILED(gradBuff->WriteToSubresource(
           0,
           nullptr,
           data.data(),
           4 * sizeof(unsigned int),
           sizeof(unsigned int) * data.size()))) {
        return false;
    }

    return gradBuff;
}

//---------------------------------------------------------------------------
//! ファイル名から拡張子を取得する
//! @param  path 対象のパス文字列
//! @return      拡張子
//---------------------------------------------------------------------------
std::string GetExtension(const std::string& path)
{
    int idx = path.rfind('.');
    return path.substr(idx + 1, path.length() - idx - 1);
}

//---------------------------------------------------------------------------
//! ファイル名から拡張子を取得する(ワイド文字版)
//! @param  path 対象のパス文字列
//! @return      拡張子
//---------------------------------------------------------------------------
std::wstring GetExtension(const std::wstring& path)
{
    int idx = path.rfind(L'.');
    return path.substr(idx + 1, path.length() - idx - 1);
}

//---------------------------------------------------------------------------
//! モデルのパスとテクスチャのパスから合成パスを得る
//! @param  modelPath アプリケーションから見たpmdモデルのパス
//! @param  texPath 　PMDモデルから見たテクスチャのパス
//! @return           アプリケーションから見たテクスチャのパス
//---------------------------------------------------------------------------
std::string GetTexturePathFromModelAndTexPath(const std::string& modelPath, const char* texPath)
{
    //ファイルのフォルダ区切りは\と/の二種類が使用される可能性があり
    //ともかく末尾の\か/を得られればいいので、双方のrfindをとり比較する
    //int型に代入しているのは見つからなかった場合はrfindがepos(-1→0xffffffff)を返すため
    int  pathIndex1 = modelPath.rfind('/');
    int  pathIndex2 = modelPath.rfind('\\');
    auto pathIndex  = std::max(pathIndex1, pathIndex2);
    auto folderPath = modelPath.substr(0, pathIndex + 1);
    return folderPath + texPath;
}

//---------------------------------------------------------------------------
//! テクスチャのパスをセパレータ文字で分離する
//! @param  path     対象のパス文字列
//! @param  splitter 区切り文字
//! @return          分離前後の文字列ペア
//---------------------------------------------------------------------------
std::pair<std::string, std::string> SplitFileName(const std::string& path, const char splitter)
{
    int                                 idx = path.find(splitter);
    std::pair<std::string, std::string> ret;
    ret.first  = path.substr(0, idx);
    ret.second = path.substr(idx + 1, path.length() - idx - 1);
    return ret;
}

//---------------------------------------------------------------------------
//! std::string(マルチバイト文字列) から wstring(ワイド文字列)を得る
//! @param  str マルチバイト文字列
//! @retrun     変換されたワイド文字列
//---------------------------------------------------------------------------
std::wstring GetWideStringFromString(const std::string& str)
{
    // 文字列数を取得
    auto num1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);

    std::wstring wstr;
    wstr.resize(num1);

    // 確保済みのwstrに変換文字列をコピー
    auto num2 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, &wstr[0], num1);

    assert(num1 == num2);   // 一応チェック
    return wstr;
}

//---------------------------------------------------------------------------
//! Z軸を特定の方向を向かす行列を返す関数
//! @param lookat 向かせたい方向ベクトル
//! @param up     上ベクトル
//! @param right  右ベクトル
//! @return Z軸が特定の向いた行列
//---------------------------------------------------------------------------
XMMATRIX LookAtMatrix(const XMVECTOR& lookat, XMFLOAT3& up, XMFLOAT3& right)
{
    //向かせたい方向(z軸)
    XMVECTOR vz = lookat;

    //(向かせたい方向を向かせたときの)仮のy軸ベクトル
    XMVECTOR vy = XMVector3Normalize(XMLoadFloat3(&up));

    //(向かせたい方向を向かせたときの)y軸
    //XMVECTOR vx = XMVector3Normalize(XMVector3Cross(vz, vx));
    XMVECTOR vx = XMVector3Normalize(XMVector3Cross(vy, vz));
    vy          = XMVector3Normalize(XMVector3Cross(vz, vx));

    ///LookAtとupが同じ方向を向いてたらright基準で作り直す
    if(abs(XMVector3Dot(vy, vz).m128_f32[0]) == 1.0f) {
        //仮のX方向を定義
        vx = XMVector3Normalize(XMLoadFloat3(&right));
        //向かせたい方向を向かせたときのY軸を計算
        vy = XMVector3Normalize(XMVector3Cross(vz, vx));
        //真のX軸を計算
        vx = XMVector3Normalize(XMVector3Cross(vy, vz));
    }
    XMMATRIX ret = XMMatrixIdentity();
    ret.r[0]     = vx;
    ret.r[1]     = vy;
    ret.r[2]     = vz;
    return ret;
}

//---------------------------------------------------------------------------
//! 特定のベクトルを特定の方向に向けるための行列を返す
//! @param origin 特定のベクトル
//! @param lookat 向かせたい方向
//! @param up     上ベクトル
//! @param right  右ベクトル
//! @retval 特定のベクトルを特定の方向に向けるための行列
//---------------------------------------------------------------------------
XMMATRIX LookAtMatrix(const XMVECTOR& origin, const XMVECTOR& lookat, XMFLOAT3& up, XMFLOAT3& right)
{
    return XMMatrixTranspose(LookAtMatrix(origin, up, right)) *
           LookAtMatrix(lookat, up, right);
}

}