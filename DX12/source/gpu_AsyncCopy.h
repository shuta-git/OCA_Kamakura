//---------------------------------------------------------------------------
//!	@file	gpu_AsyncCopy.h
//!	@brief	GPU非同期コピー転送
//---------------------------------------------------------------------------
#pragma once

namespace gpu {

// 非同期コピー
//!	@param	[in]	d3dResource         D3D12リソース
//!	@param	[in]	subresources        サブリソースの配列先頭アドレス
//!	@param	[in]	subresourceCount    サブリソース数
inline bool asyncCopy(ID3D12Resource* d3dResource, D3D12_SUBRESOURCE_DATA* subresources, size_t subresourceCount)
{
    // 転送に必要なメモリサイズを計算
    // https://glhub.blogspot.com/2016/07/dx12-getcopyablefootprints.html
    u64  requiredSize          = 0;   // 必要なサイズ
    auto firstSubresourceIndex = 0;   // 最初のサブリソース番号
    {
        auto resourceDesc = d3dResource->GetDesc();
        gpu::d3dDevice()->GetCopyableFootprints(&resourceDesc, 0, static_cast<UINT>(subresourceCount), 0, nullptr, nullptr, nullptr, &requiredSize);
    }

    //----------------------------------------------------------
    // 非同期コピーの準備
    //----------------------------------------------------------
    struct CopyContext
    {
        com_ptr<ID3D12CommandAllocator>    d3d12CommandAllocator_;
        com_ptr<ID3D12GraphicsCommandList> d3d12CommandList_;
        com_ptr<ID3D12Resource>            uploadHeap_;

        std::shared_ptr<gpu::Fence> gpuFence_;
    };

    std::unique_ptr<CopyContext> context = std::make_unique<CopyContext>();

    // 転送元のGPUメモリを確保(UPLOADヒープ)
    // ※このメモリは転送完了まで破棄してはならない
    {
        auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        auto resourceDesc   = CD3DX12_RESOURCE_DESC::Buffer(requiredSize);
        if(FAILED(gpu::d3dDevice()->CreateCommittedResource(
               &heapProperties,
               D3D12_HEAP_FLAG_NONE,
               &resourceDesc,
               D3D12_RESOURCE_STATE_GENERIC_READ,   // メモリの属性(DMA転送元) GENERIC_READ指定が必要
               nullptr,
               IID_PPV_ARGS(&context->uploadHeap_)))) {
            return false;
        }
    }

    {
        // コマンドアロケーター作成
        if(FAILED(gpu::d3dDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COPY, IID_PPV_ARGS(&context->d3d12CommandAllocator_)))) {
            return false;
        }
        // コマンドリストを作成
        if(FAILED(gpu::d3dDevice()->CreateCommandList(0,                                          // [in]  GPUノードマスク
                                                      D3D12_COMMAND_LIST_TYPE_COPY,               // [in]  コマンドリストの種類(COPY)
                                                      context->d3d12CommandAllocator_.Get(),      // [in]  コマンドアロケーター
                                                      nullptr,                                    // [in]  パイプラインステート
                                                      IID_PPV_ARGS(&context->d3d12CommandList_)   // [out] 作成されたコマンドリスト
                                                      ))) {
            return false;
        }
        // フェンスの作成
        context->gpuFence_ = gpu::createFence(0, true);
        if(!context->gpuFence_) {
            return false;
        }
    }

    //----------------------------------------------------------
    // VRAMへの転送するコマンドを登録
    //----------------------------------------------------------

    // 内部的にはメモリをUPLOADヒープにCPUでコピーした後に
    // GPUコマンドでVRAMのバッファへコピー
    auto commandList = context->d3d12CommandList_.Get();
    UpdateSubresources(commandList,
                       d3dResource,
                       context->uploadHeap_.Get(),
                       0,
                       firstSubresourceIndex,
                       static_cast<UINT>(subresourceCount),
                       subresources);
    // コマンドリストの終端
    commandList->Close();

    //----------------------------------------------------------
    // COPYキューで非同期コピーコマンド実行
    //----------------------------------------------------------
    auto               d3dCommandQueue = gpu::render()->commandQueue(D3D12_COMMAND_LIST_TYPE_COPY);
    ID3D12CommandList* lists{ commandList };
    d3dCommandQueue->ExecuteCommandLists(1, &lists);

    // コピー終了のメッセージシグナルを実行
    {
        auto& fence = context->gpuFence_;

        // CPU側のイベントとフェンスを関連付け
        // 注意点  Signalコマンド実行前に設定が必要
        fence->d3d12Fence()->SetEventOnCompletion(1, fence->event());

        d3dCommandQueue->Signal(fence->d3d12Fence(), 1);
    }
    /*
    // 転送終了後に状態をCOPY_DEST→PIXEL_SHADER_RESOUCEへ遷移させる
    {
        auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(d3dResource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
        commandList->ResourceBarrier(1, &resourceBarrier);
    }
*/

    //----------------------------------------------------------
    // 遅延解放用のスレッドを起動
    //----------------------------------------------------------
    std::thread t(
        [](std::unique_ptr<CopyContext> c) {
            //--------------------------------------------------
            // イベントハンドルを待機
            // コピー終了フェンスに連動するイベントを待つことで、コピーが終わった瞬間にその先が実行される
            //--------------------------------------------------
            WaitForSingleObject(c->gpuFence_->event(), INFINITE);

            //--------------------------------------------------
            // コピー完了後の後処理
            //--------------------------------------------------
            // 一時的に確保したUPLOADヒープの解放
            // cはunique_ptrのため、このスレッド関数終了後に自動解放される
            //c->uploadHeap_.Reset();
        },
        std::move(context));

    // std::threadの管理下からスレッドを切り離し
    t.detach();

    return true;
}

}   // namespace gpu
