//---------------------------------------------------------------------------
//!	@file	gpu_PipelineState.cpp
//!	@brief	GPUパイプラインステート
//---------------------------------------------------------------------------
#include "gpu_PipelineState.h"
#include <unordered_map>
#include <fstream>
#include <sstream>

bool operator==(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& ihs, const D3D12_GRAPHICS_PIPELINE_STATE_DESC& rhs)
{
    return memcmp(&ihs,&rhs,sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC)) == 0;
}

bool operator!=(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& ihs, const D3D12_GRAPHICS_PIPELINE_STATE_DESC& rhs)
{
    return !operator==(ihs, rhs);
}

namespace gpu {

//===========================================================================
// GPUパイプラインステート(実装部)
//===========================================================================
class PipelineStateImpl final : public gpu::PipelineState
{
public:
    PipelineStateImpl(com_ptr<ID3D12PipelineState>& pipelineState)
    {
        d3dPipelineState_ = pipelineState;
    }

    virtual ~PipelineStateImpl() = default;

public:
    //! D3Dパイプラインステートを取得
    virtual ID3D12PipelineState* d3dPipelineState() const override
    {
        return d3dPipelineState_.Get();
    }

private:
    com_ptr<ID3D12PipelineState> d3dPipelineState_;
};

namespace {

struct CustomHash
{
    using result_type = std::size_t;

    // ハッシュ計算
    std::size_t operator()(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& key) const
    {
        // 現在は文字列データとしてハッシュ計算しているが、高速化のために最適化予定
        std::string bytes(reinterpret_cast<const char*>(&key), sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
        return std::hash<std::string>()(bytes);
    }
};

std::unordered_map<D3D12_GRAPHICS_PIPELINE_STATE_DESC, com_ptr<ID3D12PipelineState>, CustomHash> pipelineStates_;

}   // namespace

//---------------------------------------------------------------------------
//! パイプラインステートを生成
//---------------------------------------------------------------------------
std::shared_ptr<PipelineState> createPipelineState(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc)
{
    com_ptr<ID3D12PipelineState> d3dPipelineState;

    if(pipelineStates_.find(desc) != pipelineStates_.end()) {
        // 生成済だった場合
        d3dPipelineState = pipelineStates_[desc];   
    }
    else {
        //------------------------------------------------------
        // パイプラインステートを新規作成
        //------------------------------------------------------
        if(FAILED(gpu::d3dDevice()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&d3dPipelineState)))) {
            return false;
        }

        // 生成済リストに登録
        pipelineStates_[desc] = d3dPipelineState;

#if 0
        // descの内容から64bitハッシュ値を計算しIDとしてファイル名に利用
        CustomHash hash;
        auto hashValue = hash(desc);// 全体をハッシュ値に使うとアドレス値が含まれるためこの方法では問題が発生
                
        // ハッシュ値を文字列化
        std::string hashName;
        {
            std::stringstream ss;
            ss << std::hex << hashValue;
            hashName = ss.str();
        }

        // TODO: ここで得られるBlobをストレージに保存することで高速にパイプラインステートを再生成可能
        // (現時点では取得しているのみ)
        com_ptr<ID3DBlob> blob;
        d3dPipelineState->GetCachedBlob(&blob);

        //------------------------------------------------------
        // キャッシュをファイルに保存
        //------------------------------------------------------
        std::ofstream file(std::string("pipeline_state/")+ hashName + ".pso");

        file.write(reinterpret_cast<const char*>(blob->GetBufferPointer()), blob->GetBufferSize());
#endif 
    }
    if(!d3dPipelineState) {
        return nullptr;
    }

    //----------------------------------------------------------
    // クラス初期化
    //----------------------------------------------------------
    return std::make_shared<gpu::PipelineStateImpl>(d3dPipelineState);
};

}   // namespace gpu
