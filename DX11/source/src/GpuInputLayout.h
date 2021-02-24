//===========================================================================
//! @file   GpuInputLayout.h
//!	@brief	入力レイアウト
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

    //! デストラクタ
    virtual ~InputLayout();

    //! D3D入力レイアウトを取得
    //!	@param [in]	shaderVs	関連する頂点シェーダー
    ID3D11InputLayout* getD3DInputLayout(const gpu::ShaderVs* shaderVs);

private:
    std::vector<D3D11_INPUT_ELEMENT_DESC>                        desc_;
    std::unordered_map<const gpu::ShaderVs*, ID3D11InputLayout*> inputLayout_;
};

}   // namespace gpu
