#pragma once
//---------------------------------------------------------------------------
//!	@file	gpu_Shader.h
//!	@brief	GPUシェーダー
//---------------------------------------------------------------------------

namespace gpu {

class Shader
{
public:
    virtual ~Shader() = default;

    //----------------------------------------------------------
    //! @name キャスト
    //----------------------------------------------------------
    //@{

    //! D3D12バイトコード定義
    virtual operator D3D12_SHADER_BYTECODE() const = 0;

    //@}
    //----------------------------------------------------------
    //! @name 参照
    //----------------------------------------------------------
    //@{

    //! シェーダーバイトコードを取得
    virtual const void* byteCode() const = 0;

    //! シェーダーバイトコードのサイズを取得
    virtual size_t byteCodeSize() const = 0;

    //@}

protected:
    // new禁止
    Shader()          = default;
};
    
//! シェーダーをコンパイル
//! @param  [in]    path        ファイルパス
//! @param  [in]    entrypoint  関数名
//! @param  [in]    target      シェーダーモデル名 "vs_5_1" "ps_5_1" etc...
std::shared_ptr<gpu::Shader> createShader(const char* path, const char* entrypoint, const char* target);


}   // namespace gpu