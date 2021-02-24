//===========================================================================
//!	@file   PassFilterHDR.cpp
//!	@brief	HDRフィルターパス
//===========================================================================
#include "PassFilterHDR.h"

//---------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------
bool PassFilterHDR::initialize()
{
    // シェーダーの読み込み
    shaderTonemappingVs_ = gpu::ShaderVs::create("shader/vsPrim2D.fx");
    shaderTonemappingPs_ = gpu::ShaderPs::create("shader/psTonemapping.fx");

    shaderVs_integrate_ = gpu::ShaderVs::create("shader/vsPrim2D.fx");
    if(!shaderVs_integrate_) {
        return false;
    }

    shaderPs_integrate_ = gpu::ShaderPs::create("shader/psIntegrate.fx");
    if(!shaderPs_integrate_) {
        return false;
    }

    //-------------------------------------------------------------
    // フィルター初期化
    //-------------------------------------------------------------
    if(filterGlare_.initialize() == false) {
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------
//!	解放
//---------------------------------------------------------------------------
void PassFilterHDR::finalize()
{
    shaderVs_integrate_.reset();
    shaderPs_integrate_.reset();
}

//---------------------------------------------------------------------------
//!	開始
//---------------------------------------------------------------------------
void PassFilterHDR::begin()
{
	gpu::setRenderTarget(GmRender()->getRenderBuffer(), nullptr);

    //=============================================================
    // Diffuse/SpecularバッファをHDRバッファへ合成
    //=============================================================
	gpu::setTexture(0, GmRender()->getDiffuseTexture());
	gpu::setTexture(1, GmRender()->getSpecularTexture());
	
	gpu::setShader(shaderVs_integrate_, shaderPs_integrate_);
    GmRender()->put();
}

//---------------------------------------------------------------------------
//!	実行
//---------------------------------------------------------------------------
void PassFilterHDR::execute()
{
    filterGlare_.render();
}

//---------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------
void PassFilterHDR::end()
{
    //-------------------------------------------------------------
    // トーンマッピングを適用
    //-------------------------------------------------------------
    auto hdrBuffer = GmRender()->getRenderBuffer();

    GmRender()->setHDR(false);   // ここからSDR

    // SDRバックバッファを設定
    gpu::setRenderTarget(GmRender()->getRenderBuffer());

    dxTexture(hdrBuffer);

    // トーンマッピングフィルターシェーダー設定
    gpu::setShader(shaderTonemappingVs_, shaderTonemappingPs_);

    GmRender()->put();
}
