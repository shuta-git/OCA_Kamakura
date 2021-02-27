//===========================================================================
//!	@file	SystemRender.cpp
//!	@brief	DirectX11昤夋僔僗僥儉娗棟
//===========================================================================
#include "SystemRender.h"

ID3D11Device*        gpD3DDevice  = nullptr;   // D3D僨僶僀僗
ID3D11DeviceContext* gpD3DContext = nullptr;   // D3D僐儞僥僉僗僩
gpu::Texture*        systemTexture_[SYSTEM_TEXTURE_MAX];

//---------------------------------------------------------------------------
//! 僔僗僥儉梡僥僋僗僠儍傪庢摼
//---------------------------------------------------------------------------
gpu::Texture* getSystemTexture(SYSTEM_TEXTURE type)
{
    return systemTexture_[type];
}

namespace gpu {

//! D3D僨僶僀僗傪庢摼
ID3D11Device* D3DDevice()
{
    return gpD3DDevice;
}

//! D3D僨僶僀僗僐儞僥僉僗僩傪庢摼
ID3D11DeviceContext* D3DContext()
{
    return gpD3DContext;
}

}   // namespace gpu

//===========================================================================
//	昤夋娗棟僔僗僥儉	SystemRender
//===========================================================================
SystemRender systemRender;

//! 儗儞僟儕儞僌僔僗僥儉傪庢摼
SystemRender* GmRender()
{
    return &systemRender;
}

//---------------------------------------------------------------------------
//! 僐儞僗僩儔僋僞
//---------------------------------------------------------------------------
SystemRender::SystemRender()
: driverType_(D3D_DRIVER_TYPE_UNKNOWN)    // 嶌惉偝傟偨僨僶僀僗偺庬椶			(HW, WARP, REF)
, featureLevel_(D3D_FEATURE_LEVEL_11_0)   // 嶌惉偝傟偨僨僶僀僗偺婡擻儗儀儖	(DX9,DX10,DX11)
{
}

//---------------------------------------------------------------------------
//! 僨僗僩儔僋僞
//---------------------------------------------------------------------------
SystemRender::~SystemRender()
{
    cleanup();
}

