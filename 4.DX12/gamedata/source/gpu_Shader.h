#pragma once
//---------------------------------------------------------------------------
//!	@file	gpu_Shader.h
//!	@brief	GPU�V�F�[�_�[
//---------------------------------------------------------------------------

namespace gpu {

class Shader
{
public:
    virtual ~Shader() = default;

    //----------------------------------------------------------
    //! @name �L���X�g
    //----------------------------------------------------------
    //@{

    //! D3D12�o�C�g�R�[�h��`
    virtual operator D3D12_SHADER_BYTECODE() const = 0;

    //@}
    //----------------------------------------------------------
    //! @name �Q��
    //----------------------------------------------------------
    //@{

    //! �V�F�[�_�[�o�C�g�R�[�h���擾
    virtual const void* byteCode() const = 0;

    //! �V�F�[�_�[�o�C�g�R�[�h�̃T�C�Y���擾
    virtual size_t byteCodeSize() const = 0;

    //@}

protected:
    // new�֎~
    Shader()          = default;
};
    
//! �V�F�[�_�[���R���p�C��
//! @param  [in]    path        �t�@�C���p�X
//! @param  [in]    entrypoint  �֐���
//! @param  [in]    target      �V�F�[�_�[���f���� "vs_5_1" "ps_5_1" etc...
std::shared_ptr<gpu::Shader> createShader(const char* path, const char* entrypoint, const char* target);


}   // namespace gpu