//===========================================================================
//!	@file	SystemRender.cpp
//!	@brief	DirectX11�`��V�X�e���Ǘ�
//===========================================================================
#include "SystemRender.h"

ID3D11Device*        gpD3DDevice  = nullptr;   // D3D�f�o�C�X
ID3D11DeviceContext* gpD3DContext = nullptr;   // D3D�R���e�L�X�g
gpu::Texture*        systemTexture_[SYSTEM_TEXTURE_MAX];

//---------------------------------------------------------------------------
//! �V�X�e���p�e�N�X�`�����擾
//---------------------------------------------------------------------------
gpu::Texture* getSystemTexture(SYSTEM_TEXTURE type)
{
    return systemTexture_[type];
}

namespace gpu {

//! D3D�f�o�C�X���擾
ID3D11Device* D3DDevice()
{
    return gpD3DDevice;
}

//! D3D�f�o�C�X�R���e�L�X�g���擾
ID3D11DeviceContext* D3DContext()
{
    return gpD3DContext;
}

}   // namespace gpu

//===========================================================================
//	�`��Ǘ��V�X�e��	SystemRender
//===========================================================================
SystemRender systemRender;

//! �����_�����O�V�X�e�����擾
SystemRender* GmRender()
{
    return &systemRender;
}

//---------------------------------------------------------------------------
//! �R���X�g���N�^
//---------------------------------------------------------------------------
SystemRender::SystemRender()
: driverType_(D3D_DRIVER_TYPE_UNKNOWN)    // �쐬���ꂽ�f�o�C�X�̎��			(HW, WARP, REF)
, featureLevel_(D3D_FEATURE_LEVEL_11_0)   // �쐬���ꂽ�f�o�C�X�̋@�\���x��	(DX9,DX10,DX11)
{
}

//---------------------------------------------------------------------------
//! �f�X�g���N�^
//---------------------------------------------------------------------------
SystemRender::~SystemRender()
{
    cleanup();
}