//---------------------------------------------------------------------------
//! 弶婜壔
//!	@param	[in]	width	暆
//!	@param	[in]	height	崅偝
//!	@param	[in]	hWnd	昤夋懳徾偺僂傿儞僪僂僴儞僪儖
//!	@retval	true	惓忢廔椆	乮惉岟乯
//!	@retval	false	僄儔乕廔椆	乮幐攕乯
//---------------------------------------------------------------------------
bool SystemRender::setup(u32 width, u32 height, HWND hWnd)
{
    HRESULT hr = S_OK;

    u32 createDeviceFlags = 0;
#if 1
    //=============================================================
    // 僨僶僢僌儗僀儎乕僨僶僀僗傪巊梡
    //=============================================================
    // 乮僷僼僅乕儅儞僗偲堷偒姺偊偵僄儔乕徻嵶儊僢僙乕僕弌椡傪峴偆乯
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    //-------------------------------------------------------------
    // 僨僶僀僗嶌惉偺偨傔偺僆僾僔儑儞愝掕
    //-------------------------------------------------------------
    DXGI_SWAP_CHAIN_DESC desc;
    DXGI_MODE_DESC&      buffer = desc.BufferDesc;

    buffer.Width                   = width;                                  // 夋柺夝憸搙(暆)
    buffer.Height                  = height;                                 // 夋柺夝憸搙(崅偝)
    buffer.RefreshRate.Numerator   = 60;                                     // 儕僼儗僢僔儏儗乕僩(暘巕) 60.0fps
    buffer.RefreshRate.Denominator = 1;                                      // 儕僼儗僢僔儏儗乕僩(暘曣)
    buffer.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;             // 僺僋僙儖僼僅乕儅僢僩
    buffer.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;   // 塮憸僗僉儍儞儔僀儞偺巜掕
    buffer.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;          // 夋憸僗働乕儕儞僌
    desc.SampleDesc.Count          = 1;                                      // 儅儖僠僒儞僾儖傾儞僠僄僀儕傾僔儞僌偺僒儞僾儖悢(1񎎚�8)
    desc.SampleDesc.Quality        = 0;                                      // 丂丂丂丂丂丂丂丂乂丂丂丂丂丂丂丂丂偺昳幙(0乣)
    desc.BufferCount               = 3;                                      // 僼儘儞僩僶僢僼傽傕娷傔偨僶僢僼傽悢 2:僟僽儖僶僢僼傽 3:僩儕僾儖僶僢僼傽
    desc.OutputWindow              = hWnd;                                   // 弌椡懳徾偺僂傿儞僪僂僴儞僪儖
    desc.Windowed                  = true;                                   // true:僂傿儞僪僂儌乕僪 false:僼儖僗僋儕乕儞儌乕僪
    desc.SwapEffect                = DXGI_SWAP_EFFECT_DISCARD;               // 夋柺峏怴儌乕僪

    //---- 僶僢僋僶僢僼傽偺梡搑
    desc.BufferUsage = DXGI_USAGE_SHADER_INPUT |           // 僥僋僗僠儍
                       DXGI_USAGE_RENDER_TARGET_OUTPUT |   // 昤夋僞乕僎僢僩
                       DXGI_USAGE_BACK_BUFFER;             // 僶僢僋僶僢僼傽

    //---- 僶僢僋僶僢僼傽偺摦嶌僼儔僌
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;   // 夝憸搙曄峏傪嫋壜

    //-------------------------------------------------------------
    // D3D僨僶僀僗嶌惉
    // 摨帪偵僶僢僋僶僢僼傽僗儚僢僾僠僃僀儞偲Immediate僐儞僥僉僗僩偑嶌惉偝傟傞
    //-------------------------------------------------------------
    //---- D3D僨僶僀僗弶婜壔偺帋峴弴彉(Hardware仺Software)
    static const std::array<D3D_DRIVER_TYPE, 3> driverTypes = {
        D3D_DRIVER_TYPE_HARDWARE,    // 僴乕僪僂僃傾GPU
        D3D_DRIVER_TYPE_WARP,        // 僜僼僩僂僃傾WARP僨僶僀僗 (Windows Advanced Rasterization Platform)
        D3D_DRIVER_TYPE_REFERENCE,   // 儕僼傽儗儞僗儔僗僞儔僀僓
    };

    //---- D3D僨僶僀僗偱僒億乕僩偡傞婡擻儗儀儖丅嶌惉帋峴弴彉
    static const std::array<D3D_FEATURE_LEVEL, 2> featureLevels = {
        D3D_FEATURE_LEVEL_11_0,   // DirectX 11.0 (僥僙儗乕僞乕, ComputeShader, 抶墑僐儞僥僉僗僩)
        D3D_FEATURE_LEVEL_10_0,   // DirectX 10.0	(僕僆儊僩儕僔僃乕僟乕, 僗僩儕乕儉弌椡)
    };

    // 僴乕僪僂僃傾僒億乕僩偟偰偄傞GPU偐傜弴偵嶌惉傪帋峴偟偰偄偔
    for(auto& type : driverTypes) {
        driverType_ = type;

        hr = D3D11CreateDeviceAndSwapChain(
            nullptr,                  //	[in]	懳徾偺僨傿僗僾儗僀傾僟僾僞GPU
            driverType_,              //	[in]	僨僶僀僗偺庬椶
            nullptr,                  //	[in]	僜僼僩僂僃傾儔僗僞儔僀僓
            createDeviceFlags,        //	[in]	僆僾僔儑儞僼儔僌
            &featureLevels[0],        //	[in]	帋峴偡傞婡擻儗儀儖偺攝楍
            featureLevels.size(),     //	[in]	丂丂丂丂乂丂丂丂丂丂攝楍悢
            D3D11_SDK_VERSION,        //	[in]	SDK僶乕僕儑儞 (D3D11_SDK_VERSION屌掕)
            &desc,                    //	[in]	僗儚僢僾僠僃僀儞偺愝掕僷僞儊乕僞
            &dxgiSwapChain_,          //	[out]	嶌惉偝傟偨僗儚僢僾僠僃僀儞
            &d3dDevice_,              //	[out]	嶌惉偝傟偨Direct3D僨僶僀僗
            &featureLevel_,           //	[out]	嶌惉偝傟偨僨僶僀僗偺婡擻儗儀儖
            &d3dImmediateContext_);   //	[out]	嶌惉偝傟偨僀儈僨傿僄僀僩僐儞僥僉僗僩

        //---- 惉岟偟偨応崌偼偦傟埲崀偺帋峴偼峴傢偢偵嶌惉偝傟偨僨僶僀僗傪棙梡丅
        if(hr == S_OK)
            break;
    }
    // 偡傋偰幐攕偟偨応崌偼僄儔乕
    if(hr != S_OK) {
        return false;
    }

    // 僌儘乕僶儖曄悢偵曐懚偟偰偍偔
    gpD3DDevice  = d3dDevice_.Get();
    gpD3DContext = d3dImmediateContext_.Get();

    //-------------------------------------------------------------
    //	昤夋僞乕僎僢僩偺嶌惉
    //-------------------------------------------------------------
    {
        // 僗儚僢僾僠僃僀儞偺拞偵偁傞僶僢僋僶僢僼傽儊儌儕乕傪庢摼
        ID3D11Texture2D* d3dRenderTarget = nullptr;
        hr                               = dxgiSwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&d3dRenderTarget);
        if(hr != S_OK)
            return false;

        backBuffer_.reset(gpu::createTexture(d3dRenderTarget));
    }
    // HDR僶僢僼傽偺嶌惉
    for(u32 i = 0; i < 2; ++i) {
        auto hdr = gpu::createTarget(width, height, DXGI_FORMAT_R11G11B10_FLOAT);
        hdrBuffer_[i].reset(hdr);
    }
    // SDR僶僢僼傽偺嶌惉
    for(u32 i = 0; i < 2; ++i) {
        auto sdr = gpu::createTarget(width, height, DXGI_FORMAT_R8G8B8A8_UNORM);
        sdrBuffer_[i].reset(sdr);
    }

    // HDR儚乕僋僶僢僼傽偺嶌惉
    for(auto& t : hdrWorkBuffer_) {
        for(u32 mip = 0; mip < std::size(t); ++mip) {
            u32 w = std::max(1u, width >> mip);
            u32 h = std::max(1u, height >> mip);

            auto hdr = gpu::createTarget(w, h, DXGI_FORMAT_R11G11B10_FLOAT);
            t[mip].reset(hdr);
        }
    }

    //--------------------------------------------------------------------------
    // 倅僶僢僼傽偺嶌惉
    //--------------------------------------------------------------------------
    {
        //---- GPU儊儌儕偺妋曐(Texture2D)
        D3D11_TEXTURE2D_DESC desc;

        desc.Width              = width;                                                   // 暆
        desc.Height             = height;                                                  // 崅偝
        desc.MipLevels          = 1;                                                       // 儈僢僾儗儀儖抜悢
        desc.ArraySize          = 1;                                                       // 攝楍悢
        desc.Format             = DXGI_FORMAT_R32_TYPELESS;                                // DXGI_FORMAT_D32_FLOAT 僺僋僙儖僼僅乕儅僢僩
        desc.SampleDesc.Count   = 1;                                                       // 儅儖僠僒儞僾儖僒儞僾儖悢
        desc.SampleDesc.Quality = 0;                                                       // 儅儖僠僒儞僾儖昳幙
        desc.Usage              = D3D11_USAGE_DEFAULT;                                     // 梡搑			(Z僶僢僼傽偼DEFAULT屌掕)
        desc.BindFlags          = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;   // 僶僀儞僪愝掕 (Z僶僢僼傽偵愝掕)
        desc.CPUAccessFlags     = 0;                                                       // CPU傾僋僙僗	(Z僶僢僼傽偼0屌掕)
        desc.MiscFlags          = 0;                                                       // 偦偺懠僆僾僔儑儞

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

    // 僔儍僪僂儅僢僾弶婜壔
    if(!shadowmap_.initialize(4096)) {
        MessageBox(nullptr, "僔儍僪僂儅僢僾偺弶婜壔偵幐攕偟傑偟偨丅", "ERROR", MB_OK);
        return false;
    }
    // GBuffer弶婜壔
    if(!gbuffer_.initialize(width, height)) {
        MessageBox(nullptr, "GBuffer偺弶婜壔偵幐攕偟傑偟偨丅", "ERROR", MB_OK);
        return false;
    }

    diffuseTexture_.reset(gpu::createTarget(width, height, DXGI_FORMAT_R11G11B10_FLOAT));
    specularTexture_.reset(gpu::createTarget(width, height, DXGI_FORMAT_R11G11B10_FLOAT));

    //-------------------------------------------------------------
    // 擖椡儗僀傾僂僩偺嶌惉
    //-------------------------------------------------------------
    // 捀揰峔憿懱偺僨乕僞乕峔憿傪掕媊偟偰GPU偵搉偡偨傔偺忣曬
    // 捀揰僔僃乕僟乕偲懳偵側傞忣曬丅
    // clang-format off
	D3D11_INPUT_ELEMENT_DESC layout[]{
        // 僙儅儞僥僢僋柤(擟堄)
        // 伀    僙儅儞僥僢僋斣崋(0乣7)
        // 伀          伀          僨乕僞宍幃     僗僩儕乕儉僗儘僢僩斣崋(0-15)
        // 伀          伀            伀                  伀   峔憿懱偺愭摢偐傜偺僆僼僙僢僩傾僪儗僗(愭摢偐傜n僶僀僩栚)
        // 伀          伀            伀                  伀                伀               捀揰撉傒崬傒偺峏怴廃婜     峏怴娫妘
        // 伀          伀            伀                  伀                伀                         伀                 伀
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, position_), D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR"   , 0, DXGI_FORMAT_R8G8B8A8_UNORM , 0, offsetof(Vertex, color_)   , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal_)  , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, tangent_) , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, binormal_), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT   , 0, offsetof(Vertex, uv_)      , D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    // clang-format on

    inputLayout_.reset(new gpu::InputLayout(layout));

    //---- 僥僋僗僠儍乕僒儞僾儔乕
    {
        D3D11_SAMPLER_DESC desc{};

        desc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;   // 奼戝丒弅彫丒儈僢僾傪僶僀儕僯傾曗娫 (TriLinearFilter)
        desc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;        // U曽岦 - 儕僺乕僩WRAP
        desc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;        // V曽岦 - 儕僺乕僩WRAP
        desc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;        // W曽岦 - 儕僺乕僩WRAP
        desc.ComparisonFunc = D3D11_COMPARISON_NEVER;            // 撉傒崬傫偩抣偲斾妑偡傞墘嶼
        desc.MinLOD         = 0;                                 // 嵟彫儈僢僾LOD
        desc.MaxLOD         = D3D11_FLOAT32_MAX;                 // 嵟戝儈僢僾LOD
        hr                  = d3dDevice_->CreateSamplerState(&desc, &d3dSamplerState_LinearWrap_);
        if(hr != S_OK) {
            return false;
        }

        desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;   // U曽岦 - CLAMP
        desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;   // V曽岦 - CLAMP
        desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;   // W曽岦 - CLAMP
        hr            = d3dDevice_->CreateSamplerState(&desc, &d3dSamplerState_LinearClamp_);
        if(hr != S_OK) {
            return false;
        }
    }

    //----------------------------------------------------------
    // 僽儗儞僪僗僥乕僩
    //----------------------------------------------------------
    {
        D3D11_BLEND_DESC desc{};

        desc.AlphaToCoverageEnable  = false;   // Alpha-to-coverage傪巊梡偡傞偐偳偆偐
        desc.IndependentBlendEnable = false;   // 屄暿偵僽儗儞僪儌乕僪愝掕傪偡傞偐偳偆偐

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

        // 捠忢僽儗儞僪
        rt.BlendEnable = true;
        d3dDevice_->CreateBlendState(&desc, &d3dBlendState_[BLEND_MODE_BLEND]);

        // 壛嶼僽儗儞僪
        rt.SrcBlend       = D3D11_BLEND_SRC_ALPHA;
        rt.DestBlend      = D3D11_BLEND_ONE;
        rt.SrcBlendAlpha  = D3D11_BLEND_SRC_ALPHA;
        rt.DestBlendAlpha = D3D11_BLEND_ONE;
        d3dDevice_->CreateBlendState(&desc, &d3dBlendState_[BLEND_MODE_ADD]);
    }

    // IBL僥僋僗僠儍
    iblDiffuse_.reset(gpu::createTexture("data/rnl_crossDiffuseHDR.dds"));
    iblSpecular_.reset(gpu::createTexture("data/rnl_crossSpecularHDR.dds"));

    return true;
}

