//===========================================================================
//! @file   GpuShader.h
//!	@brief	シェーダー
//===========================================================================
#pragma once

namespace gpu {
//===========================================================================
//! シェーダー基底
//===========================================================================
class ShaderBase : public Resource
{
public:
    //! コンストラクタ
    ShaderBase() = default;

    //! デストラクタ
    virtual ~ShaderBase() = default;

    //! シェーダーバイナリのアドレスを取得
    const void* getShaderBinary() const;

    //! シェーダーバイナリのサイズを取得
    size_t getShaderBinarySize() const;

protected:
    com_ptr<ID3DBlob> shaderBinary_;
};

//===========================================================================
//! 頂点シェーダー
//===========================================================================
class ShaderVs : public ShaderBase
{
public:
    //! コンストラクタ
    ShaderVs(const std::wstring& path);

    //! デストラクタ
    virtual ~ShaderVs() = default;

    //! D3Dシェーダーを取得
    ID3D11VertexShader* get() const { return shader_.Get(); }

    //! シェーダーの作成
    static std::unique_ptr<gpu::ShaderVs> create(const char* fileName);

    //! リソースのリロード
    virtual bool reload() override;

private:
    com_ptr<ID3D11VertexShader> shader_;
};

//===========================================================================
//! ピクセルシェーダー
//===========================================================================
class ShaderPs : public ShaderBase
{
public:
    //! コンストラクタ
    ShaderPs(const std::wstring& path);

    //! デストラクタ
    virtual ~ShaderPs() = default;

    //! D3Dシェーダーを取得
    ID3D11PixelShader* get() const { return shader_.Get(); }

    //! シェーダーの作成
    static std::unique_ptr<gpu::ShaderPs> create(const char* fileName);

    //! リソースのリロード
    virtual bool reload() override;

private:
    com_ptr<ID3D11PixelShader> shader_;
};

//===========================================================================
//! 演算シェーダー
//===========================================================================
class ShaderCs : public ShaderBase
{
public:
    //! コンストラクタ
    ShaderCs(const std::wstring& path);

    //! デストラクタ
    virtual ~ShaderCs() = default;

    //! D3Dシェーダーを取得
    ID3D11ComputeShader* get() const { return shader_.Get(); }

    //! シェーダーの作成
    static std::unique_ptr<gpu::ShaderCs> create(const char* fileName);

    //! リソースのリロード
    virtual bool reload() override;

private:
    com_ptr<ID3D11ComputeShader> shader_;
};

}   // namespace gpu
