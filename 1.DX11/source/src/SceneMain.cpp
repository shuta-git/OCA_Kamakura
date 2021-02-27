#include "SceneMain.h"
//===========================================================================
//!	@file	SceneMain.h
//!	@brief	���C���V�[��
//===========================================================================

//---------------------------------------------------------------------------
//! �R���X�g���N�^
//---------------------------------------------------------------------------
SceneMain::SceneMain()
{
    initialize();
}

//---------------------------------------------------------------------------
//! �f�X�g���N�^
//---------------------------------------------------------------------------
SceneMain::~SceneMain()
{
	finalize();
}

//---------------------------------------------------------------------------
//! ������
//!	@retval	true	����I��	(����)
//!	@retval	false	�G���[�I��	(���s)
//---------------------------------------------------------------------------
bool SceneMain::initialize()
{
    ID3D11Device* d3dDevice = gpu::D3DDevice();
    HRESULT       hr;

    shaderVs_ = gpu::ShaderVs::create("shader/vsModel.fx");
    shaderPs_ = gpu::ShaderPs::create("shader/psModel.fx");
    shaderCs_ = gpu::ShaderCs::create("shader/csGaussianBlur5x5_H.fx");

    //=============================================================
    // ���_�f�[�^�쐬
    //=============================================================
    //if(setupGeometry_() == false) {
    //    return false;
    //}

    //-------------------------------------------------------------
    // �e�N�X�`���̓ǂݍ���
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
    // �J����������
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
        MessageBox(nullptr, "fbx���f���̓ǂݍ��݂Ɏ��s���܂���.", nullptr, MB_OK);
    }

    modelFbx_->setRotate(Vector3(90, 0, 0));

    modelFbx2_ = gpu::loadFBX("data/Hero/hero.fbx", 1.0f);
    if(modelFbx2_ == false) {
        MessageBox(nullptr, "fbx���f���̓ǂݍ��݂Ɏ��s���܂���.", nullptr, MB_OK);
    }

    return true;
}

//---------------------------------------------------------------------------
//! �X�V
//---------------------------------------------------------------------------
void SceneMain::update()
{ 
    //-------------------------------------------------------------
    // �J�����̍X�V
    //-------------------------------------------------------------
    camera_->update();

    //-------------------------------------------------------------
    // ���[���h�s��
    //-------------------------------------------------------------

    // ��]�e�X�g
    static f32 t = 0.0f;
    //  t += 0.02f;

    //---- �s��X�V
    matWorld_ = Matrix::rotateY(t);
}

//---------------------------------------------------------------------------
//! �`��
//---------------------------------------------------------------------------
void SceneMain::render(RenderMode mode)
{
    ID3D11DeviceContext* d3dContext = gpu::D3DContext();

    if (mode == RenderMode::Scene) {
        //-------------------------------------------------------------
        // �V�[���p�̃J�����s���ݒ�
        //-------------------------------------------------------------
        GmRender()->setCamera(camera_.get());
    }

    modelFbx_->render();
    modelFbx2_->render();

    //=============================================================
    // ComputeShader�̃e�X�g���s
    //=============================================================
    if (mode == RenderMode::Scene) {
        gpu::setRenderTarget(nullptr);	// �e�N�X�`���ŃA�N�Z�X���邽�߂ɏՓ˂�����邽�߂ɂ�������O��

        auto* srcTexture = GmRender()->getRenderBuffer(0);
        auto* dstTexture = GmRender()->getRenderBuffer(1);

        gpu::setShader(shaderCs_);
        gpu::setTexture(0, srcTexture);
        gpu::setRWTexture(0, dstTexture);

        u32 numthreadsX = 256;   // HLSL���̃X���b�h����`
        u32 numthreadsY = 1;

        gpu::dispatch((srcTexture->getWidth() + numthreadsX - 1) / numthreadsX,   // �[�������X���b�h�O���[�v�Ɋ܂߂�
            (srcTexture->getHeight() + numthreadsY - 1) / numthreadsY,
            1);

        GmRender()->swapRenderBuffer();

        gpu::setTexture(0, nullptr);
        gpu::setRWTexture(0, nullptr);

        // RWTexture�ɐݒ肵�����ƂŊO�ꂽRenderTarget���ăZ�b�g
        // ��MRT�ɂȂ��Ă��邽�ߗv�C��
        gpu::setRenderTarget(GmRender()->getRenderBuffer(), GmRender()->getDepthStencil());
    }
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
void SceneMain::finalize()
{
}

//---------------------------------------------------------------------------
//! imgui�`��
//---------------------------------------------------------------------------
void SceneMain::imguiRender()
{
}