//---------------------------------------------------------------------------
//! ������
//!	@param	[in]	width	��
//!	@param	[in]	height	����
//!	@param	[in]	hWnd	�`��Ώۂ̃E�B���h�E�n���h��
//!	@retval	true	����I��	�i�����j
//!	@retval	false	�G���[�I��	�i���s�j
//---------------------------------------------------------------------------
bool SystemRender::setup(u32 width, u32 height, HWND hWnd)
{
    HRESULT hr = S_OK;

    u32 createDeviceFlags = 0;
#if 1
    //=============================================================
    // �f�o�b�O���C���[�f�o�C�X���g�p
    //=============================================================
    // �i�p�t�H�[�}���X�ƈ��������ɃG���[�ڍ׃��b�Z�[�W�o�͂��s���j
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    //-------------------------------------------------------------
    // �f�o�C�X�쐬�̂��߂̃I�v�V�����ݒ�
    //-------------------------------------------------------------
    DXGI_SWAP_CHAIN_DESC desc;
    DXGI_MODE_DESC&      buffer = desc.BufferDesc;

    buffer.Width                   = width;                                  // ��ʉ𑜓x(��)
    buffer.Height                  = height;                                 // ��ʉ𑜓x(����)
    buffer.RefreshRate.Numerator   = 60;                                     // ���t���b�V�����[�g(���q) 60.0fps
    buffer.RefreshRate.Denominator = 1;                                      // ���t���b�V�����[�g(����)
    buffer.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;             // �s�N�Z���t�H�[�}�b�g
    buffer.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;   // �f���X�L�������C���̎w��
    buffer.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;          // �摜�X�P�[�����O
    desc.SampleDesc.Count          = 1;                                      // �}���`�T���v���A���`�G�C���A�V���O�̃T���v����(1�2�4�8)
    desc.SampleDesc.Quality        = 0;                                      // �@�@�@�@�@�@�@�@�V�@�@�@�@�@�@�@�@�̕i��(0�`)
    desc.BufferCount               = 3;                                      // �t�����g�o�b�t�@���܂߂��o�b�t�@�� 2:�_�u���o�b�t�@ 3:�g���v���o�b�t�@
    desc.OutputWindow              = hWnd;                                   // �o�͑Ώۂ̃E�B���h�E�n���h��
    desc.Windowed                  = true;                                   // true:�E�B���h�E���[�h false:�t���X�N���[�����[�h
    desc.SwapEffect                = DXGI_SWAP_EFFECT_DISCARD;               // ��ʍX�V���[�h

    //---- �o�b�N�o�b�t�@�̗p�r
    desc.BufferUsage = DXGI_USAGE_SHADER_INPUT |           // �e�N�X�`��
                       DXGI_USAGE_RENDER_TARGET_OUTPUT |   // �`��^�[�Q�b�g
                       DXGI_USAGE_BACK_BUFFER;             // �o�b�N�o�b�t�@

    //---- �o�b�N�o�b�t�@�̓���t���O
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;   // �𑜓x�ύX������

    //-------------------------------------------------------------
    // D3D�f�o�C�X�쐬
    // �����Ƀo�b�N�o�b�t�@�X���b�v�`�F�C����Immediate�R���e�L�X�g���쐬�����
    //-------------------------------------------------------------
    //---- D3D�f�o�C�X�������̎��s����(Hardware��Software)
    static const std::array<D3D_DRIVER_TYPE, 3> driverTypes = {
        D3D_DRIVER_TYPE_HARDWARE,    // �n�[�h�E�F�AGPU
        D3D_DRIVER_TYPE_WARP,        // �\�t�g�E�F�AWARP�f�o�C�X (Windows Advanced Rasterization Platform)
        D3D_DRIVER_TYPE_REFERENCE,   // ���t�@�����X���X�^���C�U
    };

    //---- D3D�f�o�C�X�ŃT�|�[�g����@�\���x���B�쐬���s����
    static const std::array<D3D_FEATURE_LEVEL, 2> featureLevels = {
        D3D_FEATURE_LEVEL_11_0,   // DirectX 11.0 (�e�Z���[�^�[, ComputeShader, �x���R���e�L�X�g)
        D3D_FEATURE_LEVEL_10_0,   // DirectX 10.0	(�W�I���g���V�F�[�_�[, �X�g���[���o��)
    };

    // �n�[�h�E�F�A�T�|�[�g���Ă���GPU���珇�ɍ쐬�����s���Ă���
    for(auto& type : driverTypes) {
        driverType_ = type;

        hr = D3D11CreateDeviceAndSwapChain(
            nullptr,                  //	[in]	�Ώۂ̃f�B�X�v���C�A�_�v�^GPU
            driverType_,              //	[in]	�f�o�C�X�̎��
            nullptr,                  //	[in]	�\�t�g�E�F�A���X�^���C�U
            createDeviceFlags,        //	[in]	�I�v�V�����t���O
            &featureLevels[0],        //	[in]	���s����@�\���x���̔z��
            featureLevels.size(),     //	[in]	�@�@�@�@�V�@�@�@�@�@�z��
            D3D11_SDK_VERSION,        //	[in]	SDK�o�[�W���� (D3D11_SDK_VERSION�Œ�)
            &desc,                    //	[in]	�X���b�v�`�F�C���̐ݒ�p�^���[�^
            &dxgiSwapChain_,          //	[out]	�쐬���ꂽ�X���b�v�`�F�C��
            &d3dDevice_,              //	[out]	�쐬���ꂽDirect3D�f�o�C�X
            &featureLevel_,           //	[out]	�쐬���ꂽ�f�o�C�X�̋@�\���x��
            &d3dImmediateContext_);   //	[out]	�쐬���ꂽ�C�~�f�B�G�C�g�R���e�L�X�g

        //---- ���������ꍇ�͂���ȍ~�̎��s�͍s�킸�ɍ쐬���ꂽ�f�o�C�X�𗘗p�B
        if(hr == S_OK)
            break;
    }
    // ���ׂĎ��s�����ꍇ�̓G���[
    if(hr != S_OK) {
        return false;
    }

    // �O���[�o���ϐ��ɕۑ����Ă���
    gpD3DDevice  = d3dDevice_.Get();
    gpD3DContext = d3dImmediateContext_.Get();

    //-------------------------------------------------------------
    //	�`��^�[�Q�b�g�̍쐬
    //-------------------------------------------------------------
    {
        // �X���b�v�`�F�C���̒��ɂ���o�b�N�o�b�t�@�������[���擾
        ID3D11Texture2D* d3dRenderTarget = nullptr;
        hr                               = dxgiSwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&d3dRenderTarget);
        if(hr != S_OK)
            return false;

        backBuffer_.reset(gpu::createTexture(d3dRenderTarget));
    }
    // HDR�o�b�t�@�̍쐬
    for(u32 i = 0; i < 2; ++i) {
        auto hdr = gpu::createTarget(width, height, DXGI_FORMAT_R11G11B10_FLOAT);
        hdrBuffer_[i].reset(hdr);
    }
    // SDR�o�b�t�@�̍쐬
    for(u32 i = 0; i < 2; ++i) {
        auto sdr = gpu::createTarget(width, height, DXGI_FORMAT_R8G8B8A8_UNORM);
        sdrBuffer_[i].reset(sdr);
    }

    // HDR���[�N�o�b�t�@�̍쐬
    for(auto& t : hdrWorkBuffer_) {
        for(u32 mip = 0; mip < std::size(t); ++mip) {
            u32 w = std::max(1u, width >> mip);
            u32 h = std::max(1u, height >> mip);

            auto hdr = gpu::createTarget(w, h, DXGI_FORMAT_R11G11B10_FLOAT);
            t[mip].reset(hdr);
        }
    }

    //--------------------------------------------------------------------------
    // �y�o�b�t�@�̍쐬
    //--------------------------------------------------------------------------
    {
        //---- GPU�������̊m��(Texture2D)
        D3D11_TEXTURE2D_DESC desc;

        desc.Width              = width;                                                   // ��
        desc.Height             = height;                                                  // ����
        desc.MipLevels          = 1;                                                       // �~�b�v���x���i��
        desc.ArraySize          = 1;                                                       // �z��
        desc.Format             = DXGI_FORMAT_R32_TYPELESS;                                // DXGI_FORMAT_D32_FLOAT �s�N�Z���t�H�[�}�b�g
        desc.SampleDesc.Count   = 1;                                                       // �}���`�T���v���T���v����
        desc.SampleDesc.Quality = 0;                                                       // �}���`�T���v���i��
        desc.Usage              = D3D11_USAGE_DEFAULT;                                     // �p�r			(Z�o�b�t�@��DEFAULT�Œ�)
        desc.BindFlags          = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;   // �o�C���h�ݒ� (Z�o�b�t�@�ɐݒ�)
        desc.CPUAccessFlags     = 0;                                                       // CPU�A�N�Z�X	(Z�o�b�t�@��0�Œ�)
        desc.MiscFlags          = 0;                                                       // ���̑��I�v�V����

        ID3D11Texture2D* d3dTexture2D = nullptr;
        hr                            = gpu::D3DDevice()->CreateTexture2D(&desc, nullptr, &d3dTexture2D);
        if(hr != S_OK)
            return false;

        depthStencil_.reset(gpu::createTexture(d3dTexture2D));
    }

    systemTexture_[SYSTEM_TEXTURE_NULL_WHITE] = gpu::createTexture("data/NullWhite.dds");
    systemTexture_[SYSTEM_TEXTURE_NULL_BLACK] = gpu::createTexture("data/NullBlack.dds");

    shaderCopyVs_ = gpu::ShaderVs::create("shader/vsPrim2D.fx");
    shaderCopyPs_ = gpu::ShaderPs::create("shader/psTexture.fx");

    // �V���h�E�}�b�v������
    if(!shadowmap_.initialize(4096)) {
        MessageBox(nullptr, "�V���h�E�}�b�v�̏������Ɏ��s���܂����B", "ERROR", MB_OK);
        return false;
    }
    // GBuffer������
    if(!gbuffer_.initialize(width, height)) {
        MessageBox(nullptr, "GBuffer�̏������Ɏ��s���܂����B", "ERROR", MB_OK);
        return false;
    }

    diffuseTexture_.reset(gpu::createTarget(width, height, DXGI_FORMAT_R11G11B10_FLOAT));
    specularTexture_.reset(gpu::createTarget(width, height, DXGI_FORMAT_R11G11B10_FLOAT));

    //-------------------------------------------------------------
    // ���̓��C�A�E�g�̍쐬
    //-------------------------------------------------------------
    // ���_�\���̂̃f�[�^�[�\�����`����GPU�ɓn�����߂̏��
    // ���_�V�F�[�_�[�Ƒ΂ɂȂ���B
    // clang-format off
	D3D11_INPUT_ELEMENT_DESC layout[]{
        // �Z�}���e�b�N��(�C��)
        // ��    �Z�}���e�b�N�ԍ�(0�`7)
        // ��          ��          �f�[�^�`��     �X�g���[���X���b�g�ԍ�(0-15)
        // ��          ��            ��                  ��   �\���̂̐擪����̃I�t�Z�b�g�A�h���X(�擪����n�o�C�g��)
        // ��          ��            ��                  ��                ��               ���_�ǂݍ��݂̍X�V����     �X�V�Ԋu
        // ��          ��            ��                  ��                ��                         ��                 ��
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, position_), D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR"   , 0, DXGI_FORMAT_R8G8B8A8_UNORM , 0, offsetof(Vertex, color_)   , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal_)  , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, tangent_) , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, binormal_), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT   , 0, offsetof(Vertex, uv_)      , D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    // clang-format on

    inputLayout_.reset(new gpu::InputLayout(layout));

    //---- �e�N�X�`���[�T���v���[
    {
        D3D11_SAMPLER_DESC desc{};

        desc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;   // �g��E�k���E�~�b�v���o�C���j�A��� (TriLinearFilter)
        desc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;        // U���� - ���s�[�gWRAP
        desc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;        // V���� - ���s�[�gWRAP
        desc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;        // W���� - ���s�[�gWRAP
        desc.ComparisonFunc = D3D11_COMPARISON_NEVER;            // �ǂݍ��񂾒l�Ɣ�r���鉉�Z
        desc.MinLOD         = 0;                                 // �ŏ��~�b�vLOD
        desc.MaxLOD         = D3D11_FLOAT32_MAX;                 // �ő�~�b�vLOD
        hr                  = d3dDevice_->CreateSamplerState(&desc, &d3dSamplerState_LinearWrap_);
        if(hr != S_OK) {
            return false;
        }

        desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;   // U���� - CLAMP
        desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;   // V���� - CLAMP
        desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;   // W���� - CLAMP
        hr            = d3dDevice_->CreateSamplerState(&desc, &d3dSamplerState_LinearClamp_);
        if(hr != S_OK) {
            return false;
        }
    }

    //----------------------------------------------------------
    // �u�����h�X�e�[�g
    //----------------------------------------------------------
    {
        D3D11_BLEND_DESC desc{};

        desc.AlphaToCoverageEnable  = false;   // Alpha-to-coverage���g�p���邩�ǂ���
        desc.IndependentBlendEnable = false;   // �ʂɃu�����h���[�h�ݒ�����邩�ǂ���

        auto& rt = desc.RenderTarget[0];

        rt.BlendEnable           = false;
        rt.SrcBlend              = D3D11_BLEND_SRC_ALPHA;
        rt.DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
        rt.BlendOp               = D3D11_BLEND_OP_ADD;
        rt.SrcBlendAlpha         = D3D11_BLEND_SRC_ALPHA;
        rt.DestBlendAlpha        = D3D11_BLEND_INV_SRC_ALPHA;
        rt.BlendOpAlpha          = D3D11_BLEND_OP_ADD;
        rt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        d3dDevice_->CreateBlendState(&desc, &d3dBlendState_[BLEND_MODE_OFF]);

        // �ʏ�u�����h
        rt.BlendEnable = true;
        d3dDevice_->CreateBlendState(&desc, &d3dBlendState_[BLEND_MODE_BLEND]);

        // ���Z�u�����h
        rt.SrcBlend       = D3D11_BLEND_SRC_ALPHA;
        rt.DestBlend      = D3D11_BLEND_ONE;
        rt.SrcBlendAlpha  = D3D11_BLEND_SRC_ALPHA;
        rt.DestBlendAlpha = D3D11_BLEND_ONE;
        d3dDevice_->CreateBlendState(&desc, &d3dBlendState_[BLEND_MODE_ADD]);
    }

    // IBL�e�N�X�`��
    iblDiffuse_.reset(gpu::createTexture("data/rnl_crossDiffuseHDR.dds"));
    iblSpecular_.reset(gpu::createTexture("data/rnl_crossSpecularHDR.dds"));

    return true;
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
void SystemRender::cleanup()
{
    //---- �f�o�C�X�R���e�L�X�g�ɃZ�b�g����Ă���I�u�W�F�N�g�����ׂĉ�������B
    if(d3dImmediateContext_) {
        d3dImmediateContext_->ClearState();
    }

    for(auto& texture : systemTexture_) {
        delete texture;
        texture = nullptr;
    }
}

