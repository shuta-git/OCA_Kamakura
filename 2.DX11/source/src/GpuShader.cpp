//===========================================================================
//! @file   GpuShader.cpp
//!	@brief	シェーダー
//===========================================================================
namespace gpu {

//---------------------------------------------------------------------------
//!	HLSLシェーダーをコンパイル
//!	@param	[in]	fileName		ファイル名
//!	@param	[in]	functionName	実行開始関数名
//!	@param	[in]	shaderModel		シェーダーモデル文字列("vs_4_0", "ps_4_0", "cs_5_0" etc...)
//!	@return	シェーダーバイナリオブジェクト(nullptrなら失敗)
//---------------------------------------------------------------------------
ID3DBlob* compileShaderFromFile(const wchar_t* fileName, const char* functionName, const char* shaderModel)
{
    //-------------------------------------------------------------
    //	HLSLシェーダーソースコードをメモリに読み込み
    //-------------------------------------------------------------
    std::vector<char> shaderSource;

    FILE* fp;
    if(_wfopen_s(&fp, fileName, L"rb") != 0) {
        return nullptr;
    }

    //---- ファイルサイズを取得
    fseek(fp, 0, SEEK_END);
    size_t fileSize = ftell(fp);   // ファイルサイズ
    fseek(fp, 0, SEEK_SET);

    //---- ワーク領域を確保して読み込み
    shaderSource.resize(fileSize);   // ワーク領域の確保
    if(shaderSource.empty()) {
        fclose(fp);
        return false;
    }
    fread(&shaderSource[0], fileSize, 1, fp);   // 読み込み
    fclose(fp);

    //-------------------------------------------------------------
    // シェーダーコンパイラーのオプション設定
    //-------------------------------------------------------------
    u32 flags = D3DCOMPILE_ENABLE_STRICTNESS;

    //---- 行列の列優先・行優先
    //	flags |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;		// 行優先
    flags |= D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;   // 列優先(転置)

    //---- 分岐コードの生成制御(if, for文など)
    //	flags |= D3DCOMPILE_AVOID_FLOW_CONTROL;			// 分岐しないフラットなコードを生成
    flags |= D3DCOMPILE_PREFER_FLOW_CONTROL;   // 動的分岐を含むコードを生成

    //---- 最適化レべル（シェーダー性能とコンパイル時間に影響）
    //	flags |= D3DCOMPILE_OPTIMIZATION_LEVEL0;		// 最適化なし
    //	flags |= D3DCOMPILE_OPTIMIZATION_LEVEL1;		// ↑
    //	flags |= D3DCOMPILE_OPTIMIZATION_LEVEL2;		// ↓
    flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;   // 最大限の最適化

#if 0
	//---- シェーダーにデバッグ情報を付加
	// ※注意	Release版には含めないこと
	flags |= D3DCOMPILE_DEBUG;
#endif
    //-------------------------------------------------------------
    // HLSLソースコード内のdefineマクロ定義
    //-------------------------------------------------------------
    std::array<D3D_SHADER_MACRO, 2> defineMacros = { { { "__WINDOWS__", "1" },
                                                       //---- 終端にnullptr指定しておく
                                                       { nullptr, nullptr } } };

    //-------------------------------------------------------------
    // HLSLシェーダーをコンパイル
    //-------------------------------------------------------------
    HRESULT           hr;
    ID3DBlob*         shaderBlob;   // シェーダーバイナリ
    com_ptr<ID3DBlob> errorBlob;    // エラーメッセージ出力用

    hr = D3DCompileFromFile(
        fileName,
        &defineMacros[0],
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        functionName,
        shaderModel,
        flags,
        0,
        &shaderBlob,
        &errorBlob);

    //-------------------------------------------------------------
    // コンパイルエラーの場合はエラーメッセージの内容を出力
    //-------------------------------------------------------------
    if(errorBlob) {
        // VisualStudio 「出力」ウィンドウに表示
        OutputDebugString((const char*)errorBlob->GetBufferPointer());

        // ポップアップでも表示
        MessageBox(nullptr, (const char*)errorBlob->GetBufferPointer(), "シェーダーコンパイル エラー", MB_OK);
    }

    return shaderBlob;
}

//===========================================================================
//! シェーダー基底
//===========================================================================

//---------------------------------------------------------------------------
//! シェーダーバイナリのアドレスを取得
//---------------------------------------------------------------------------
const void* ShaderBase::getShaderBinary() const
{
    return shaderBinary_->GetBufferPointer();
}

//---------------------------------------------------------------------------
//! シェーダーバイナリのサイズを取得
//---------------------------------------------------------------------------
size_t ShaderBase::getShaderBinarySize() const
{
    return shaderBinary_->GetBufferSize();
}

//===========================================================================
//! 頂点シェーダー
//===========================================================================

//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
ShaderVs::ShaderVs(const std::wstring& path)
{
    setPath(path);
    reload();
}

//---------------------------------------------------------------------------
//! シェーダーの作成
//---------------------------------------------------------------------------
std::unique_ptr<gpu::ShaderVs> ShaderVs::create(const char* fileName)
{
    std::wstring resourcePath = makeResourcePath(fileName);
    return std::make_unique<gpu::ShaderVs>(resourcePath);
}

//---------------------------------------------------------------------------
//! リソースのリロード
//---------------------------------------------------------------------------
bool ShaderVs::reload()
{
    std::wstring path = std::wstring(L"data/") + getPath();

    auto shaderBinary = compileShaderFromFile(path.c_str(), "main", "vs_5_0");   // 頂点シェーダー
    if(shaderBinary == nullptr) {
        return nullptr;
    }

    shader_.Reset();

    // シェーダーの作成
    D3DDevice()->CreateVertexShader(shaderBinary->GetBufferPointer(),
                                    shaderBinary->GetBufferSize(),
                                    nullptr,
                                    &shader_);
    shaderBinary_ = shaderBinary;

    return true;
}

//===========================================================================
//! ピクセルシェーダー
//===========================================================================

//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
ShaderPs::ShaderPs(const std::wstring& path)
{
    setPath(path);
    reload();
}

//---------------------------------------------------------------------------
//! シェーダーの作成
//---------------------------------------------------------------------------
std::unique_ptr<gpu::ShaderPs> ShaderPs::create(const char* fileName)
{
    std::wstring resourcePath = makeResourcePath(fileName);
    return std::make_unique<gpu::ShaderPs>(resourcePath);
}

//---------------------------------------------------------------------------
//! リソースのリロード
//---------------------------------------------------------------------------
bool ShaderPs::reload()
{
    std::wstring path = std::wstring(L"data/") + getPath();

    auto shaderBinary = compileShaderFromFile(path.c_str(), "main", "ps_5_0");   // ピクセルシェーダー
    if(shaderBinary == nullptr) {
        return nullptr;
    }
    shader_.Reset();

    D3DDevice()->CreatePixelShader(shaderBinary->GetBufferPointer(),
                                   shaderBinary->GetBufferSize(),
                                   nullptr,
                                   &shader_);
    shaderBinary_ = shaderBinary;

    return true;
}

//===========================================================================
//! 演算シェーダー
//===========================================================================

//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
ShaderCs::ShaderCs(const std::wstring& path)
{
    setPath(path);
    reload();
}

//---------------------------------------------------------------------------
//! シェーダーの作成
//---------------------------------------------------------------------------
std::unique_ptr<gpu::ShaderCs> ShaderCs::create(const char* fileName)
{
    std::wstring resourcePath = makeResourcePath(fileName);
    return std::make_unique<gpu::ShaderCs>(resourcePath);
}

//---------------------------------------------------------------------------
//! リソースのリロード
//---------------------------------------------------------------------------
bool ShaderCs::reload()
{
    std::wstring path = std::wstring(L"data/") + getPath();

    auto shaderBinary = compileShaderFromFile(path.c_str(), "main", "cs_5_0");   // 演算シェーダー
    if(shaderBinary == nullptr) {
        return nullptr;
    }
    shader_.Reset();

    D3DDevice()->CreateComputeShader(shaderBinary->GetBufferPointer(),
                                     shaderBinary->GetBufferSize(),
                                     nullptr,
                                     &shader_);
    shaderBinary_ = shaderBinary;

    return true;
}

}   // namespace gpu
