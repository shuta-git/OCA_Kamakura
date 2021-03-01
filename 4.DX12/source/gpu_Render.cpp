//---------------------------------------------------------------------------
//!	@file	gpu_Render.h(仮名)
//!	@brief	gpu全般
//---------------------------------------------------------------------------
#include "gpu_Render.h"
#include "gpu_PipelineState.h"
#include "IndexAllocator.h"
#include "DelayedRelease.h"


#include <map>

namespace gpu {

#ifdef  LTCG
constexpr bool USE_DEBUG_LAYER = false; //!< デバッグレイヤー無効
#endif   //  LTCG
constexpr bool USE_DEBUG_LAYER = true;   //!< デバッグレイヤー有効


//===========================================================================
// gpu::RenderImpl
//===========================================================================
class RenderImpl final : public gpu::Render
{
public:
    //! 初期化
    virtual bool initialize(u32 width, u32 height) override;

    //! 更新
    virtual void update() override;

    //! 描画
    virtual void render() override;

    //! 解放
    virtual void finalize() override;

    //! コマンドリストを取得
    virtual ID3D12GraphicsCommandList* graphicsCommandList() const override { return d3d12CommandList_.Get(); }

    //! コマンドキューを取得
    virtual ID3D12CommandQueue* commandQueue(D3D12_COMMAND_LIST_TYPE type)const override
    {
        switch(type) {
            case D3D12_COMMAND_LIST_TYPE_DIRECT:// 通常キュー
                return d3d12CommandQueue_.Get();
            case D3D12_COMMAND_LIST_TYPE_COMPUTE:// Computeキュー
                return d3d12CommandQueueCompute_.Get();
            case D3D12_COMMAND_LIST_TYPE_COPY:  // コピーキュー
                return d3d12CommandQueueCopy_.Get();
            default:
                ASSERT_MESSAGE(false, "不正なパラメーターです。");
                break;
        }
        return nullptr;
    }

    //! スワップチェインを取得
    virtual gpu::SwapChain* swapChain() const override
    {
        return swapChain_.get();
    }

    //! 実体を取得
    static gpu::RenderImpl* instance()
    {
        static gpu::RenderImpl singleton; // gpu::RenderImplのシングルトン
        return &singleton;
    }

    //! imguiを取得
    virtual com_ptr<ID3D12DescriptorHeap> GetHeapForImgui() const override
    {
        return heapForImgui_;
    }

private:
    //! コンストラクタ
    // new禁止
    RenderImpl();

    // コピー禁止/move禁止
    RenderImpl(const RenderImpl&) = delete;
    RenderImpl(RenderImpl&&)      = delete;
    RenderImpl& operator=(const RenderImpl&) = delete;
    RenderImpl& operator=(RenderImpl&&) = delete;

private:

    //! imguiを生成
    virtual com_ptr<ID3D12DescriptorHeap> CreateDescriptorHeapForImgui() override;

    //! 描画パイプラインの初期化
    bool initializePipeLine();

    //! アセットの読み込み
    bool loadAssets();

    //! コマンドリストへのコマンド登録
    void buildCommandList();

private:

    static constexpr u32 FRAME_COUNT = 2;

    //----------------------------------------------------------
    //! @name パイプライン関連
    //----------------------------------------------------------
    //@{

    com_ptr<ID3D12CommandQueue> d3d12CommandQueue_;          //!< コマンドキュー(DIRECT)
    com_ptr<ID3D12CommandQueue> d3d12CommandQueueCompute_;   //!< コマンドキュー(COMPUTE)
    com_ptr<ID3D12CommandQueue> d3d12CommandQueueCopy_;      //!< コマンドキュー(COPY)

    std::unique_ptr<gpu::SwapChain> swapChain_;   //!< スワップチェイン

    com_ptr<ID3D12CommandAllocator> d3d12CommandAllocators_[FRAME_COUNT];   //!< コマンドアロケーター
    
    com_ptr<ID3D12RootSignature> d3d12RootSignature_;   //!< ルート署名
   
    std::shared_ptr<gpu::PipelineState> pipelineState_;   //!< パイプラインステート  
    
    com_ptr<ID3D12GraphicsCommandList> d3d12CommandList_;                   //!< コマンドリスト

    //@}
    //----------------------------------------------------------
    //! @name アプリケーション向けリソース
    //----------------------------------------------------------
    //@{

    com_ptr<ID3D12Resource>     constantBuffer_;                        //!< 定数バッファ
    void*                       constantBufferTops_[FRAME_COUNT] = {};   //!< 定数バッファのメモリの先頭アドレス
    com_ptr<ID3D12Resource>     depthBuffer_;                           //!< 深度バッファ
    
    std::shared_ptr<gpu::PMD>   pmd_;                                   //!< PMD
    std::shared_ptr<gpu::FBX>   fbx_;                                   //!< FBX

    //@}

