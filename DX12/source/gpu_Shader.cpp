//---------------------------------------------------------------------------
//!	@file	gpu_Shader.cpp
//!	@brief	GPU�V�F�[�_�[
//---------------------------------------------------------------------------

namespace gpu {

//===========================================================================
//! �V�F�[�_�[��������
//===========================================================================
class ShaderImpl final : public gpu::Shader
{
public:
    // �V�F�[�_�[���
    enum class Type
    {
        Unknown,    //!< ����`
        Vs,         //!< ���_�V�F�[�_�[
        Ps,         //!< �s�N�Z���V�F�[�_�[
        Cs,         //!< ���Z�V�F�[�_�[�i�R���s���[�g�V�F�[�_�[�j
    };

    //! �R���X�g���N�^
    ShaderImpl() = default;

    //! �f�X�g���N�^
    virtual ~ShaderImpl() = default;

    //! ������
    //!@ param [in] byteCode        �V�F�[�_�[�o�C�g�R�[�h�̐擪�A�h���X
    //!@ param [in] byteCodeSize    �V�F�[�_�[�o�C�g�R�[�h�̃T�C�Y(�P��:bytes)
    bool initialize(void* byteCode, size_t byteCodeSize);

    //! �V�F�[�_�[���t���N�V���������\�z
    // ���͏����Ȃ�

    //----------------------------------------------------------
    //! @name �L���X�g
    //----------------------------------------------------------
    //@{

    //! D3D12�o�C�g�R�[�h��`
    virtual operator D3D12_SHADER_BYTECODE() const override
    {
        return { byteCode_.data(), byteCode_.size() };
    }

    //@}
    //----------------------------------------------------------
    //! @name �Q��
    //----------------------------------------------------------
    //@{

    //! �V�F�[�_�[�o�C�g�R�[�h���擾
    virtual const void* byteCode() const override
    {
        return byteCode_.data();
    }

    //! �V�F�[�_�[�o�C�g�R�[�h�̃T�C�Y���擾
    virtual size_t byteCodeSize() const override
    {
        return byteCode_.size();
    }

    //@}

private:
    // �R�s�[�֎~/move�֎~
    ShaderImpl(const ShaderImpl&) = delete;
    ShaderImpl(ShaderImpl&&)      = delete;
    ShaderImpl& operator=(const ShaderImpl&) = delete;
    ShaderImpl& operator=(ShaderImpl&&) = delete;

private:
    Type                   type_ = Type::Unknown;   //!< �V�F�[�_�[�̎��
    std::vector<std::byte> byteCode_;               //!< �V�F�[�_�[�o�C�g�R�[�h
};

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool ShaderImpl::initialize(void* byteCode, size_t byteCodeSize)
{
    //----------------------------------------------------------
    // �o�C�g�R�[�h�ۑ�
    //----------------------------------------------------------
    byteCode_.resize(byteCodeSize);
    memcpy(byteCode_.data(), byteCode, byteCodeSize);

    // �����ŃV�F�[�_�[���t���N�V���������\�z
    // ���͏����Ȃ�

    return true;
}

//---------------------------------------------------------------------------
//! �V�F�[�_�[���R���p�C��
//! @param  [in]    path    �t�@�C���p�X
//! @param  [in]    entrypoint    �֐���
//! @param  [in]    target    �V�F�[�_�[���f���� "vs_5_1" "ps_5_1" etc...
//---------------------------------------------------------------------------
std::shared_ptr<gpu::Shader> createShader(const char* path,const char* entrypoint, const char* target)
{
    assert(path);
    assert(entrypoint);
    assert(target);

    //----------------------------------------------------------
    // �V�F�[�_�[���R���p�C��
    //----------------------------------------------------------
    u32 compileFlags = D3DCOMPILE_OPTIMIZATION_LEVEL3;

    // HLSL 5.1 ���g�p�������I�C���f�b�N�X�쐬
    // �X���b�g�̔z��A�N�Z�X�ɂ͂��̃I�v�V�������K�v
    // https://docs.microsoft.com/ja-jp/windows/win32/direct3d12/dynamic-indexing-using-hlsl-5-1
    compileFlags |= D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES;   // ShaderModel5.1�p

#if defined(_DEBUG)
    // GraphicDebugging�c�[���p�ɃV�F�[�_�[�f�o�b�O�����ݒ�
    compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    const D3D_SHADER_MACRO* defines = nullptr;

    com_ptr<ID3DBlob> byteCode = nullptr;
    com_ptr<ID3DBlob> errors;

    D3DCompileFromFile(
        string::convertUtf8ToUnicode(path).c_str(),     // [in]�@�t�@�C���p�X
        defines,                                        // [in]  �v���v���Z�b�T�}�N����`
        D3D_COMPILE_STANDARD_FILE_INCLUDE,              // [in]  �J�X�^���C���N���[�h����(D3D_COMPILE_STANDARD_FILE_INCLUD�w��Ńf�t�H���g����)
        entrypoint,                                     // [in]  �֐���
        target,                                         // [in]  �V�F�[�_�[���f����
        compileFlags,                                   // [in]  �R���p�C���t���O  (D3DCOMPILE_xxxx)
        0,                                              // [in]  �R���p�C���t���O�Q(D3DCOMPILE_FLAGS2_xxxx)
        &byteCode,                                      // [out] �R���p�C�����ꂽ�o�C�g�R�[�h
        &errors                                         // [out] �G���[���b�Z�[�W
    );

    // �G���[�o��
    if(errors != nullptr) {
        MessageBox(application::hwnd(), static_cast<char*>(errors->GetBufferPointer()), path, MB_OK);
        OutputDebugStringA((char*)errors->GetBufferPointer());
        return nullptr;
    }

    //----------------------------------------------------------
    // �N���X��������
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