//---------------------------------------------------------------------------
//! 夝曻
//---------------------------------------------------------------------------
void SystemRender::cleanup()
{
    //---- 僨僶僀僗僐儞僥僉僗僩偵僙僢僩偝傟偰偄傞僆僽僕僃僋僩傪偡傋偰夝彍偡傞丅
    if(d3dImmediateContext_) {
        d3dImmediateContext_->ClearState();
    }

    for(auto& texture : systemTexture_) {
        delete texture;
        texture = nullptr;
    }
}

//---------------------------------------------------------------------------
//! 昤夋奐巒
//---------------------------------------------------------------------------
void SystemRender::begin()
{
    isHDR_ = true;   // 偙傟埲崀偼HDR儌乕僪

    gpu::setInputLayout(inputLayout_);   // 擖椡儗僀傾僂僩

    gpu::D3DContext()->PSSetSamplers(0, 1, d3dSamplerState_LinearWrap_.GetAddressOf());    // 僥僋僗僠儍僒儞僾儔乕
    gpu::D3DContext()->PSSetSamplers(1, 1, d3dSamplerState_LinearClamp_.GetAddressOf());   // 僥僋僗僠儍僒儞僾儔乕

    // IBL僥僋僗僠儍
    gpu::setTexture(17, iblDiffuse_);
    gpu::setTexture(18, iblSpecular_);
}

