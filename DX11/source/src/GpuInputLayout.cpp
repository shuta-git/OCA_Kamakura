//===========================================================================
//! @file   GpuInputLayout.cpp
//!	@brief	���̓��C�A�E�g
//===========================================================================
namespace gpu {

//---------------------------------------------------------------------------
//! �f�X�g���N�^
//---------------------------------------------------------------------------
InputLayout::~InputLayout()
{
    //! �n�b�V���e�[�u���ɓo�^����Ă���D3D���̓��C�A�E�g�����ׂĉ��
    for(auto pair : inputLayout_) {
        ID3D11InputLayout* p = pair.second;
        p->Release();
    }
}

//---------------------------------------------------------------------------
//! D3D���̓��C�A�E�g���擾
//---------------------------------------------------------------------------
ID3D11InputLayout* InputLayout::getD3DInputLayout(const gpu::ShaderVs* shaderVs)
{
    //! ���_�V�F�[�_�[�Ɋ֘A�t����ꂽD3D���̓��C�A�E�g������΂��̂܂ܗ��p
    auto d3dInputLauout = inputLayout_[shaderVs];
    if(d3dInputLauout) {
        return d3dInputLauout;
    }

    //! �Ȃ���ΐV���ɐ������ċL�^
    //------------------------------------------------------------
    //! ���̓��C�A�E�g�̍쐬
    //------------------------------------------------------------
    D3DDevice()->CreateInputLayout(
        desc_.data(),                      //! D3D11_INPUT_ELEMENT_DESC�̐擪
        (UINT)desc_.size(),                //! �v�f��
        shaderVs->getShaderBinary(),       //! ���_�V�F�[�_�[�̃V�F�[�_�[�o�C�i���[
        shaderVs->getShaderBinarySize(),   //! ���_�V�F�[�_�[�̃o�C�i���[�T�C�Y
        &d3dInputLauout);
    //! �L�^
    inputLayout_[shaderVs] = d3dInputLauout;

    return d3dInputLauout;
}
}   // namespace gpu