    com_ptr<ID3D12DescriptorHeap> heapForImgui_;   //!<　imguiヒープ

    f32 aspectRation_ = 16.0f / 9.0f;   //!< アスペクト比
    u32 width_        = 0;              //!< 解像度（幅）
    u32 height_       = 0;              //!< 解像度（高さ）
};

//============================================================================================================
// 内部実装
//============================================================================================================
namespace {

struct Vertex
{
    DirectX::XMFLOAT3 position;   //!< 座標 POSITION
    DirectX::XMFLOAT4 color_;     //!< 頂点カラー COLOR
};

struct SceneConstantBuffer
{
    matrix matWorld_ = math::identity(); //!< ワールド行列
    matrix matView_  = math::identity(); //!< ビュー行列
    matrix matProj_  = math::identity(); //!< 投影行列(プロジェクション行列)
    float3 eye_;                         //!< カメラ位置
    float3 lookAt_;                      //!< カメラ注視点
    float3  worldUp_;                    //!< 世界の上方向のベクトル
};

com_ptr<IDXGIFactory6> dxgiFactory_ ; //!< DXGIFactory
com_ptr<ID3D12Device>  d3d12Device_;  //!< D3D12デバイス

//!< フィーチャレベル列挙
D3D_FEATURE_LEVEL levels[] = {
    D3D_FEATURE_LEVEL_12_1,
    D3D_FEATURE_LEVEL_12_0,
    D3D_FEATURE_LEVEL_11_1,
    D3D_FEATURE_LEVEL_11_0,
};

std::array<gpu::DescriptorHeap, static_cast<size_t>(DESCRIPTOR_TYPE::Max)> descriptorHeaps;
std::array<IndexAllocator, static_cast<size_t>(DESCRIPTOR_TYPE::Max)> indexAllocators;

SceneConstantBuffer cb_{};   //!< 定数バッファの実データー
CameraDebug         camera_;   //!< デバッグカメラ

}   // namespace

//---------------------------------------------------------------------------
//! ハードウェアアダプターを検出取得
//! @return 取得したハードウェアアダプター（nullptrの場合は失敗）
//---------------------------------------------------------------------------
com_ptr<IDXGIAdapter1> getHardwareAdapter()
{
    com_ptr<IDXGIAdapter1> adapter;

    // すべてのアダプターを検索
    // IntelHDGraphics/NVIDIAGeForce/AMDRaderonなど複数持つ環境もあるため
    // ここで使用するディスプレイアダプターを選択する。ソフトウェアレンダラーは読み飛ばし
    for(u32 adapterIndex = 0; dxgiFactory_->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND; ++adapterIndex) {
        DXGI_ADAPTER_DESC1 desc;
        adapter->GetDesc1(&desc);

        // ソフトウェアレンダラーを除外
        if(desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
            continue;
        }

        // D3D12デバイスを生成試行。
        // ※この段階では実際に生成せず、生成可能かどうかのみチェック（最後の引数がnullptr）
        if(SUCCEEDED(D3D12CreateDevice(adapter.Get(),D3D_FEATURE_LEVEL_12_0,__uuidof(ID3D12Device),nullptr))) {
            OutputDebugString("GPU=");
            OutputDebugStringW(desc.Description);

            break;
        }
    }

    return adapter;
}

//---------------------------------------------------------------------------
//! 定数バッファのサイズを計算
//---------------------------------------------------------------------------
u32 calcConstantBufferByteSize(u32 byteSize)
{
    // 定数バッファはサイズ境界256バイト境界に合わせる必要がある
    // サイズを256バイト境界に切り上げる
    return (byteSize + (D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1)) & ~(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1);
}

//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
RenderImpl::RenderImpl()
{
}

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool RenderImpl::initialize(u32 width, u32 height)
{
    width_ = width;
    height_ = height;

    //! 描画パイプライン初期化
    if(!initializePipeLine()) {
        return false;
    }

    heapForImgui_ = CreateDescriptorHeapForImgui();
    if(!heapForImgui_) {
        return false;
    }

    //! アセット読み込み
    if(!loadAssets()) {
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------
//! 描画パイプラインの初期化
//---------------------------------------------------------------------------
bool RenderImpl::initializePipeLine()
{
    u32 dxgiFactoryFlags = 0;

    //----------------------------------------------------------
    // デバッグレイヤー
    //----------------------------------------------------------
    if constexpr(USE_DEBUG_LAYER) {
        // デバッグレイヤー有効化（Windows10のオプション機能： グラフィックツールの追加が必要）
        // [設定] - [アプリと機能] - [オプション機能]
        // 設定を切り替えた後にデバイスを生成することでデバックレイヤーが有効になります。
        com_ptr<ID3D12Debug> d3d12Debug;
        if(SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&d3d12Debug)))) {
            // デバッグレイヤー有効化
            d3d12Debug->EnableDebugLayer();

            // DXGIFactory生成にもフラグが必要
            dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        }
    }
    //----------------------------------------------------------
    // ファクトリーの生成
    //
    // DXGIFactoryからアダプター取得し、スワップチェイン生成が可能
    //  +--------------+          +---------------+
    //  | DXGIFactory2 |>>>>>>>>>>| IDXGIAdapter1 |
    //  +--------------+  Create  +---------------+
    //          v                      ↑ 参照
    //          v                 +=================+
    //           >>>>>>>>>>>>>>>>>| IDXGISwapChain1 |
    //                    Create  +=================+
    //----------------------------------------------------------
    if(FAILED(CreateDXGIFactory2(dxgiFactoryFlags,IID_PPV_ARGS(&dxgiFactory_)))) {
        return false;
    }

