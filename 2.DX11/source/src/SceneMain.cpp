#include "SceneMain.h"
//===========================================================================
//!	@file	SceneMain.h
//!	@brief	メインシーン
//===========================================================================

//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
SceneMain::SceneMain()
{
    initialize();
}

//---------------------------------------------------------------------------
//! デストラクタ
//---------------------------------------------------------------------------
SceneMain::~SceneMain()
{
	finalize();
}

//---------------------------------------------------------------------------
//! 初期化
//!	@retval	true	正常終了	(成功)
//!	@retval	false	エラー終了	(失敗)
//---------------------------------------------------------------------------
bool SceneMain::initialize()
{
    ID3D11Device* d3dDevice = gpu::D3DDevice();
    HRESULT       hr;

    shaderVs_ = gpu::ShaderVs::create("shader/vsModel.fx");
    shaderPs_ = gpu::ShaderPs::create("shader/psModel.fx");
    shaderCs_ = gpu::ShaderCs::create("shader/csGaussianBlur5x5_H.fx");

    //=============================================================
    // 頂点データ作成
    //=============================================================
    //if(setupGeometry_() == false) {
    //    return false;
    //}

    //-------------------------------------------------------------
    // テクスチャの読み込み
    //-------------------------------------------------------------
    texture_.reset(gpu::createTexture("seafloor.dds"));
    if (!texture_)
        return false;

    //cubeEnvironment_.reset(gpu::createTexture("data/uffizi_cross.dds"));
    cubeEnvironment_.reset(gpu::createTexture("data/rnl_cross.dds"));
    if (!cubeEnvironment_)
        return false;

    gpu::setTexture(16, cubeEnvironment_);

    //-------------------------------------------------------------
    // カメラ初期化
    //-------------------------------------------------------------
    camera_ = std::make_unique<CameraDebug>();
    camera_->setPosition({ 2.0f, 5.0f, 6.0f });
    camera_->setLookAt({ 0.0f, 0.0f, 0.0f });

    camera_->setNearZ(0.01f);
    camera_->setFarZ(1000.0f);

    //if (modelFbx_.load("data/Hero/hero.fbx",1.0f) == false) {
    //if (modelFbx_.load("data/Cerberus_by_Andrew_Maximov/Cerberus_LP.fbx",1.0f) == false) {
    //if (modelFbx_.load("data/Hero/hero1.fbx",0.03f) == false) {
    //if (modelFbx_.load("data/Hero/hero2.fbx",0.03f) == false) {
    //if (modelFbx_.load("data/Hero/hero3.fbx",0.03f) == false) {
    //if (modelFbx_.load("data/fps_hands/AK.FBX") == false) {
    //if (modelFbx_.load("../OpenSource/OpenFBX/runtime/a.FBX") == false) {
    modelFbx_ = gpu::loadFBX("data/Forest/Forest.FBX", 13.0f);
    if(modelFbx_ == false) {
        MessageBox(nullptr, "fbxモデルの読み込みに失敗しました.", nullptr, MB_OK);
    }

    modelFbx_->setRotate(Vector3(90, 0, 0));

    modelFbx2_ = gpu::loadFBX("data/Hero/hero.fbx", 1.0f);
    if(modelFbx2_ == false) {
        MessageBox(nullptr, "fbxモデルの読み込みに失敗しました.", nullptr, MB_OK);
    }

    return true;
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void SceneMain::update()
{ 
    //-------------------------------------------------------------
    // カメラの更新
    //-------------------------------------------------------------
    camera_->update();

    //-------------------------------------------------------------
    // ワールド行列
    //-------------------------------------------------------------

    // 回転テスト
    static f32 t = 0.0f;
    //  t += 0.02f;

    //---- 行列更新
    matWorld_ = Matrix::rotateY(t);
}

//---------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------
void SceneMain::render(RenderMode mode)
{
    ID3D11DeviceContext* d3dContext = gpu::D3DContext();

    if (mode == RenderMode::Scene) {
        //-------------------------------------------------------------
        // シーン用のカメラ行列を設定
        //-------------------------------------------------------------
        GmRender()->setCamera(camera_.get());
    }

    modelFbx_->render();
    modelFbx2_->render();

    //=============================================================
    // ComputeShaderのテスト実行
    //=============================================================
    if (mode == RenderMode::Scene) {
        gpu::setRenderTarget(nullptr);	// テクスチャでアクセスするために衝突を避けるためにいったん外す

        auto* srcTexture = GmRender()->getRenderBuffer(0);
        auto* dstTexture = GmRender()->getRenderBuffer(1);

        gpu::setShader(shaderCs_);
        gpu::setTexture(0, srcTexture);
        gpu::setRWTexture(0, dstTexture);

        u32 numthreadsX = 256;   // HLSL内のスレッド数定義
        u32 numthreadsY = 1;

        gpu::dispatch((srcTexture->getWidth() + numthreadsX - 1) / numthreadsX,   // 端数分もスレッドグループに含める
            (srcTexture->getHeight() + numthreadsY - 1) / numthreadsY,
            1);

        GmRender()->swapRenderBuffer();

        gpu::setTexture(0, nullptr);
        gpu::setRWTexture(0, nullptr);

        // RWTextureに設定したことで外れたRenderTargetを再セット
        // ※MRTになっているため要修正
        gpu::setRenderTarget(GmRender()->getRenderBuffer(), GmRender()->getDepthStencil());
    }
}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void SceneMain::finalize()
{
}

//---------------------------------------------------------------------------
//! imgui描画
//---------------------------------------------------------------------------
void SceneMain::imguiRender()
{
}
