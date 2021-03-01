//---------------------------------------------------------------------------
//!	@file	gpu_PipelineState.h
//!	@brief	GPUパイプラインステート
//---------------------------------------------------------------------------
#pragma once

namespace gpu {

//===========================================================================
//! パイプラインステート
//===========================================================================
class PipelineState
{
public:
    virtual ~PipelineState() = default;

public:
    //! D3Dパイプラインステートを取得
    virtual ID3D12PipelineState* d3dPipelineState() const = 0;
};

//! パイプラインステートを生成
std::shared_ptr<PipelineState> createPipelineState(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc);

}   // namespace gpu