    //----------------------------------------------------------
    //! D3D12デバイスの作成
    //----------------------------------------------------------
    {
        // 最初にハードウェアで試行して失敗した場合はソフトウェアアダプターで生成。
        bool useWarpDevice = false; // [デバック用]強制的にWARPデバイスを利用
        
        if(!useWarpDevice) {
            // ハードウェアアダプターを選択
            com_ptr<IDXGIAdapter1> hardwareAdapter = getHardwareAdapter();

            // ハードウェアアダプターはあった場合はデバイス生成
            
            if(hardwareAdapter) {
                D3D_FEATURE_LEVEL featureLevel;
                for(auto l : levels) {
                    auto result = D3D12CreateDevice(hardwareAdapter.Get(), l, IID_PPV_ARGS(&d3d12Device_));
                    if(result == S_OK) {
                        featureLevel = l;
                        break;
                    }
                }
            }

            if(!d3dDevice) {
                // WARP(Windows Advanced Rasterization Platform)デバイス    
                // 高速ソフトウェアレンダリング
                com_ptr<IDXGIAdapter1> warpAdapter;
                if(FAILED(dxgiFactory_->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)))) {
                    return false;
                }
                if(FAILED(D3D12CreateDevice(
                       warpAdapter.Get(),
                       D3D_FEATURE_LEVEL_12_0,
                       IID_PPV_ARGS(&d3d12Device_)
                    ))) 
                {
                    return false;
                }
            }
        }
    }

    //----------------------------------------------------------
    // コマンドキューの作成
    //----------------------------------------------------------
    {
        D3D12_COMMAND_QUEUE_DESC queueDesc{};
        // デフォルトコマンドキュー指定
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

        // 通常キュー
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        if(FAILED(gpu::d3dDevice()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&d3d12CommandQueue_)))) {
            return false;
        }

        // Computeキュー
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
        if(FAILED(gpu::d3dDevice()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&d3d12CommandQueueCompute_)))) {
            return false;
        }

        // コピーキュー
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
        if(FAILED(gpu::d3dDevice()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&d3d12CommandQueueCopy_)))) {
            return false;
        }
    }

    //----------------------------------------------------------
    // スワップチェインの作成
    //----------------------------------------------------------
    swapChain_ = gpu::createSwapChain(width_, height_, DXGI_FORMAT_R8G8B8A8_UNORM, FRAME_COUNT);
    if(!swapChain_) {
        return false;
    }

    //----------------------------------------------------------
    // DescriptorHeap作成
    //----------------------------------------------------------
    {
        // RTV用
        constexpr size_t capacity = FRAME_COUNT;

        indexAllocators[static_cast<u32>(gpu::DESCRIPTOR_TYPE::RTV)] = IndexAllocator(capacity);
        descriptorHeaps[static_cast<u32>(gpu::DESCRIPTOR_TYPE::RTV)] = gpu::DescriptorHeap(capacity, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }
    {
        // SBV/SRV/UAV 共用
        constexpr size_t countCBV = FRAME_COUNT;   // 定数バッファ用
        constexpr size_t countSRV = 3;             // テクスチャ用

        //constexpr size_t capacity = 256;
        constexpr size_t capacity = FRAME_COUNT;

        indexAllocators[static_cast<u32>(gpu::DESCRIPTOR_TYPE::CBV_SRV_UAV)] = IndexAllocator(capacity);
        descriptorHeaps[static_cast<u32>(gpu::DESCRIPTOR_TYPE::CBV_SRV_UAV)] = gpu::DescriptorHeap(capacity, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);   // CBV/SRV/UAV用
    }
    {
        // DSV用
        constexpr size_t capacity = 1;

        indexAllocators[static_cast<u32>(gpu::DESCRIPTOR_TYPE::DSV)] = IndexAllocator(capacity);
        descriptorHeaps[static_cast<u32>(gpu::DESCRIPTOR_TYPE::DSV)] = gpu::DescriptorHeap(capacity, D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    }

    //----------------------------------------------------------
    // スワップチェインのバックバッファから
    // RenderTargerリソースとViewを作成
    //----------------------------------------------------------
    {
        //CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(heapRTV_->GetCPUDescriptorHandleForHeapStart());
        // それぞれのフレームのRTVを作成
        //    RTV 1要素あたりのサイズ strideRTV_
        //           |←-→|
        //
        //           +---+---+---+
        //  heapRTV_ |   |   |   |
        //           +---+---+---+
        //            [0] [1] [2]
        //             ↑   ↑   ↑　rtvHandleを進めながら作成
        //         rtvHandle
        for(u32 n = 0; n < FRAME_COUNT; ++n) {
            // スワップチェインからバックバッファのResourceを取得
            // バックバッファからRTV作成
            d3d12Device_->CreateRenderTargetView(swapChain_->backBuffer(n), nullptr, descriptorHeap(gpu::DESCRIPTOR_TYPE::RTV)->at(n));
        }
    }

   
    //----------------------------------------------------------
    // 深度バッファの作成
    //----------------------------------------------------------
    {
        // DepthStencilBufferView用のバッファ作成
        // 詳細設定
        D3D12_RESOURCE_DESC depthResourceDesc{};
        depthResourceDesc.Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE2D;   // 2次元のテクスチャデータ
        depthResourceDesc.Width            = width_;                               // 幅と高さはレンダーターゲットと同じ
        depthResourceDesc.Height           = height_;                              // 上に同じ
        depthResourceDesc.DepthOrArraySize = 1;                                    // テクスチャ配列でも、3Dテクスチャでもない
        depthResourceDesc.Format           = DXGI_FORMAT_D32_FLOAT;                // 深度値書き込み用フォーマット
        depthResourceDesc.SampleDesc.Count = 1;                                    // サンプルは1ピクセルあたり1つ
        depthResourceDesc.Flags            = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
        // ヒーププロパティ―作成
        CD3DX12_HEAP_PROPERTIES depthHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
        // クリアバリュー作成
        CD3DX12_CLEAR_VALUE depthClearValue(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);
        // バッファ作成
        if(FAILED(d3d12Device_->CreateCommittedResource(
               &depthHeapProperties,
               D3D12_HEAP_FLAG_NONE,
               &depthResourceDesc,
               D3D12_RESOURCE_STATE_DEPTH_WRITE,
               &depthClearValue,
               IID_PPV_ARGS(&depthBuffer_)))) {
            return false;
        }

        D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilView{};
        depthStencilView.Format        = DXGI_FORMAT_D32_FLOAT;           // デプス値に32bit使用
        depthStencilView.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;   // 2Dテクスチャ
        depthStencilView.Flags         = D3D12_DSV_FLAG_NONE;             // フラグは特になし
        d3d12Device_->CreateDepthStencilView(
            depthBuffer_.Get(),
            &depthStencilView,
            descriptorHeap(gpu::DESCRIPTOR_TYPE::DSV)->data());
    }

    //----------------------------------------------------------
    // コマンドアロケーターを作成
    // 1フレームに必要なGPUコマンドを作成するメモリをここから確保する
    //----------------------------------------------------------
    for(u32 i = 0; i < FRAME_COUNT; ++i) {
        if(FAILED(d3d12Device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&d3d12CommandAllocators_[i])))) {
            return false;
        }
    }

    return true;
}