//---------------------------------------------------------------------------
//! �`��J�n
//---------------------------------------------------------------------------
void SystemRender::begin()
{
    isHDR_ = true;   // ����ȍ~��HDR���[�h

    gpu::setInputLayout(inputLayout_);   // ���̓��C�A�E�g

    gpu::D3DContext()->PSSetSamplers(0, 1, d3dSamplerState_LinearWrap_.GetAddressOf());    // �e�N�X�`���T���v���[
    gpu::D3DContext()->PSSetSamplers(1, 1, d3dSamplerState_LinearClamp_.GetAddressOf());   // �e�N�X�`���T���v���[

    // IBL�e�N�X�`��
    gpu::setTexture(17, iblDiffuse_);
    gpu::setTexture(18, iblSpecular_);
}

//---------------------------------------------------------------------------
//! �`��I��
//---------------------------------------------------------------------------
void SystemRender::end()
{
    // �f�v�X���O��
    gpu::setRenderTarget(GmRender()->getRenderBuffer(), nullptr);
}

//---------------------------------------------------------------------------
//! �V�[���̊J�n
//---------------------------------------------------------------------------
void SystemRender::beginScene()
{
    auto renderBuffer = GmRender()->getRenderBuffer();

    //-------------------------------------------------------------
    // �`��^�[�Q�b�g�ƃf�v�X�o�b�t�@���N���A
    //-------------------------------------------------------------

    //---- �J���[�o�b�t�@�̃J���[�N���A
    gpu::clearTarget(renderBuffer, { 0.5f, 0.5f, 0.8f, 1.0f });

    gpu::clearTarget(gbuffer_.getGBuffer(0), { 0, 1, 0, 1 });
    gpu::clearTarget(gbuffer_.getGBuffer(1), { 0, 0, 1, 1 });
    gpu::clearTarget(gbuffer_.getGBuffer(2), { 1, 0, 0, 1 });
    gpu::clearTarget(gbuffer_.getGBuffer(3), { 1, 1, 1, 1 });

    //---- �f�v�X�o�b�t�@�̃N���A�i�ŉ��i��1.0f�ŏ������B�y��r��LESSEQUAL���g�p�j
    gpu::clearTarget(depthStencil_);

    // �Ɩ��p�e�N�X�`���̃N���A
    gpu::clearTarget(diffuseTexture_);
    gpu::clearTarget(specularTexture_);

    //-------------------------------------------------------------
    // �`��^�[�Q�b�g�̐ݒ�
    //-------------------------------------------------------------
    gbuffer_.begin();   // GBuffer�J�n
                        //gpu::setRenderTarget(renderBuffer, depthStencil_);

    // �V���h�E�p�e�N�X�`���̐ݒ�
    gpu::setTexture(10, shadowmap_.getShadowTexture());
}

