//---------------------------------------------------------------------------
//!	@file	gpu_Render.h(����)
//!	@brief	gpu�S��
//---------------------------------------------------------------------------
#include "gpu_Render.h"
#include "gpu_PipelineState.h"
#include "IndexAllocator.h"
#include "DelayedRelease.h"


#include <map>

namespace gpu {

#ifdef  LTCG
constexpr bool USE_DEBUG_LAYER = false; //!< �f�o�b�O���C���[����
#endif   //  LTCG
constexpr bool USE_DEBUG_LAYER = true;   //!< �f�o�b�O���C���[�L��


//===========================================================================
// gpu::RenderImpl
//===========================================================================
class RenderImpl final : public gpu::Render
{
public:
    //! ������
    virtual bool initialize(u32 width, u32 height) override;

    //! �X�V
    virtual void update() override;

    //! �`��
    virtual void render() override;

    //! ���
    virtual void finalize() override;

    //! �R�}���h���X�g���擾
    virtual ID3D12GraphicsCommandList* graphicsCommandList() const override { return d3d12CommandList_.Get(); }

    //! �R�}���h�L���[���擾
    virtual ID3D12CommandQueue* commandQueue(D3D12_COMMAND_LIST_TYPE type)const override
    {
        switch(type) {
            case D3D12_COMMAND_LIST_TYPE_DIRECT:// �ʏ�L���[
                return d3d12CommandQueue_.Get();
            case D3D12_COMMAND_LIST_TYPE_COMPUTE:// Compute�L���[
                return d3d12CommandQueueCompute_.Get();
            case D3D12_COMMAND_LIST_TYPE_COPY:  // �R�s�[�L���[
                return d3d12CommandQueueCopy_.Get();
            default:
                ASSERT_MESSAGE(false, "�s���ȃp�����[�^�[�ł��B");
                break;
        }
        return nullptr;
    }

    //! �X���b�v�`�F�C�����擾
    virtual gpu::SwapChain* swapChain() const override
    {
        return swapChain_.get();
    }

    //! ���̂��擾
    static gpu::RenderImpl* instance()
    {
        static gpu::RenderImpl singleton; // gpu::RenderImpl�̃V���O���g��
        return &singleton;
    }

    //! imgui���擾
    virtual com_ptr<ID3D12DescriptorHeap> GetHeapForImgui() const override
    {
        return heapForImgui_;
    }

private:
    //! �R���X�g���N�^
    // new�֎~
    RenderImpl();

    // �R�s�[�֎~/move�֎~
    RenderImpl(const RenderImpl&) = delete;
    RenderImpl(RenderImpl&&)      = delete;
    RenderImpl& operator=(const RenderImpl&) = delete;
    RenderImpl& operator=(RenderImpl&&) = delete;

private:

    //! imgui�𐶐�
    virtual com_ptr<ID3D12DescriptorHeap> CreateDescriptorHeapForImgui() override;

    //! �`��p�C�v���C���̏�����
    bool initializePipeLine();

    //! �A�Z�b�g�̓ǂݍ���
    bool loadAssets();

    //! �R�}���h���X�g�ւ̃R�}���h�o�^
    void buildCommandList();

private:

    static constexpr u32 FRAME_COUNT = 2;

    //----------------------------------------------------------
    //! @name �p�C�v���C���֘A
    //----------------------------------------------------------
    //@{

    com_ptr<ID3D12CommandQueue> d3d12CommandQueue_;          //!< �R�}���h�L���[(DIRECT)
    com_ptr<ID3D12CommandQueue> d3d12CommandQueueCompute_;   //!< �R�}���h�L���[(COMPUTE)
    com_ptr<ID3D12CommandQueue> d3d12CommandQueueCopy_;      //!< �R�}���h�L���[(COPY)

    std::unique_ptr<gpu::SwapChain> swapChain_;   //!< �X���b�v�`�F�C��

    com_ptr<ID3D12CommandAllocator> d3d12CommandAllocators_[FRAME_COUNT];   //!< �R�}���h�A���P�[�^�[
    
    com_ptr<ID3D12RootSignature> d3d12RootSignature_;   //!< ���[�g����
   
    std::shared_ptr<gpu::PipelineState> pipelineState_;   //!< �p�C�v���C���X�e�[�g  
    
    com_ptr<ID3D12GraphicsCommandList> d3d12CommandList_;                   //!< �R�}���h���X�g

    //@}
    //----------------------------------------------------------
    //! @name �A�v���P�[�V�����������\�[�X
    //----------------------------------------------------------
    //@{

    com_ptr<ID3D12Resource>     constantBuffer_;                        //!< �萔�o�b�t�@
    void*                       constantBufferTops_[FRAME_COUNT] = {};   //!< �萔�o�b�t�@�̃������̐擪�A�h���X
    com_ptr<ID3D12Resource>     depthBuffer_;                           //!< �[�x�o�b�t�@
    
    std::shared_ptr<gpu::PMD>   pmd_;                                   //!< PMD
    std::shared_ptr<gpu::FBX>   fbx_;                                   //!< FBX

    //@}

    com_ptr<ID3D12DescriptorHeap> heapForImgui_;   //!<�@imgui�q�[�v

