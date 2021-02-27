//===========================================================================
//! @file   GpuInputLayout.cpp
//!	@brief	入力レイアウト
//===========================================================================
namespace gpu {

//---------------------------------------------------------------------------
//! デストラクタ
//---------------------------------------------------------------------------
InputLayout::~InputLayout()
{
    //! ハッシュテーブルに登録されているD3D入力レイアウトをすべて解放
    for(auto pair : inputLayout_) {
        ID3D11InputLayout* p = pair.second;
        p->Release();
    }
}

//---------------------------------------------------------------------------
//! D3D入力レイアウトを取得
//---------------------------------------------------------------------------
ID3D11InputLayout* InputLayout::getD3DInputLayout(const gpu::ShaderVs* shaderVs)
{
    //! 頂点シェーダーに関連付けられたD3D入力レイアウトがあればそのまま利用
    auto d3dInputLauout = inputLayout_[shaderVs];
    if(d3dInputLauout) {
        return d3dInputLauout;
    }

    //! なければ新たに生成して記録
    //------------------------------------------------------------
    //! 入力レイアウトの作成
    //------------------------------------------------------------
    D3DDevice()->CreateInputLayout(
        desc_.data(),                      //! D3D11_INPUT_ELEMENT_DESCの先頭
        (UINT)desc_.size(),                //! 要素数
        shaderVs->getShaderBinary(),       //! 頂点シェーダーのシェーダーバイナリー
        shaderVs->getShaderBinarySize(),   //! 頂点シェーダーのバイナリーサイズ
        &d3dInputLauout);
    //! 記録
    inputLayout_[shaderVs] = d3dInputLauout;

    return d3dInputLauout;
}
}   // namespace gpu