//---------------------------------------------------------------------------
//! �V�[���̏I��
//---------------------------------------------------------------------------
void SystemRender::endScene()
{
    // GBuffer�I��
    gbuffer_.end();
}

//---------------------------------------------------------------------------
//! Imgui�`��
//---------------------------------------------------------------------------
void SystemRender::imguiRender()
{
    shadowmap_.imguiRender();
    gbuffer_.imguiRender();
}

//---------------------------------------------------------------------------
//! �J�����̐ݒ�
//---------------------------------------------------------------------------
void SystemRender::setCamera(const Camera* camera)
{
    setCamera(camera->getViewMatrix(), camera->getProjMatrix());
}

//---------------------------------------------------------------------------
//! �J�����̐ݒ�(�s��w��)
//---------------------------------------------------------------------------
void SystemRender::setCamera(const Matrix& view, const Matrix& proj)
{
    //-------------------------------------------------------------
    // �萔�o�b�t�@��]��
    //-------------------------------------------------------------
    {
        auto p      = cbCamera_.begin();
        p->matView_ = view;
        p->matProj_ = proj;

        // �V���h�E�p��������̃J�����s��
        p->matLightView_ = shadowmap_.getViewMatrix();
        p->matLightProj_ = shadowmap_.getProjMatrix();

        // �t�s��ϊ��������ă��[���h��Ԃɂ���J�����ʒu�����߂�
        Vector3 eye;
        eye.x_ = -Vector3::dot(view.axisX(), view.translate());
        eye.y_ = -Vector3::dot(view.axisY(), view.translate());
        eye.z_ = -Vector3::dot(view.axisZ(), view.translate());

        p->cameraPosition_.x_ = eye.x_;
        p->cameraPosition_.y_ = eye.y_;
        p->cameraPosition_.z_ = eye.z_;

        cbCamera_.end();
    }
    gpu::setConstantBuffer(0, cbCamera_);   // �萔�o�b�t�@
}