    f32 aspectRation_ = 16.0f / 9.0f;   //!< �A�X�y�N�g��
    u32 width_        = 0;              //!< �𑜓x�i���j
    u32 height_       = 0;              //!< �𑜓x�i�����j
};

//============================================================================================================
// ��������
//============================================================================================================
namespace {

struct Vertex
{
    DirectX::XMFLOAT3 position;   //!< ���W POSITION
    DirectX::XMFLOAT4 color_;     //!< ���_�J���[ COLOR
};

struct SceneConstantBuffer
{
    matrix matWorld_ = math::identity(); //!< ���[���h�s��
    matrix matView_  = math::identity(); //!< �r���[�s��
    matrix matProj_  = math::identity(); //!< ���e�s��(�v���W�F�N�V�����s��)
    float3 eye_;                         //!< �J�����ʒu
    float3 lookAt_;                      //!< �J���������_
    float3  worldUp_;                    //!< ���E�̏�����̃x�N�g��
};

com_ptr<IDXGIFactory6> dxgiFactory_ ; //!< DXGIFactory
com_ptr<ID3D12Device>  d3d12Device_;  //!< D3D12�f�o�C�X

//!< �t�B�[�`�����x����
D3D_FEATURE_LEVEL levels[] = {
    D3D_FEATURE_LEVEL_12_1,
    D3D_FEATURE_LEVEL_12_0,
    D3D_FEATURE_LEVEL_11_1,
    D3D_FEATURE_LEVEL_11_0,
};

std::array<gpu::DescriptorHeap, static_cast<size_t>(DESCRIPTOR_TYPE::Max)> descriptorHeaps;
std::array<IndexAllocator, static_cast<size_t>(DESCRIPTOR_TYPE::Max)> indexAllocators;

SceneConstantBuffer cb_{};   //!< �萔�o�b�t�@�̎��f�[�^�[
CameraDebug         camera_;   //!< �f�o�b�O�J����

}   // namespace

//---------------------------------------------------------------------------
//! �n�[�h�E�F�A�A�_�v�^�[�����o�擾
//! @return �擾�����n�[�h�E�F�A�A�_�v�^�[�inullptr�̏ꍇ�͎��s�j
//---------------------------------------------------------------------------
com_ptr<IDXGIAdapter1> getHardwareAdapter()
{
    com_ptr<IDXGIAdapter1> adapter;

    // ���ׂẴA�_�v�^�[������
    // IntelHDGraphics/NVIDIAGeForce/AMDRaderon�ȂǕ������������邽��
    // �����Ŏg�p����f�B�X�v���C�A�_�v�^�[��I������B�\�t�g�E�F�A�����_���[�͓ǂݔ�΂�
    for(u32 adapterIndex = 0; dxgiFactory_->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND; ++adapterIndex) {
        DXGI_ADAPTER_DESC1 desc;
        adapter->GetDesc1(&desc);

        // �\�t�g�E�F�A�����_���[�����O
        if(desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
            continue;
        }

        // D3D12�f�o�C�X�𐶐����s�B
        // �����̒i�K�ł͎��ۂɐ��������A�����\���ǂ����̂݃`�F�b�N�i�Ō�̈�����nullptr�j
        if(SUCCEEDED(D3D12CreateDevice(adapter.Get(),D3D_FEATURE_LEVEL_12_0,__uuidof(ID3D12Device),nullptr))) {
            OutputDebugString("GPU=");
            OutputDebugStringW(desc.Description);

            break;
        }
    }

    return adapter;
}

//---------------------------------------------------------------------------
//! �萔�o�b�t�@�̃T�C�Y���v�Z
//---------------------------------------------------------------------------
u32 calcConstantBufferByteSize(u32 byteSize)
{
    // �萔�o�b�t�@�̓T�C�Y���E256�o�C�g���E�ɍ��킹��K�v������
    // �T�C�Y��256�o�C�g���E�ɐ؂�グ��
    return (byteSize + (D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1)) & ~(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1);
}

//---------------------------------------------------------------------------
//! �R���X�g���N�^
//---------------------------------------------------------------------------
RenderImpl::RenderImpl()
{
}

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool RenderImpl::initialize(u32 width, u32 height)
{
    width_ = width;
    height_ = height;

    //! �`��p�C�v���C��������
    if(!initializePipeLine()) {
        return false;
    }

    heapForImgui_ = CreateDescriptorHeapForImgui();
    if(!heapForImgui_) {
        return false;
    }

    //! �A�Z�b�g�ǂݍ���
    if(!loadAssets()) {
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------
//! �`��p�C�v���C���̏�����
//---------------------------------------------------------------------------
bool RenderImpl::initializePipeLine()
{
    u32 dxgiFactoryFlags = 0;

    //----------------------------------------------------------
    // �f�o�b�O���C���[
    //----------------------------------------------------------
    if constexpr(USE_DEBUG_LAYER) {
        // �f�o�b�O���C���[�L�����iWindows10�̃I�v�V�����@�\�F �O���t�B�b�N�c�[���̒ǉ����K�v�j
        // [�ݒ�] - [�A�v���Ƌ@�\] - [�I�v�V�����@�\]
        // �ݒ��؂�ւ�����Ƀf�o�C�X�𐶐����邱�ƂŃf�o�b�N���C���[���L���ɂȂ�܂��B
        com_ptr<ID3D12Debug> d3d12Debug;
        if(SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&d3d12Debug)))) {
            // �f�o�b�O���C���[�L����
            d3d12Debug->EnableDebugLayer();

            // DXGIFactory�����ɂ��t���O���K�v
            dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        }
    }
    //----------------------------------------------------------
    // �t�@�N�g���[�̐���
    //
    // DXGIFactory����A�_�v�^�[�擾���A�X���b�v�`�F�C���������\
    //  +--------------+          +---------------+
    //  | DXGIFactory2 |>>>>>>>>>>| IDXGIAdapter1 |
    //  +--------------+  Create  +---------------+
    //          v                      �� �Q��
    //          v                 +=================+
    //           >>>>>>>>>>>>>>>>>| IDXGISwapChain1 |
    //                    Create  +=================+
    //----------------------------------------------------------
    if(FAILED(CreateDXGIFactory2(dxgiFactoryFlags,IID_PPV_ARGS(&dxgiFactory_)))) {
        return false;
    }

