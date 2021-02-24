//---------------------------------------------------------------------------
//!	@file	gpu_Shader.cpp
//!	@brief	GPUシェーダー
//---------------------------------------------------------------------------

namespace gpu {

//===========================================================================
//! シェーダー内部実装
//===========================================================================
class ShaderImpl final : public gpu::Shader
{
public:
    // シェーダー種類
    enum class Type
    {
        Unknown,    //!< 未定義
        Vs,         //!< 頂点シェーダー
        Ps,         //!< ピクセルシェーダー
        Cs,         //!< 演算シェーダー（コンピュートシェーダー）
    };

    //! コンストラクタ
    ShaderImpl() = default;

    //! デストラクタ
    virtual ~ShaderImpl() = default;

    //! 初期化
    //!@ param [in] byteCode        シェーダーバイトコードの先頭アドレス
    //!@ param [in] byteCodeSize    シェーダーバイトコードのサイズ(単位:bytes)
    bool initialize(void* byteCode, size_t byteCodeSize);

    //! シェーダーリフレクション情報を構築
    // 今は書かない

    //----------------------------------------------------------
    //! @name キャスト
    //----------------------------------------------------------
    //@{

    //! D3D12バイトコード定義
    virtual operator D3D12_SHADER_BYTECODE() const override
    {
        return { byteCode_.data(), byteCode_.size() };
    }

    //@}
    //----------------------------------------------------------
    //! @name 参照
    //----------------------------------------------------------
    //@{

    //! シェーダーバイトコードを取得
    virtual const void* byteCode() const override
    {
        return byteCode_.data();
    }

    //! シェーダーバイトコードのサイズを取得
    virtual size_t byteCodeSize() const override
    {
        return byteCode_.size();
    }

    //@}

private:
    // コピー禁止/move禁止
    ShaderImpl(const ShaderImpl&) = delete;
    ShaderImpl(ShaderImpl&&)      = delete;
    ShaderImpl& operator=(const ShaderImpl&) = delete;
    ShaderImpl& operator=(ShaderImpl&&) = delete;

private:
    Type                   type_ = Type::Unknown;   //!< シェーダーの種類
    std::vector<std::byte> byteCode_;               //!< シェーダーバイトコード
};

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool ShaderImpl::initialize(void* byteCode, size_t byteCodeSize)
{
    //----------------------------------------------------------
    // バイトコード保存
    //----------------------------------------------------------
    byteCode_.resize(byteCodeSize);
    memcpy(byteCode_.data(), byteCode, byteCodeSize);

    // ここでシェーダーリフレクション情報を構築
    // 今は書かない

    return true;
}

//---------------------------------------------------------------------------
//! シェーダーをコンパイル
//! @param  [in]    path    ファイルパス
//! @param  [in]    entrypoint    関数名
//! @param  [in]    target    シェーダーモデル名 "vs_5_1" "ps_5_1" etc...
//---------------------------------------------------------------------------
std::shared_ptr<gpu::Shader> createShader(const char* path,const char* entrypoint, const char* target)
{
    assert(path);
    assert(entrypoint);
    assert(target);

    //----------------------------------------------------------
    // シェーダーをコンパイル
    //----------------------------------------------------------
    u32 compileFlags = D3DCOMPILE_OPTIMIZATION_LEVEL3;

    // HLSL 5.1 を使用した動的インデックス作成
    // スロットの配列アクセスにはこのオプションが必要
    // https://docs.microsoft.com/ja-jp/windows/win32/direct3d12/dynamic-indexing-using-hlsl-5-1
    compileFlags |= D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES;   // ShaderModel5.1用

#if defined(_DEBUG)
    // GraphicDebuggingツール用にシェーダーデバッグ向け設定
    compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    const D3D_SHADER_MACRO* defines = nullptr;

    com_ptr<ID3DBlob> byteCode = nullptr;
    com_ptr<ID3DBlob> errors;

    D3DCompileFromFile(
        string::convertUtf8ToUnicode(path).c_str(),     // [in]　ファイルパス
        defines,                                        // [in]  プリプロセッサマクロ定義
        D3D_COMPILE_STANDARD_FILE_INCLUDE,              // [in]  カスタムインクルード処理(D3D_COMPILE_STANDARD_FILE_INCLUD指定でデフォルト動作)
        entrypoint,                                     // [in]  関数名
        target,                                         // [in]  シェーダーモデル名
        compileFlags,                                   // [in]  コンパイラフラグ  (D3DCOMPILE_xxxx)
        0,                                              // [in]  コンパイラフラグ２(D3DCOMPILE_FLAGS2_xxxx)
        &byteCode,                                      // [out] コンパイルされたバイトコード
        &errors                                         // [out] エラーメッセージ
    );

    // エラー出力
    if(errors != nullptr) {
        MessageBox(application::hwnd(), static_cast<char*>(errors->GetBufferPointer()), path, MB_OK);
        OutputDebugStringA((char*)errors->GetBufferPointer());
        return nullptr;
    }

    //----------------------------------------------------------
    // クラスを初期化
    //----------------------------------------------------------
    auto p = std::make_shared<gpu::ShaderImpl>();
    if(p) {
        if(!p->initialize(byteCode->GetBufferPointer(), byteCode->GetBufferSize())) {
            p.reset();
        }
    }
    return p;
}

}   // namespace gpu