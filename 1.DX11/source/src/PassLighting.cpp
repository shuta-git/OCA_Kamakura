//===========================================================================
//!	@file   PassLighting.cpp
//!	@brief	照明パス
//===========================================================================
#include "PassLighting.h"

//---------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------
bool PassLighting::initialize()
{
    shaderVs_ = gpu::ShaderVs::create("shader/vsPrim2D.fx");
    if(!shaderVs_) {
        return false;
    }

    shaderPs_ = gpu::ShaderPs::create("shader/psLight_Directional.fx");
    if(!shaderPs_) {
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------
//!	解放
//---------------------------------------------------------------------------
void PassLighting::finalize()
{
    shaderVs_.reset();
    shaderPs_.reset();
}

//---------------------------------------------------------------------------
//!	開始
//---------------------------------------------------------------------------
void PassLighting::begin()
{
    std::array<ptr<gpu::Texture>, 2> mrts{
        GmRender()->getDiffuseTexture(),
        GmRender()->getSpecularTexture(),
    };
    gpu::setRenderTarget(mrts.size(), mrts.data(), nullptr);

    gpu::setTexture(9, GmRender()->getDepthStencil());   // デプステクスチャ

    // 光の加算合成のためにブレンド有効
    f32 blendFactor[4]{};
    gpu::D3DContext()->OMSetBlendState(GmRender()->getBlendState(BLEND_MODE_ADD), blendFactor, 0xffffffffUL);
}

//---------------------------------------------------------------------------
//!	実行
//---------------------------------------------------------------------------
void PassLighting::execute()
{
    gpu::setShader(shaderVs_, shaderPs_);
    GmRender()->put();
}

//---------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------
void PassLighting::end()
{
    // もとに戻す
    f32 blendFactor[4]{};
    gpu::D3DContext()->OMSetBlendState(GmRender()->getBlendState(BLEND_MODE_OFF), blendFactor, 0xffffffffUL);
    gpu::setRenderTarget(GmRender()->getRenderBuffer(), GmRender()->getDepthStencil());
}