    //----------------------------------------------------------
    //! D3D12�f�o�C�X�̍쐬
    //----------------------------------------------------------
    {
        // �ŏ��Ƀn�[�h�E�F�A�Ŏ��s���Ď��s�����ꍇ�̓\�t�g�E�F�A�A�_�v�^�[�Ő����B
        bool useWarpDevice = false; // [�f�o�b�N�p]�����I��WARP�f�o�C�X�𗘗p
        
        if(!useWarpDevice) {
            // �n�[�h�E�F�A�A�_�v�^�[��I��
            com_ptr<IDXGIAdapter1> hardwareAdapter = getHardwareAdapter();

            // �n�[�h�E�F�A�A�_�v�^�[�͂������ꍇ�̓f�o�C�X����
            
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
                // WARP(Windows Advanced Rasterization Platform)�f�o�C�X    
                // �����\�t�g�E�F�A�����_�����O
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
    // �R�}���h�L���[�̍쐬
    //----------------------------------------------------------
    {
        D3D12_COMMAND_QUEUE_DESC queueDesc{};
        // �f�t�H���g�R�}���h�L���[�w��
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

        // �ʏ�L���[
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        if(FAILED(gpu::d3dDevice()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&d3d12CommandQueue_)))) {
            return false;
        }

        // Compute�L���[
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
        if(FAILED(gpu::d3dDevice()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&d3d12CommandQueueCompute_)))) {
            return false;
        }

        // �R�s�[�L���[
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
        if(FAILED(gpu::d3dDevice()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&d3d12CommandQueueCopy_)))) {
            return false;
        }
    }

    //----------------------------------------------------------
    // �X���b�v�`�F�C���̍쐬
    //----------------------------------------------------------
    swapChain_ = gpu::createSwapChain(width_, height_, DXGI_FORMAT_R8G8B8A8_UNORM, FRAME_COUNT);
    if(!swapChain_) {
        return false;
    }

    //----------------------------------------------------------
    // DescriptorHeap�쐬
    //----------------------------------------------------------
    {
        // RTV�p
        constexpr size_t capacity = FRAME_COUNT;

        indexAllocators[static_cast<u32>(gpu::DESCRIPTOR_TYPE::RTV)] = IndexAllocator(capacity);
        descriptorHeaps[static_cast<u32>(gpu::DESCRIPTOR_TYPE::RTV)] = gpu::DescriptorHeap(capacity, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }
    {
        // SBV/SRV/UAV ���p
        constexpr size_t countCBV = FRAME_COUNT;   // �萔�o�b�t�@�p
        constexpr size_t countSRV = 3;             // �e�N�X�`���p

        //constexpr size_t capacity = 256;
        constexpr size_t capacity = FRAME_COUNT;

        indexAllocators[static_cast<u32>(gpu::DESCRIPTOR_TYPE::CBV_SRV_UAV)] = IndexAllocator(capacity);
        descriptorHeaps[static_cast<u32>(gpu::DESCRIPTOR_TYPE::CBV_SRV_UAV)] = gpu::DescriptorHeap(capacity, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);   // CBV/SRV/UAV�p
    }
    {
        // DSV�p
        constexpr size_t capacity = 1;

        indexAllocators[static_cast<u32>(gpu::DESCRIPTOR_TYPE::DSV)] = IndexAllocator(capacity);
        descriptorHeaps[static_cast<u32>(gpu::DESCRIPTOR_TYPE::DSV)] = gpu::DescriptorHeap(capacity, D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    }

    //----------------------------------------------------------
    // �X���b�v�`�F�C���̃o�b�N�o�b�t�@����
    // RenderTarger���\�[�X��View���쐬
    //----------------------------------------------------------
    {
        //CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(heapRTV_->GetCPUDescriptorHandleForHeapStart());
        // ���ꂼ��̃t���[����RTV���쐬
        //    RTV 1�v�f������̃T�C�Y strideRTV_
        //           |��-��|
        //
        //           +---+---+---+
        //  heapRTV_ |   |   |   |
        //           +---+---+---+
        //            [0] [1] [2]
        //             ��   ��   ���@rtvHandle��i�߂Ȃ���쐬
        //         rtvHandle
        for(u32 n = 0; n < FRAME_COUNT; ++n) {
            // �X���b�v�`�F�C������o�b�N�o�b�t�@��Resource���擾
            // �o�b�N�o�b�t�@����RTV�쐬
            d3d12Device_->CreateRenderTargetView(swapChain_->backBuffer(n), nullptr, descriptorHeap(gpu::DESCRIPTOR_TYPE::RTV)->at(n));
        }
    }

   
    //----------------------------------------------------------
    // �[�x�o�b�t�@�̍쐬
    //----------------------------------------------------------
    {
        // DepthStencilBufferView�p�̃o�b�t�@�쐬
        // �ڍאݒ�
        D3D12_RESOURCE_DESC depthResourceDesc{};
        depthResourceDesc.Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE2D;   // 2�����̃e�N�X�`���f�[�^
        depthResourceDesc.Width            = width_;                               // ���ƍ����̓����_�[�^�[�Q�b�g�Ɠ���
        depthResourceDesc.Height           = height_;                              // ��ɓ���
        depthResourceDesc.DepthOrArraySize = 1;                                    // �e�N�X�`���z��ł��A3D�e�N�X�`���ł��Ȃ�
        depthResourceDesc.Format           = DXGI_FORMAT_D32_FLOAT;                // �[�x�l�������ݗp�t�H�[�}�b�g
        depthResourceDesc.SampleDesc.Count = 1;                                    // �T���v����1�s�N�Z��������1��
        depthResourceDesc.Flags            = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
        // �q�[�v�v���p�e�B�\�쐬
        CD3DX12_HEAP_PROPERTIES depthHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
        // �N���A�o�����[�쐬
        CD3DX12_CLEAR_VALUE depthClearValue(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);
        // �o�b�t�@�쐬
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
        depthStencilView.Format        = DXGI_FORMAT_D32_FLOAT;           // �f�v�X�l��32bit�g�p
        depthStencilView.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;   // 2D�e�N�X�`��
        depthStencilView.Flags         = D3D12_DSV_FLAG_NONE;             // �t���O�͓��ɂȂ�
        d3d12Device_->CreateDepthStencilView(
            depthBuffer_.Get(),
            &depthStencilView,
            descriptorHeap(gpu::DESCRIPTOR_TYPE::DSV)->data());
    }

    //----------------------------------------------------------
    // �R�}���h�A���P�[�^�[���쐬
    // 1�t���[���ɕK�v��GPU�R�}���h���쐬���郁��������������m�ۂ���
    //----------------------------------------------------------
    for(u32 i = 0; i < FRAME_COUNT; ++i) {
        if(FAILED(d3d12Device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&d3d12CommandAllocators_[i])))) {
            return false;
        }
    }

    return true;
}

