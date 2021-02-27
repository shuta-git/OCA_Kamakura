//===========================================================================
//! @file   GpuInputLayout.h
//!	@brief	���̓��C�A�E�g
//===========================================================================
#pragma once

namespace gpu {

class ShaderVs;

class InputLayout
{
public:
    template<int N>
    InputLayout(const D3D11_INPUT_ELEMENT_DESC (&desc)[N])
    {
        desc_.resize(N);
        for(u32 i = 0; i < N; ++i) {
            desc_[i] = desc[i];
        }
    }

    //! �f�X�g���N�^
    virtual ~InputLayout();

    //! D3D���̓��C�A�E�g���擾
    //!	@param [in]	shaderVs	�֘A���钸�_�V�F�[�_�[
    ID3D11InputLayout* getD3DInputLayout(const gpu::ShaderVs* shaderVs);

private:
    std::vector<D3D11_INPUT_ELEMENT_DESC>                        desc_;
    std::unordered_map<const gpu::ShaderVs*, ID3D11InputLayout*> inputLayout_;
};

}   // namespace gpu