//---------------------------------------------------------------------------
//! imguiを生成
//---------------------------------------------------------------------------
com_ptr<ID3D12DescriptorHeap> RenderImpl::CreateDescriptorHeapForImgui()
{
    com_ptr<ID3D12DescriptorHeap> ret;

    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.Flags                      = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    desc.NodeMask                   = 0;
    desc.NumDescriptors             = 1;
    desc.Type                       = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

    if(FAILED(d3d12Device_->CreateDescriptorHeap(&desc, IID_PPV_ARGS(ret.ReleaseAndGetAddressOf())))) {
        return false;
    };
    
    return ret;
}

//---------------------------------------------------------------------------
//! アセット読み込み
// 頂点バッファ・インデックスバッファ・定数バッファ・深度バッファ
// シェーダー・テクスチャ・モデル
//---------------------------------------------------------------------------
bool RenderImpl::loadAssets()
{
    auto frameIndex = swapChain_->frameIndex();

    //----------------------------------------------------------
    // ルート署名(Root Signature)作成
    // ※　ここは再度やり直した方がよい
    //----------------------------------------------------------
    {
        // ルート署名のサポートバージョンを指定
        D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData{ D3D_ROOT_SIGNATURE_VERSION_1_1 };   // デフォルトは1.1

        // チェックして1.1がサポートしていない場合は1.0にする
        if(FAILED(d3d12Device_->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData)))) {
            featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
        }

        //------------------------------------------------------
        // 使用するシェーダーのスロットを設定
        // 定数バッファを頂点シェーダー slot0(b0) に設定できるように。
        //                +-------------------------------------+            +-------+
        // rootParameters | DescriptorTable [VISIBILITY_VERTEX] |>>>>>ranges |CBV, b0| 種類と使用スロットを指定
        //                +-------------------------------------+            +-------+
        //                |                :                    |            |   :   |
        //                                 : ※複数の場合は配列で定義               :
        //------------------------------------------------------

        // ディスクリプタレンジ
        CD3DX12_DESCRIPTOR_RANGE descriptorRange[4] = {};
        descriptorRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);//定数[b0](ビュープロジェクション用)
        descriptorRange[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);//定数[b1](ワールド、ボーン用)
        descriptorRange[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 2);//定数[b2](マテリアル用)
        descriptorRange[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 4, 0);//テクスチャ４つ(基本とsphとspaとトゥーン)

        // ルートパラメーター
        CD3DX12_ROOT_PARAMETER rootparam[3] = {};
        rootparam[0].InitAsDescriptorTable(1, &descriptorRange[0]);   // 座標変換
        rootparam[1].InitAsDescriptorTable(1, &descriptorRange[1]);   //  マテリアル周り
        rootparam[2].InitAsDescriptorTable(2, &descriptorRange[2]);   //マテリアル周り

        // サンプラー
        CD3DX12_STATIC_SAMPLER_DESC samplerDesc[2] = {};
        samplerDesc[0].Init(0);
        samplerDesc[1].Init(1,D3D12_FILTER_ANISOTROPIC,D3D12_TEXTURE_ADDRESS_MODE_CLAMP,D3D12_TEXTURE_ADDRESS_MODE_CLAMP);

        //------------------------------------------------------
        // ルート署名の生成
        //------------------------------------------------------
        // 詳細設定
        CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
        rootSignatureDesc.Init(3,rootparam,2,samplerDesc,D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

        // 設定項目をメモリバッファ(Blob)に生成出力
        // この関数の負荷が高いため注意。
        // TODO: ここで出力されたBlobをストレージに保存することで2回目以降この関数を経由せずに高速に再生成可能
        com_ptr<ID3DBlob> signature;    // ルート署名のバッファ
        com_ptr<ID3DBlob> error;        // エラーメッセージのバッファ
        if(FAILED(D3D12SerializeRootSignature(
            &rootSignatureDesc,         // ルート署名設定
            D3D_ROOT_SIGNATURE_VERSION_1_0,   // ルート署名バージョン
            &signature,                 // ルート署名のバッファ
            &error)))                   // エラーメッセージのバッファ
        {         
            if(error) {
                OutputDebugString(static_cast<LPCSTR>(error->GetBufferPointer()));
                MessageBox(application::hwnd(), static_cast<LPCSTR>(error->GetBufferPointer()), "RootSignature作成エラー", MB_OK);
            }
            return false;
        }

        if(FAILED(d3d12Device_->CreateRootSignature(
               0,                                       // [in]  GPUノードマスク(現在は0)
               signature->GetBufferPointer(),           // [in]  ルート署名バッファのメモリの先頭アドレス
               signature->GetBufferSize(),              // [in]  ルート署名バッファのメモリのサイズ
               IID_PPV_ARGS(&d3d12RootSignature_))))    // [out] 作成されたルート署名
        {
            return false;
        }

    }

    //----------------------------------------------------------
    // パイプラインステートを作成
    // ※すべての設定を組み込んだ情報を生成するため非常に低速なため注意
    //----------------------------------------------------------
    std::shared_ptr<gpu::Shader> shaderVs_;
    std::shared_ptr<gpu::Shader> shaderPs_;
    {
        //  シェーダーをコンパイルしてBlobにロード
        shaderVs_ = gpu::createShader("Shader.fx","VS","vs_5_1");
        shaderPs_ = gpu::createShader("Shader.fx","PS", "ps_5_1");

        if(!shaderVs_ || !shaderPs_) {
            return false;
        }

        // 頂点入力レイアウト
        D3D12_INPUT_ELEMENT_DESC inputElements[] = {
            {
                // 座標情報
                "POSITION",                                 // セマンティクス名
                0,                                          // セマンティック番号        
                DXGI_FORMAT_R32G32B32_FLOAT,                // データ形式
                0,                                          // スロット番号
                D3D12_APPEND_ALIGNED_ELEMENT,               // 頂点構造体先頭からのオフセット
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, // 
                0                                           // 一度に描画するインスタンスの数
            },
            // 法線
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            // UV値
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            // ボーン
            { "BONENO", 0, DXGI_FORMAT_R16G16_UINT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "WEIGHT", 0, DXGI_FORMAT_R8_UINT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            //{ "EDGE_FLG", 0, DXGI_FORMAT_R16G16_UINT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        };

         //----------------------------------------------------------
         // パイプラインステートの項目を設定
         //----------------------------------------------------------
         D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
         desc.pRootSignature                    = d3d12RootSignature_.Get();                                      // ルート署名
         desc.VS                                = *shaderVs_;                                                     // 頂点シェーダー
         desc.PS                                = *shaderPs_;                                                     // ピクセルシェーダー
         desc.StreamOutput                      = {};                                                             // ストリーム出力
         desc.BlendState                        = CD3DX12_BLEND_DESC(D3D12_DEFAULT);                              // ブレンドステート
         desc.SampleMask                        = 0xfffffffful;                                                   // サンプルマスク(ビットがマスクビットで32個)
         desc.RasterizerState                   = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);                         // ラスタライザステート
         desc.DepthStencilState.DepthEnable     = true;                                                           // デプスOFF
         desc.DepthStencilState.StencilEnable   = false;                                                          // ステンシルOFF
         desc.DepthStencilState.DepthFunc       = D3D12_COMPARISON_FUNC_LESS;
         desc.DepthStencilState.DepthWriteMask  = D3D12_DEPTH_WRITE_MASK_ALL;
         desc.InputLayout                       = { inputElements, static_cast<UINT>(std::size(inputElements)) }; // 入力レイアウト
         desc.IBStripCutValue                   = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFFFFFF;                  // インデックスバッファのStripカット値
         desc.PrimitiveTopologyType             = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;                         // 描画プリミティブ型(三角形)
         desc.NumRenderTargets                  = 1;                                                              // RenderTarget個数
         desc.RTVFormats[0]                     = DXGI_FORMAT_R8G8B8A8_UNORM;                                     // RenderTargetのピクセル形式
         desc.DSVFormat                         = DXGI_FORMAT_D32_FLOAT;                                          // デプスのピクセル形式(向こうの場合はUNKNOWN指定)
         desc.SampleDesc.Count                  = 1;                                                              // マルチサンプルOFF
         desc.SampleDesc.Quality                = 0;
         desc.NodeMask                          = 0;                                                              // GPUノードマスク
         desc.Flags                             = D3D12_PIPELINE_STATE_FLAG_NONE;                                 // フラグ
         // ラスタライザ(RS
         desc.RasterizerState.MultisampleEnable     = false;
         desc.RasterizerState.CullMode              = D3D12_CULL_MODE_NONE;   //カリングしない
         desc.RasterizerState.FillMode              = D3D12_FILL_MODE_SOLID;
         desc.RasterizerState.DepthClipEnable       = true;
         desc.RasterizerState.FrontCounterClockwise = false;
         desc.RasterizerState.DepthBias             = D3D12_DEFAULT_DEPTH_BIAS;
         desc.RasterizerState.DepthBiasClamp        = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
         desc.RasterizerState.SlopeScaledDepthBias  = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
         desc.RasterizerState.AntialiasedLineEnable = false;
         desc.RasterizerState.ForcedSampleCount     = 0;
         desc.RasterizerState.ConservativeRaster    = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

         // TODO:作成後のBlobをこのメンバに指定することで高速再生成可能
         desc.CachedPSO.pCachedBlob = nullptr;          // Blobの先頭アドレス
         desc.CachedPSO.CachedBlobSizeInBytes = 0;      // Blobのバイトサイズ
        
         //------------------------------------------------------
         // パイプラインステートを作成
         //------------------------------------------------------
         pipelineState_ = gpu::createPipelineState(desc);
         if(!pipelineState_) {
             return false;
         }
    }

    //----------------------------------------------------------
    // コマンドリストの作成
    // コマンドリストはフレーム単位のコマンドの生成を担う
    //----------------------------------------------------------
    {
        if(FAILED(d3d12Device_->CreateCommandList(
            0,
            D3D12_COMMAND_LIST_TYPE_DIRECT,
            d3d12CommandAllocators_[frameIndex].Get(),
            pipelineState_->d3dPipelineState(),
            IID_PPV_ARGS(&d3d12CommandList_)))) 
        {
            return false;
        }
    }

    //----------------------------------------------------------
    // 定数バッファの作成
    //----------------------------------------------------------
    {
        // GPUメモリの確保
        auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        auto resourceDesc   = CD3DX12_RESOURCE_DESC::Buffer(1024 * 64);                                      // 64KB

        if(FAILED(d3d12Device_->CreateCommittedResource(
               &heapProperties,                     // [in]  メモリタイプ（UPLOADヒープ）
               D3D12_HEAP_FLAG_NONE,                // [in]　ヒープフラグ（シェーダーからのアクセル権など）
               &resourceDesc,                       // [in]  確保するメモリの詳細設定
               D3D12_RESOURCE_STATE_GENERIC_READ,   // [in]  メモリのステート
               nullptr,                             // [in]  初期化カラー値
               IID_PPV_ARGS(&constantBuffer_)))) {  // [out] 確保されたGPUメモリ
            return false;
        }

        // 定数バッファのメモリマッピング
        // 動的なバッファについてはアプリケーション終了までMapしたまま利用しても支障ありません
        CD3DX12_RANGE readRange(0, 0); // CPU側から読み取らないための範囲指定
        void*         data = nullptr;
        if(FAILED(constantBuffer_->Map(0, &readRange,&data))) {
            return false;
        }

        // 定数バッファのサイズを256バイト境界で取得
        auto size = calcConstantBufferByteSize(sizeof(SceneConstantBuffer));
        for(u32 i = 0; i < FRAME_COUNT; ++i) {
            // 定数バッファViewの作成
            // CBVはDescrptorヒープから作成する必要がある
            D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
            desc.BufferLocation = constantBuffer_->GetGPUVirtualAddress() + size * i;   // GPU アドレス
            desc.SizeInBytes    = size;                                                 // 定数バッファサイズは256バイト境界

            d3d12Device_->CreateConstantBufferView(&desc, descriptorHeap(gpu::DESCRIPTOR_TYPE::CBV_SRV_UAV)->at(i));

            // 初期化コピー
            constantBufferTops_[i] = reinterpret_cast<void*>(reinterpret_cast<uintptr>(data) + size * i);
            memcpy(constantBufferTops_[i], &cb_, sizeof(cb_));
        }
    }

    //----------------------------------------------------------
    // PMDの読み込み
    //----------------------------------------------------------
    pmd_ = gpu::createPMDFromFile("../resource/Model/初音ミク.pmd");

    fbx_ = gpu::createFBXFromFile("../resource/Model/Cerberus_LP.FBX");
    //pmd_->LoadVMDFile("../../../directx12_resource/motion/squat.vmd");

    //----------------------------------------------------------
    // 転送コマンドが登録されたコマンドリストをここで実行
    //----------------------------------------------------------
    if(FAILED(d3d12CommandList_->Close())) {
        return false;
    }
    ID3D12CommandList* commandLists[] = { d3d12CommandList_.Get() };
    auto*              commandQueue   = gpu::render()->commandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);
    commandQueue->ExecuteCommandLists(static_cast<UINT>(std::size(commandLists)), commandLists);

    // 実行完了までブロッキングでまつ（非効率なため改善が必要）
    swapChain_->waitForGpu();

    return true;
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void RenderImpl::update()
{
    auto frameIndex = swapChain_->frameIndex();

    pmd_->MotionUpdate();

    camera_.update();
    //cb_.matView_ = camera_.view();
    //cb_.matProj_ = camera_.projection();

    cb_.eye_     = float3(0, 20, -35);
    cb_.lookAt_  = float3(0, 15, 0);
    cb_.worldUp_ = float3(0, 1, 0);
    cb_.matView_ = math::lookAtRH(cb_.eye_, cb_.lookAt_, cb_.worldUp_);
    cb_.matProj_ = math::perspectiveFovRH(45.0f * (math::PI / 180.0f), 16.0f / 9.0f, 0.01f, 10000.0f);

    // 定数バッファのメモリ更新
    memcpy(constantBufferTops_[frameIndex], &cb_, sizeof(cb_));
}