//---------------------------------------------------------------------------
//! imgui�𐶐�
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
//! �A�Z�b�g�ǂݍ���
// ���_�o�b�t�@�E�C���f�b�N�X�o�b�t�@�E�萔�o�b�t�@�E�[�x�o�b�t�@
// �V�F�[�_�[�E�e�N�X�`���E���f��
//---------------------------------------------------------------------------
bool RenderImpl::loadAssets()
{
    auto frameIndex = swapChain_->frameIndex();

    //----------------------------------------------------------
    // ���[�g����(Root Signature)�쐬
    // ���@�����͍ēx��蒼���������悢
    //----------------------------------------------------------
    {
        // ���[�g�����̃T�|�[�g�o�[�W�������w��
        D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData{ D3D_ROOT_SIGNATURE_VERSION_1_1 };   // �f�t�H���g��1.1

        // �`�F�b�N����1.1���T�|�[�g���Ă��Ȃ��ꍇ��1.0�ɂ���
        if(FAILED(d3d12Device_->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData)))) {
            featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
        }

        //------------------------------------------------------
        // �g�p����V�F�[�_�[�̃X���b�g��ݒ�
        // �萔�o�b�t�@�𒸓_�V�F�[�_�[ slot0(b0) �ɐݒ�ł���悤�ɁB
        //                +-------------------------------------+            +-------+
        // rootParameters | DescriptorTable [VISIBILITY_VERTEX] |>>>>>ranges |CBV, b0| ��ނƎg�p�X���b�g���w��
        //                +-------------------------------------+            +-------+
        //                |                :                    |            |   :   |
        //                                 : �������̏ꍇ�͔z��Œ�`               :
        //------------------------------------------------------

        // �f�B�X�N���v�^�����W
        CD3DX12_DESCRIPTOR_RANGE descriptorRange[4] = {};
        descriptorRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);//�萔[b0](�r���[�v���W�F�N�V�����p)
        descriptorRange[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);//�萔[b1](���[���h�A�{�[���p)
        descriptorRange[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 2);//�萔[b2](�}�e���A���p)
        descriptorRange[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 4, 0);//�e�N�X�`���S��(��{��sph��spa�ƃg�D�[��)

        // ���[�g�p�����[�^�[
        CD3DX12_ROOT_PARAMETER rootparam[3] = {};
        rootparam[0].InitAsDescriptorTable(1, &descriptorRange[0]);   // ���W�ϊ�
        rootparam[1].InitAsDescriptorTable(1, &descriptorRange[1]);   //  �}�e���A������
        rootparam[2].InitAsDescriptorTable(2, &descriptorRange[2]);   //�}�e���A������

        // �T���v���[
        CD3DX12_STATIC_SAMPLER_DESC samplerDesc[2] = {};
        samplerDesc[0].Init(0);
        samplerDesc[1].Init(1,D3D12_FILTER_ANISOTROPIC,D3D12_TEXTURE_ADDRESS_MODE_CLAMP,D3D12_TEXTURE_ADDRESS_MODE_CLAMP);

        //------------------------------------------------------
        // ���[�g�����̐���
        //------------------------------------------------------
        // �ڍאݒ�
        CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
        rootSignatureDesc.Init(3,rootparam,2,samplerDesc,D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

        // �ݒ荀�ڂ��������o�b�t�@(Blob)�ɐ����o��
        // ���̊֐��̕��ׂ��������ߒ��ӁB
        // TODO: �����ŏo�͂��ꂽBlob���X�g���[�W�ɕۑ����邱�Ƃ�2��ڈȍ~���̊֐����o�R�����ɍ����ɍĐ����\
        com_ptr<ID3DBlob> signature;    // ���[�g�����̃o�b�t�@
        com_ptr<ID3DBlob> error;        // �G���[���b�Z�[�W�̃o�b�t�@
        if(FAILED(D3D12SerializeRootSignature(
            &rootSignatureDesc,         // ���[�g�����ݒ�
            D3D_ROOT_SIGNATURE_VERSION_1_0,   // ���[�g�����o�[�W����
            &signature,                 // ���[�g�����̃o�b�t�@
            &error)))                   // �G���[���b�Z�[�W�̃o�b�t�@
        {         
            if(error) {
                OutputDebugString(static_cast<LPCSTR>(error->GetBufferPointer()));
                MessageBox(application::hwnd(), static_cast<LPCSTR>(error->GetBufferPointer()), "RootSignature�쐬�G���[", MB_OK);
            }
            return false;
        }

        if(FAILED(d3d12Device_->CreateRootSignature(
               0,                                       // [in]  GPU�m�[�h�}�X�N(���݂�0)
               signature->GetBufferPointer(),           // [in]  ���[�g�����o�b�t�@�̃������̐擪�A�h���X
               signature->GetBufferSize(),              // [in]  ���[�g�����o�b�t�@�̃������̃T�C�Y
               IID_PPV_ARGS(&d3d12RootSignature_))))    // [out] �쐬���ꂽ���[�g����
        {
            return false;
        }

    }

    //----------------------------------------------------------
    // �p�C�v���C���X�e�[�g���쐬
    // �����ׂĂ̐ݒ��g�ݍ��񂾏��𐶐����邽�ߔ��ɒᑬ�Ȃ��ߒ���
    //----------------------------------------------------------
    std::shared_ptr<gpu::Shader> shaderVs_;
    std::shared_ptr<gpu::Shader> shaderPs_;
    {
        //  �V�F�[�_�[���R���p�C������Blob�Ƀ��[�h
        shaderVs_ = gpu::createShader("Shader.fx","VS","vs_5_1");
        shaderPs_ = gpu::createShader("Shader.fx","PS", "ps_5_1");

        if(!shaderVs_ || !shaderPs_) {
            return false;
        }

        // ���_���̓��C�A�E�g
        D3D12_INPUT_ELEMENT_DESC inputElements[] = {
            {
                // ���W���
                "POSITION",                                 // �Z�}���e�B�N�X��
                0,                                          // �Z�}���e�B�b�N�ԍ�        
                DXGI_FORMAT_R32G32B32_FLOAT,                // �f�[�^�`��
                0,                                          // �X���b�g�ԍ�
                D3D12_APPEND_ALIGNED_ELEMENT,               // ���_�\���̐擪����̃I�t�Z�b�g
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, // 
                0                                           // ��x�ɕ`�悷��C���X�^���X�̐�
            },
            // �@��
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            // UV�l
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            // �{�[��
            { "BONENO", 0, DXGI_FORMAT_R16G16_UINT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "WEIGHT", 0, DXGI_FORMAT_R8_UINT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            //{ "EDGE_FLG", 0, DXGI_FORMAT_R16G16_UINT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        };

         //----------------------------------------------------------
         // �p�C�v���C���X�e�[�g�̍��ڂ�ݒ�
         //----------------------------------------------------------
         D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
         desc.pRootSignature                    = d3d12RootSignature_.Get();                                      // ���[�g����
         desc.VS                                = *shaderVs_;                                                     // ���_�V�F�[�_�[
         desc.PS                                = *shaderPs_;                                                     // �s�N�Z���V�F�[�_�[
         desc.StreamOutput                      = {};                                                             // �X�g���[���o��
         desc.BlendState                        = CD3DX12_BLEND_DESC(D3D12_DEFAULT);                              // �u�����h�X�e�[�g
         desc.SampleMask                        = 0xfffffffful;                                                   // �T���v���}�X�N(�r�b�g���}�X�N�r�b�g��32��)
         desc.RasterizerState                   = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);                         // ���X�^���C�U�X�e�[�g
         desc.DepthStencilState.DepthEnable     = true;                                                           // �f�v�XOFF
         desc.DepthStencilState.StencilEnable   = false;                                                          // �X�e���V��OFF
         desc.DepthStencilState.DepthFunc       = D3D12_COMPARISON_FUNC_LESS;
         desc.DepthStencilState.DepthWriteMask  = D3D12_DEPTH_WRITE_MASK_ALL;
         desc.InputLayout                       = { inputElements, static_cast<UINT>(std::size(inputElements)) }; // ���̓��C�A�E�g
         desc.IBStripCutValue                   = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFFFFFF;                  // �C���f�b�N�X�o�b�t�@��Strip�J�b�g�l
         desc.PrimitiveTopologyType             = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;                         // �`��v���~�e�B�u�^(�O�p�`)
         desc.NumRenderTargets                  = 1;                                                              // RenderTarget��
         desc.RTVFormats[0]                     = DXGI_FORMAT_R8G8B8A8_UNORM;                                     // RenderTarget�̃s�N�Z���`��
         desc.DSVFormat                         = DXGI_FORMAT_D32_FLOAT;                                          // �f�v�X�̃s�N�Z���`��(�������̏ꍇ��UNKNOWN�w��)
         desc.SampleDesc.Count                  = 1;                                                              // �}���`�T���v��OFF
         desc.SampleDesc.Quality                = 0;
         desc.NodeMask                          = 0;                                                              // GPU�m�[�h�}�X�N
         desc.Flags                             = D3D12_PIPELINE_STATE_FLAG_NONE;                                 // �t���O
         // ���X�^���C�U(RS
         desc.RasterizerState.MultisampleEnable     = false;
         desc.RasterizerState.CullMode              = D3D12_CULL_MODE_NONE;   //�J�����O���Ȃ�
         desc.RasterizerState.FillMode              = D3D12_FILL_MODE_SOLID;
         desc.RasterizerState.DepthClipEnable       = true;
         desc.RasterizerState.FrontCounterClockwise = false;
         desc.RasterizerState.DepthBias             = D3D12_DEFAULT_DEPTH_BIAS;
         desc.RasterizerState.DepthBiasClamp        = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
         desc.RasterizerState.SlopeScaledDepthBias  = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
         desc.RasterizerState.AntialiasedLineEnable = false;
         desc.RasterizerState.ForcedSampleCount     = 0;
         desc.RasterizerState.ConservativeRaster    = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

         // TODO:�쐬���Blob�����̃����o�Ɏw�肷�邱�Ƃō����Đ����\
         desc.CachedPSO.pCachedBlob = nullptr;          // Blob�̐擪�A�h���X
         desc.CachedPSO.CachedBlobSizeInBytes = 0;      // Blob�̃o�C�g�T�C�Y
        
         //------------------------------------------------------
         // �p�C�v���C���X�e�[�g���쐬
         //------------------------------------------------------
         pipelineState_ = gpu::createPipelineState(desc);
         if(!pipelineState_) {
             return false;
         }
    }

    //----------------------------------------------------------
    // �R�}���h���X�g�̍쐬
    // �R�}���h���X�g�̓t���[���P�ʂ̃R�}���h�̐�����S��
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
    // �萔�o�b�t�@�̍쐬
    //----------------------------------------------------------
    {
        // GPU�������̊m��
        auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        auto resourceDesc   = CD3DX12_RESOURCE_DESC::Buffer(1024 * 64);                                      // 64KB

        if(FAILED(d3d12Device_->CreateCommittedResource(
               &heapProperties,                     // [in]  �������^�C�v�iUPLOAD�q�[�v�j
               D3D12_HEAP_FLAG_NONE,                // [in]�@�q�[�v�t���O�i�V�F�[�_�[����̃A�N�Z�����Ȃǁj
               &resourceDesc,                       // [in]  �m�ۂ��郁�����̏ڍאݒ�
               D3D12_RESOURCE_STATE_GENERIC_READ,   // [in]  �������̃X�e�[�g
               nullptr,                             // [in]  �������J���[�l
               IID_PPV_ARGS(&constantBuffer_)))) {  // [out] �m�ۂ��ꂽGPU������
            return false;
        }

        // �萔�o�b�t�@�̃������}�b�s���O
        // ���I�ȃo�b�t�@�ɂ��Ă̓A�v���P�[�V�����I���܂�Map�����܂ܗ��p���Ă��x�Ⴀ��܂���
        CD3DX12_RANGE readRange(0, 0); // CPU������ǂݎ��Ȃ����߂͈͎̔w��
        void*         data = nullptr;
        if(FAILED(constantBuffer_->Map(0, &readRange,&data))) {
            return false;
        }

        // �萔�o�b�t�@�̃T�C�Y��256�o�C�g���E�Ŏ擾
        auto size = calcConstantBufferByteSize(sizeof(SceneConstantBuffer));
        for(u32 i = 0; i < FRAME_COUNT; ++i) {
            // �萔�o�b�t�@View�̍쐬
            // CBV��Descrptor�q�[�v����쐬����K�v������
            D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
            desc.BufferLocation = constantBuffer_->GetGPUVirtualAddress() + size * i;   // GPU �A�h���X
            desc.SizeInBytes    = size;                                                 // �萔�o�b�t�@�T�C�Y��256�o�C�g���E

            d3d12Device_->CreateConstantBufferView(&desc, descriptorHeap(gpu::DESCRIPTOR_TYPE::CBV_SRV_UAV)->at(i));

            // �������R�s�[
            constantBufferTops_[i] = reinterpret_cast<void*>(reinterpret_cast<uintptr>(data) + size * i);
            memcpy(constantBufferTops_[i], &cb_, sizeof(cb_));
        }
    }

    //----------------------------------------------------------
    // PMD�̓ǂݍ���
    //----------------------------------------------------------
    pmd_ = gpu::createPMDFromFile("../../../directx12_resource/Model/�����~�N.pmd");

    fbx_ = gpu::createFBXFromFile("../../../directx12_resource/Model/Cerberus_LP.FBX");
    //pmd_->LoadVMDFile("../../../directx12_resource/motion/squat.vmd");

    //----------------------------------------------------------
    // �]���R�}���h���o�^���ꂽ�R�}���h���X�g�������Ŏ��s
    //----------------------------------------------------------
    if(FAILED(d3d12CommandList_->Close())) {
        return false;
    }
    ID3D12CommandList* commandLists[] = { d3d12CommandList_.Get() };
    auto*              commandQueue   = gpu::render()->commandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);
    commandQueue->ExecuteCommandLists(static_cast<UINT>(std::size(commandLists)), commandLists);

    // ���s�����܂Ńu���b�L���O�ł܂i������Ȃ��߉��P���K�v�j
    swapChain_->waitForGpu();

    return true;
}