//---------------------------------------------------------------------------
//! 昤夋廔椆
//---------------------------------------------------------------------------
void SystemRender::end()
{
    // 僨僾僗傪奜偡
    gpu::setRenderTarget(GmRender()->getRenderBuffer(), nullptr);
}

//---------------------------------------------------------------------------
//! 僔乕儞偺奐巒
//---------------------------------------------------------------------------
void SystemRender::beginScene()
{
    auto renderBuffer = GmRender()->getRenderBuffer();

    //-------------------------------------------------------------
    // 昤夋僞乕僎僢僩偲僨僾僗僶僢僼傽傪僋儕傾
    //-------------------------------------------------------------

    //---- 僇儔乕僶僢僼傽偺僇儔乕僋儕傾
    gpu::clearTarget(renderBuffer, { 0.5f, 0.5f, 0.8f, 1.0f });

    gpu::clearTarget(gbuffer_.getGBuffer(0), { 0, 1, 0, 1 });
    gpu::clearTarget(gbuffer_.getGBuffer(1), { 0, 0, 1, 1 });
    gpu::clearTarget(gbuffer_.getGBuffer(2), { 1, 0, 0, 1 });
    gpu::clearTarget(gbuffer_.getGBuffer(3), { 1, 1, 1, 1 });

    //---- 僨僾僗僶僢僼傽偺僋儕傾乮嵟墦宨偺1.0f偱弶婜壔丅倅斾妑偼LESSEQUAL傪巊梡乯
    gpu::clearTarget(depthStencil_);

    // 徠柧梡僥僋僗僠儍偺僋儕傾
    gpu::clearTarget(diffuseTexture_);
    gpu::clearTarget(specularTexture_);

    //-------------------------------------------------------------
    // 昤夋僞乕僎僢僩偺愝掕
    //-------------------------------------------------------------
    gbuffer_.begin();   // GBuffer奐巒
                        //gpu::setRenderTarget(renderBuffer, depthStencil_);

    // 僔儍僪僂梡僥僋僗僠儍偺愝掕
    gpu::setTexture(10, shadowmap_.getShadowTexture());
}