//---------------------------------------------------------------------------
//!	��ʍX�V
//! @param	[in]	interval	��ʍX�V���� 1:60fps 2:30fps 3:20fps 4:15fps
//---------------------------------------------------------------------------
void SystemRender::swap(s32 interval)
{
    //------------------------------------------------------------
    // �`��o�b�t�@���X���b�v�`�F�C���ɔ��f
    //------------------------------------------------------------
    auto renderBuffer = GmRender()->getRenderBuffer();

    gpu::setRenderTarget(GmRender()->getBackBuffer());
    dxTexture(renderBuffer);

    // �R�s�[�V�F�[�_�[�ݒ�
    gpu::setShader(shaderCopyVs_, shaderCopyPs_);

    put();

    //------------------------------------------------------------
    // �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�����ւ�
    //------------------------------------------------------------
    dxgiSwapChain_->Present(interval, 0);
}

//---------------------------------------------------------------------------
//! �S��ʕ`��
//---------------------------------------------------------------------------
void SystemRender::put()
{
    // �S��ʃt�B��
    // ����ȎO�p�`�ŕ`�悷�邱�Ƃŋ��E���������Ȃ�A�ő�����Ń����_�����O�ł���
    dxBegin(PT_TRIANGLES);
    {
        dxColor4ub(255, 255, 255, 255);

        dxTexCoord2f(0, 1);
        dxVertex3f(-1, -1, 0);

        dxTexCoord2f(0, -1);
        dxVertex3f(-1, +3, 0);

        dxTexCoord2f(2, 1);
        dxVertex3f(+3, -1, 0);
    }
    dxEnd();
}

//===========================================================================
//	END OF fILE
//===========================================================================