//---------------------------------------------------------------------------
//! �X�V
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

    // �萔�o�b�t�@�̃������X�V
    memcpy(constantBufferTops_[frameIndex], &cb_, sizeof(cb_));
}

//---------------------------------------------------------------------------
//! �`��
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

    // �R�}���h���X�g�ւ̓o�^
    // �V�[���������_�����O���邽�߂ɕK�v�Ȃ��ׂẴR�}���h���R�}���h���X�g�ɐݒ�
    buildCommandList();

    //----------------------------------------------------------
    // �R�}���h���X�g�̎��s
    //----------------------------------------------------------
    ID3D12CommandList* commandList = d3d12CommandList_.Get();
    auto*              commandQueue = gpu::render()->commandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);
    commandQueue->ExecuteCommandLists(1, &commandList);

    //----------------------------------------------------------
    // �t���[���X�V
    //----------------------------------------------------------
    swapChain_->present(1);
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
void RenderImpl::finalize()
{
    // �m����GPU���f�X�g���N�^�ɂ���ăN���[���A�b�v����悤�Ƃ��Ă��郊�\�[�X��
    // �Q�Ƃ��Ȃ��悤�ɂ��邽�߂ɕ`�抮����҂�
    swapChain_->waitForGpu();
}

//---------------------------------------------------------------------------
//! �R�}���h���X�g�ւ̃R�}���h�o�^
//---------------------------------------------------------------------------
void RenderImpl::buildCommandList()
{
    auto frameIndex = swapChain_->frameIndex();

    // �R�}���h���X�g�̃A���P�[�^�[�͊֘A����R�}���h���X�g��
    // GPU��ł̎��s���I�������Ƃ��ɂ̂݃��Z�b�g�ł��܂��B
    if(FAILED(d3d12CommandAllocators_[frameIndex]->Reset())) {
        assert(0);
    }

    // �R�}���h���X�g�擾
    auto c = d3d12CommandList_.Get();

    // ����̃R�}���h���X�g�Ł@ExecuteCommandList() ���Ăяo���ꂽ�ꍇ
    // ���̃R�}���h���X�g�͍ė��p�O��Reset���Ă΂Ȃ���΂Ȃ�Ȃ�
    if(FAILED(c->Reset(d3d12CommandAllocators_[frameIndex].Get(), pipelineState_->d3dPipelineState()))) {
        assert(0);
    }

    //----------------------------------------------------------
    // �K�v�ȃX�e�[�g�̐ݒ�
    //----------------------------------------------------------
    // ���[�g�����ݒ�
    c->SetGraphicsRootSignature(d3d12RootSignature_.Get());

    // CBV/SRV�p�̃q�[�v��ݒ�
    // �q�[�v�͎�ނɂ�1�̐ݒ�BCBV/SRV/UAV�͋��L
    ID3D12DescriptorHeap* heap[]{ descriptorHeap(gpu::DESCRIPTOR_TYPE::CBV_SRV_UAV)->get() };
    c->SetDescriptorHeaps(static_cast<UINT>(std::size(heap)), heap);

    // CBV/SRV�p��DescriptorTable��ݒ�
    // ���̃X���b�g�̓��[�g�p�����[�^�[�ƑΉ�
    // [0] = CBV�pDescriptorTable�̃A�h���X
    // [1] = SRV�pDescriptorTable�̃A�h���X
    c->SetGraphicsRootDescriptorTable(0, descriptorHeap(gpu::DESCRIPTOR_TYPE::CBV_SRV_UAV)->at(frameIndex));   // [CBV] 0�`(FRAME_COUNT-1)
    c->SetGraphicsRootDescriptorTable(1, descriptorHeap(gpu::DESCRIPTOR_TYPE::CBV_SRV_UAV)->at(0));            // [SRV] FRAME_COUNT

    {
        CD3DX12_VIEWPORT viewport(0.0f, 0.0f, static_cast<f32>(width_), static_cast<f32>(height_), 0.0f, 1.0f);
        CD3DX12_RECT     scissorRect(0, 0, width_, height_);

        c->RSSetViewports(1, &viewport);         // �r���[�|�[�g
        c->RSSetScissorRects(1, &scissorRect);   // �V�U�����O��`
    }

    //----------------------------------------------------------
    // RenderTarget�̃��\�[�X�o���A
    // PRESENT��RENDER_TARGET�ւ̑J��
    //----------------------------------------------------------
    {
        // ���t���[���X�V��Present�����s���邽�߂ɂ͂����ŃX�e�[�g��PRESENT�ɑJ�ڂ��Ă���K�v����
        auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(swapChain_->backBuffer(frameIndex), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
        c->ResourceBarrier(1, &resourceBarrier);
    }

    //----------------------------------------------------------
    // �`��R�}���h�̓o�^
    //----------------------------------------------------------
    // ��RenderTarget�̐ݒ�̓X�e�[�g�����\�[�X�o���A�ŃX�e�[�g��RENDER_TARGET�ɂȂ��Ă���K�v����
    D3D12_CPU_DESCRIPTOR_HANDLE rtvs[]{ descriptorHeap(gpu::DESCRIPTOR_TYPE::RTV)->at(frameIndex) };
    D3D12_CPU_DESCRIPTOR_HANDLE dsvs[]{ descriptorHeap(gpu::DESCRIPTOR_TYPE::DSV)->at(0)};
    c->OMSetRenderTargets(1, rtvs, FALSE, dsvs);

    // �R�}���h�o�^
    const f32 clearColor[]{ 1.0f, 1.0f, 1.0f, 1.0f };
    c->ClearRenderTargetView(rtvs[0], clearColor, 0, nullptr);                        // RenderTarget�̃J���[�N���A
    c->ClearDepthStencilView(dsvs[0], D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);   // DepthStencil�̃N���A
    c->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);                   // �v���~�e�B�u�^�̐ݒ�(TRIANGLELIST) �����[�g�����Ńv���~�e�B�u��TRIANGLE�ɂȂ��Ă��邱��

    // pmd�`��
    pmd_->PMDRender(c);

    // imgui
    c->SetDescriptorHeaps(1, heapForImgui_.GetAddressOf());
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), c);

    //----------------------------------------------------------
    // RenderTarget�̃��\�[�X�o���A
    // RENDER_TARGET��PRESENT�ւ̑J��
    //----------------------------------------------------------
    {
        // ���t���[���X�V��Present�����s���邽�߂ɂ͂����ŃX�e�[�g��PRESENT�ɑJ�ڂ��Ă���K�v����
        auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(swapChain_->backBuffer(frameIndex), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
        c->ResourceBarrier(1, &resourceBarrier);
    }

    // �R�}���h���X�g�̓o�^����
    if(FAILED(c->Close())) {
        assert(0);
    }
}