//---------------------------------------------------------------------------
//! 僔乕儞偺廔椆
//---------------------------------------------------------------------------
void SystemRender::endScene()
{
    // GBuffer廔椆
    gbuffer_.end();
}

//---------------------------------------------------------------------------
//! Imgui昤夋
//---------------------------------------------------------------------------
void SystemRender::imguiRender()
{
    shadowmap_.imguiRender();
    gbuffer_.imguiRender();
}

//---------------------------------------------------------------------------
//! 僇儊儔偺愝掕
//---------------------------------------------------------------------------
void SystemRender::setCamera(const Camera* camera)
{
    setCamera(camera->getViewMatrix(), camera->getProjMatrix());
}

//---------------------------------------------------------------------------
//! 僇儊儔偺愝掕(峴楍巜掕)
//---------------------------------------------------------------------------
void SystemRender::setCamera(const Matrix& view, const Matrix& proj)
{
    //-------------------------------------------------------------
    // 掕悢僶僢僼傽傪揮憲
    //-------------------------------------------------------------
    {
        auto p      = cbCamera_.begin();
        p->matView_ = view;
        p->matProj_ = proj;

        // 僔儍僪僂梡岝尮偐傜偺僇儊儔峴楍
        p->matLightView_ = shadowmap_.getViewMatrix();
        p->matLightProj_ = shadowmap_.getProjMatrix();

        // 媡峴楍曄姺傪偮偐偭偰儚乕儖僪嬻娫偵偁傞僇儊儔埵抲傪媮傔傞
        Vector3 eye;
        eye.x_ = -Vector3::dot(view.axisX(), view.translate());
        eye.y_ = -Vector3::dot(view.axisY(), view.translate());
        eye.z_ = -Vector3::dot(view.axisZ(), view.translate());

        p->cameraPosition_.x_ = eye.x_;
        p->cameraPosition_.y_ = eye.y_;
        p->cameraPosition_.z_ = eye.z_;

        cbCamera_.end();
    }
    gpu::setConstantBuffer(0, cbCamera_);   // 掕悢僶僢僼傽
}

