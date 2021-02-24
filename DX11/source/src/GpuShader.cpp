//===========================================================================
//! @file   GpuShader.cpp
//!	@brief	�V�F�[�_�[
//===========================================================================
namespace gpu {

//---------------------------------------------------------------------------
//!	HLSL�V�F�[�_�[���R���p�C��
//!	@param	[in]	fileName		�t�@�C����
//!	@param	[in]	functionName	���s�J�n�֐���
//!	@param	[in]	shaderModel		�V�F�[�_�[���f��������("vs_4_0", "ps_4_0", "cs_5_0" etc...)
//!	@return	�V�F�[�_�[�o�C�i���I�u�W�F�N�g(nullptr�Ȃ玸�s)
//---------------------------------------------------------------------------
ID3DBlob* compileShaderFromFile(const wchar_t* fileName, const char* functionName, const char* shaderModel)
{
    //-------------------------------------------------------------
    //	HLSL�V�F�[�_�[�\�[�X�R�[�h���������ɓǂݍ���
    //-------------------------------------------------------------
    std::vector<char> shaderSource;

    FILE* fp;
    if(_wfopen_s(&fp, fileName, L"rb") != 0) {
        return nullptr;
    }

    //---- �t�@�C���T�C�Y���擾
    fseek(fp, 0, SEEK_END);
    size_t fileSize = ftell(fp);   // �t�@�C���T�C�Y
    fseek(fp, 0, SEEK_SET);

    //---- ���[�N�̈���m�ۂ��ēǂݍ���
    shaderSource.resize(fileSize);   // ���[�N�̈�̊m��
    if(shaderSource.empty()) {
        fclose(fp);
        return false;
    }
    fread(&shaderSource[0], fileSize, 1, fp);   // �ǂݍ���
    fclose(fp);

    //-------------------------------------------------------------
    // �V�F�[�_�[�R���p�C���[�̃I�v�V�����ݒ�
    //-------------------------------------------------------------
    u32 flags = D3DCOMPILE_ENABLE_STRICTNESS;

    //---- �s��̗�D��E�s�D��
    //	flags |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;		// �s�D��
    flags |= D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;   // ��D��(�]�u)

    //---- ����R�[�h�̐�������(if, for���Ȃ�)
    //	flags |= D3DCOMPILE_AVOID_FLOW_CONTROL;			// ���򂵂Ȃ��t���b�g�ȃR�[�h�𐶐�
    flags |= D3DCOMPILE_PREFER_FLOW_CONTROL;   // ���I������܂ރR�[�h�𐶐�

    //---- �œK�����׃��i�V�F�[�_�[���\�ƃR���p�C�����Ԃɉe���j
    //	flags |= D3DCOMPILE_OPTIMIZATION_LEVEL0;		// �œK���Ȃ�
    //	flags |= D3DCOMPILE_OPTIMIZATION_LEVEL1;		// ��
    //	flags |= D3DCOMPILE_OPTIMIZATION_LEVEL2;		// ��
    flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;   // �ő���̍œK��

#if 0
	//---- �V�F�[�_�[�Ƀf�o�b�O����t��
	// ������	Release�łɂ͊܂߂Ȃ�����
	flags |= D3DCOMPILE_DEBUG;
#endif
    //-------------------------------------------------------------
    // HLSL�\�[�X�R�[�h����define�}�N����`
    //-------------------------------------------------------------
    std::array<D3D_SHADER_MACRO, 2> defineMacros = { { { "__WINDOWS__", "1" },
                                                       //---- �I�[��nullptr�w�肵�Ă���
                                                       { nullptr, nullptr } } };

    //-------------------------------------------------------------
    // HLSL�V�F�[�_�[���R���p�C��
    //-------------------------------------------------------------
    HRESULT           hr;
    ID3DBlob*         shaderBlob;   // �V�F�[�_�[�o�C�i��
    com_ptr<ID3DBlob> errorBlob;    // �G���[���b�Z�[�W�o�͗p

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
    // �R���p�C���G���[�̏ꍇ�̓G���[���b�Z�[�W�̓��e���o��
    //-------------------------------------------------------------
    if(errorBlob) {
        // VisualStudio �u�o�́v�E�B���h�E�ɕ\��
        OutputDebugString((const char*)errorBlob->GetBufferPointer());

        // �|�b�v�A�b�v�ł��\��
        MessageBox(nullptr, (const char*)errorBlob->GetBufferPointer(), "�V�F�[�_�[�R���p�C�� �G���[", MB_OK);
    }

    return shaderBlob;
}

//===========================================================================
//! �V�F�[�_�[���
//===========================================================================

//---------------------------------------------------------------------------
//! �V�F�[�_�[�o�C�i���̃A�h���X���擾
//---------------------------------------------------------------------------
const void* ShaderBase::getShaderBinary() const
{
    return shaderBinary_->GetBufferPointer();
}

//---------------------------------------------------------------------------
//! �V�F�[�_�[�o�C�i���̃T�C�Y���擾
//---------------------------------------------------------------------------
size_t ShaderBase::getShaderBinarySize() const
{
    return shaderBinary_->GetBufferSize();
}

//===========================================================================
//! ���_�V�F�[�_�[
//===========================================================================

//---------------------------------------------------------------------------
//! �R���X�g���N�^
//---------------------------------------------------------------------------
ShaderVs::ShaderVs(const std::wstring& path)
{
    setPath(path);
    reload();
}

//---------------------------------------------------------------------------
//! �V�F�[�_�[�̍쐬
//---------------------------------------------------------------------------
std::unique_ptr<gpu::ShaderVs> ShaderVs::create(const char* fileName)
{
    std::wstring resourcePath = makeResourcePath(fileName);
    return std::make_unique<gpu::ShaderVs>(resourcePath);
}

//---------------------------------------------------------------------------
//! ���\�[�X�̃����[�h
//---------------------------------------------------------------------------
bool ShaderVs::reload()
{
    std::wstring path = std::wstring(L"data/") + getPath();

    auto shaderBinary = compileShaderFromFile(path.c_str(), "main", "vs_5_0");   // ���_�V�F�[�_�[
    if(shaderBinary == nullptr) {
        return nullptr;
    }

    shader_.Reset();

    // �V�F�[�_�[�̍쐬
    D3DDevice()->CreateVertexShader(shaderBinary->GetBufferPointer(),
                                    shaderBinary->GetBufferSize(),
                                    nullptr,
                                    &shader_);
    shaderBinary_ = shaderBinary;

    return true;
}

//===========================================================================
//! �s�N�Z���V�F�[�_�[
//===========================================================================

//---------------------------------------------------------------------------
//! �R���X�g���N�^
//---------------------------------------------------------------------------
ShaderPs::ShaderPs(const std::wstring& path)
{
    setPath(path);
    reload();
}

//---------------------------------------------------------------------------
//! �V�F�[�_�[�̍쐬
//---------------------------------------------------------------------------
std::unique_ptr<gpu::ShaderPs> ShaderPs::create(const char* fileName)
{
    std::wstring resourcePath = makeResourcePath(fileName);
    return std::make_unique<gpu::ShaderPs>(resourcePath);
}

//---------------------------------------------------------------------------
//! ���\�[�X�̃����[�h
//---------------------------------------------------------------------------
bool ShaderPs::reload()
{
    std::wstring path = std::wstring(L"data/") + getPath();

    auto shaderBinary = compileShaderFromFile(path.c_str(), "main", "ps_5_0");   // �s�N�Z���V�F�[�_�[
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
//! ���Z�V�F�[�_�[
//===========================================================================

//---------------------------------------------------------------------------
//! �R���X�g���N�^
//---------------------------------------------------------------------------
ShaderCs::ShaderCs(const std::wstring& path)
{
    setPath(path);
    reload();
}

//---------------------------------------------------------------------------
//! �V�F�[�_�[�̍쐬
//---------------------------------------------------------------------------
std::unique_ptr<gpu::ShaderCs> ShaderCs::create(const char* fileName)
{
    std::wstring resourcePath = makeResourcePath(fileName);
    return std::make_unique<gpu::ShaderCs>(resourcePath);
}

//---------------------------------------------------------------------------
//! ���\�[�X�̃����[�h
//---------------------------------------------------------------------------
bool ShaderCs::reload()
{
    std::wstring path = std::wstring(L"data/") + getPath();

    auto shaderBinary = compileShaderFromFile(path.c_str(), "main", "cs_5_0");   // ���Z�V�F�[�_�[
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