//============================================================================================================
//! gpu::Render
//============================================================================================================
//---------------------------------------------------------------------------
//! ���̂��擾
//---------------------------------------------------------------------------
gpu::Render* Render::instance()
{
    return RenderImpl::instance();
}

//---------------------------------------------------------------------------
//! �����_�����O�Ǘ��N���X
//---------------------------------------------------------------------------
gpu::Render* render()
{
    return Render::instance();
}

//---------------------------------------------------------------------------
//! DXGIFactory���擾
//---------------------------------------------------------------------------
IDXGIFactory6* dxgiFactory()
{
    return dxgiFactory_.Get();
}

//---------------------------------------------------------------------------
//! D3D�f�o�C�X���擾
//---------------------------------------------------------------------------
ID3D12Device* d3dDevice()
{
    return d3d12Device_.Get();
}

//---------------------------------------------------------------------------
// DescriptorHeap���擾
//---------------------------------------------------------------------------
gpu::DescriptorHeap* descriptorHeap(DESCRIPTOR_TYPE type)
{
    return &descriptorHeaps[static_cast<size_t>(type)];
}

//---------------------------------------------------------------------------
// �C���f�b�N�X�A���P�[�^�[���擾
//---------------------------------------------------------------------------
IndexAllocator* indexAllocator(DESCRIPTOR_TYPE type)
{
    return &indexAllocators[static_cast<size_t>(type)];
}


}   // namespace gpu