//---------------------------------------------------------------------------
//!	夋柺峏怴
//! @param	[in]	interval	夋柺峏怴廃婜 1:60fps 2:30fps 3:20fps 4:15fps
//---------------------------------------------------------------------------
void SystemRender::swap(s32 interval)
{
    //------------------------------------------------------------
    // 昤夋僶僢僼傽傪僗儚僢僾僠僃僀儞偵斀塮
    //------------------------------------------------------------
    auto renderBuffer = GmRender()->getRenderBuffer();

    gpu::setRenderTarget(GmRender()->getBackBuffer());
    dxTexture(renderBuffer);

    // 僐僺乕僔僃乕僟乕愝掕
    gpu::setShader(shaderCopyVs_, shaderCopyPs_);

    put();

    //------------------------------------------------------------
    // 僶僢僋僶僢僼傽偲僼儘儞僩僶僢僼傽傪擖傟懼偊
    //------------------------------------------------------------
    dxgiSwapChain_->Present(interval, 0);
}

//---------------------------------------------------------------------------
//! 慡夋柺昤夋
//---------------------------------------------------------------------------
void SystemRender::put()
{
    // 慡夋柺僼傿儖
    // 嫄戝側嶰妏宍偱昤夋偡傞偙偲偱嫬奅慄偑柍偔側傝丄嵟戝岠棪偱儗儞僟儕儞僌偱偒傞
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