//---------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------
void RenderImpl::render()
{
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Rendering Test Menu");
    ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
    
    ImGui::End();
    ImGui::Render();

    // コマンドリストへの登録
    // シーンをレンダリングするために必要なすべてのコマンドをコマンドリストに設定
    buildCommandList();

    //----------------------------------------------------------
    // コマンドリストの実行
    //----------------------------------------------------------
    ID3D12CommandList* commandList = d3d12CommandList_.Get();
    auto*              commandQueue = gpu::render()->commandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);
    commandQueue->ExecuteCommandLists(1, &commandList);

    //----------------------------------------------------------
    // フレーム更新
    //----------------------------------------------------------
    swapChain_->present(1);
}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void RenderImpl::finalize()
{
    // 確実にGPUがデストラクタによってクリーンアップされようとしているリソースを
    // 参照しないようにするために描画完了を待つ
    swapChain_->waitForGpu();
}

//---------------------------------------------------------------------------
//! コマンドリストへのコマンド登録
//---------------------------------------------------------------------------
void RenderImpl::buildCommandList()
{
    auto frameIndex = swapChain_->frameIndex();

    // コマンドリストのアロケーターは関連するコマンドリストが
    // GPU上での実行を終了したときにのみリセットできます。
    if(FAILED(d3d12CommandAllocators_[frameIndex]->Reset())) {
        assert(0);
    }

    // コマンドリスト取得
    auto c = d3d12CommandList_.Get();

    // 特定のコマンドリストで　ExecuteCommandList() が呼び出された場合
    // そのコマンドリストは再利用前にResetを呼ばなければならない
    if(FAILED(c->Reset(d3d12CommandAllocators_[frameIndex].Get(), pipelineState_->d3dPipelineState()))) {
        assert(0);
    }

    //----------------------------------------------------------
    // 必要なステートの設定
    //----------------------------------------------------------
    // ルート署名設定
    c->SetGraphicsRootSignature(d3d12RootSignature_.Get());

    // CBV/SRV用のヒープを設定
    // ヒープは種類につき1個の設定。CBV/SRV/UAVは共有
    ID3D12DescriptorHeap* heap[]{ descriptorHeap(gpu::DESCRIPTOR_TYPE::CBV_SRV_UAV)->get() };
    c->SetDescriptorHeaps(static_cast<UINT>(std::size(heap)), heap);

    // CBV/SRV用のDescriptorTableを設定
    // このスロットはルートパラメーターと対応
    // [0] = CBV用DescriptorTableのアドレス
    // [1] = SRV用DescriptorTableのアドレス
    c->SetGraphicsRootDescriptorTable(0, descriptorHeap(gpu::DESCRIPTOR_TYPE::CBV_SRV_UAV)->at(frameIndex));   // [CBV] 0～(FRAME_COUNT-1)
    c->SetGraphicsRootDescriptorTable(1, descriptorHeap(gpu::DESCRIPTOR_TYPE::CBV_SRV_UAV)->at(0));            // [SRV] FRAME_COUNT

    {
        CD3DX12_VIEWPORT viewport(0.0f, 0.0f, static_cast<f32>(width_), static_cast<f32>(height_), 0.0f, 1.0f);
        CD3DX12_RECT     scissorRect(0, 0, width_, height_);

        c->RSSetViewports(1, &viewport);         // ビューポート
        c->RSSetScissorRects(1, &scissorRect);   // シザリング矩形
    }

    //----------------------------------------------------------
    // RenderTargetのリソースバリア
    // PRESENT→RENDER_TARGETへの遷移
    //----------------------------------------------------------
    {
        // ※フレーム更新のPresentを実行するためにはここでステートがPRESENTに遷移している必要あり
        auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(swapChain_->backBuffer(frameIndex), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
        c->ResourceBarrier(1, &resourceBarrier);
    }

    //----------------------------------------------------------
    // 描画コマンドの登録
    //----------------------------------------------------------
    // ※RenderTargetの設定はステートがリソースバリアでステートがRENDER_TARGETになっている必要あり
    D3D12_CPU_DESCRIPTOR_HANDLE rtvs[]{ descriptorHeap(gpu::DESCRIPTOR_TYPE::RTV)->at(frameIndex) };
    D3D12_CPU_DESCRIPTOR_HANDLE dsvs[]{ descriptorHeap(gpu::DESCRIPTOR_TYPE::DSV)->at(0)};
    c->OMSetRenderTargets(1, rtvs, FALSE, dsvs);

    // コマンド登録
    const f32 clearColor[]{ 1.0f, 1.0f, 1.0f, 1.0f };
    c->ClearRenderTargetView(rtvs[0], clearColor, 0, nullptr);                        // RenderTargetのカラークリア
    c->ClearDepthStencilView(dsvs[0], D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);   // DepthStencilのクリア
    c->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);                   // プリミティブ型の設定(TRIANGLELIST) ※ルート署名でプリミティブがTRIANGLEになっていること

    // pmd描画
    pmd_->PMDRender(c);

    // imgui
    c->SetDescriptorHeaps(1, heapForImgui_.GetAddressOf());
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), c);

    //----------------------------------------------------------
    // RenderTargetのリソースバリア
    // RENDER_TARGET→PRESENTへの遷移
    //----------------------------------------------------------
    {
        // ※フレーム更新のPresentを実行するためにはここでステートがPRESENTに遷移している必要あり
        auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(swapChain_->backBuffer(frameIndex), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
        c->ResourceBarrier(1, &resourceBarrier);
    }

    // コマンドリストの登録完了
    if(FAILED(c->Close())) {
        assert(0);
    }
}

