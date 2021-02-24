//===========================================================================
//! @file   GpuShader.h
//!	@brief	�V�F�[�_�[
//===========================================================================
#pragma once

namespace gpu {
//===========================================================================
//! �V�F�[�_�[���
//===========================================================================
class ShaderBase : public Resource
{
public:
    //! �R���X�g���N�^
    ShaderBase() = default;

    //! �f�X�g���N�^
    virtual ~ShaderBase() = default;

    //! �V�F�[�_�[�o�C�i���̃A�h���X���擾
    const void* getShaderBinary() const;

    //! �V�F�[�_�[�o�C�i���̃T�C�Y���擾
    size_t getShaderBinarySize() const;

protected:
    com_ptr<ID3DBlob> shaderBinary_;
};

//===========================================================================
//! ���_�V�F�[�_�[
//===========================================================================
class ShaderVs : public ShaderBase
{
public:
    //! �R���X�g���N�^
    ShaderVs(const std::wstring& path);

    //! �f�X�g���N�^
    virtual ~ShaderVs() = default;

    //! D3D�V�F�[�_�[���擾
    ID3D11VertexShader* get() const { return shader_.Get(); }

    //! �V�F�[�_�[�̍쐬
    static std::unique_ptr<gpu::ShaderVs> create(const char* fileName);

    //! ���\�[�X�̃����[�h
    virtual bool reload() override;

private:
    com_ptr<ID3D11VertexShader> shader_;
};

//===========================================================================
//! �s�N�Z���V�F�[�_�[
//===========================================================================
class ShaderPs : public ShaderBase
{
public:
    //! �R���X�g���N�^
    ShaderPs(const std::wstring& path);

    //! �f�X�g���N�^
    virtual ~ShaderPs() = default;

    //! D3D�V�F�[�_�[���擾
    ID3D11PixelShader* get() const { return shader_.Get(); }

    //! �V�F�[�_�[�̍쐬
    static std::unique_ptr<gpu::ShaderPs> create(const char* fileName);

    //! ���\�[�X�̃����[�h
    virtual bool reload() override;

private:
    com_ptr<ID3D11PixelShader> shader_;
};

//===========================================================================
//! ���Z�V�F�[�_�[
//===========================================================================
class ShaderCs : public ShaderBase
{
public:
    //! �R���X�g���N�^
    ShaderCs(const std::wstring& path);

    //! �f�X�g���N�^
    virtual ~ShaderCs() = default;

    //! D3D�V�F�[�_�[���擾
    ID3D11ComputeShader* get() const { return shader_.Get(); }

    //! �V�F�[�_�[�̍쐬
    static std::unique_ptr<gpu::ShaderCs> create(const char* fileName);

    //! ���\�[�X�̃����[�h
    virtual bool reload() override;

private:
    com_ptr<ID3D11ComputeShader> shader_;
};

}   // namespace gpu