//============================================================================================================
//! gpu::Render
//============================================================================================================
//---------------------------------------------------------------------------
//! 実体を取得
//---------------------------------------------------------------------------
gpu::Render* Render::instance()
{
    return RenderImpl::instance();
}

//---------------------------------------------------------------------------
//! レンダリング管理クラス
//---------------------------------------------------------------------------
gpu::Render* render()
{
    return Render::instance();
}

//---------------------------------------------------------------------------
//! DXGIFactoryを取得
//---------------------------------------------------------------------------
IDXGIFactory6* dxgiFactory()
{
    return dxgiFactory_.Get();
}

//---------------------------------------------------------------------------
//! D3Dデバイスを取得
//---------------------------------------------------------------------------
ID3D12Device* d3dDevice()
{
    return d3d12Device_.Get();
}

//---------------------------------------------------------------------------
// DescriptorHeapを取得
//---------------------------------------------------------------------------
gpu::DescriptorHeap* descriptorHeap(DESCRIPTOR_TYPE type)
{
    return &descriptorHeaps[static_cast<size_t>(type)];
}

//---------------------------------------------------------------------------
// インデックスアロケーターを取得
//---------------------------------------------------------------------------
IndexAllocator* indexAllocator(DESCRIPTOR_TYPE type)
{
    return &indexAllocators[static_cast<size_t>(type)];
}


}   // namespace gpu