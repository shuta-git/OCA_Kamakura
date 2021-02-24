//===========================================================================
//!	@file	GameScene.cpp
//!	@brief	�Q�[���V�[��
//===========================================================================
#include "GameScene.h"

//---------------------------------------------------------------------------
//! �R���X�g���N�^
//---------------------------------------------------------------------------
GameScene::GameScene()
{
    initialize();
}

//---------------------------------------------------------------------------
//! �f�X�g���N�^
//---------------------------------------------------------------------------
GameScene::~GameScene()
{
    finalize();
}

//---------------------------------------------------------------------------
//! ������
//!	@retval	true	����I��	(����)
//!	@retval	false	�G���[�I��	(���s)
//---------------------------------------------------------------------------
bool GameScene::initialize()
{
    setMode(SceneMode::GAME);

    //-------------------------------------------------------------
    // �V�F�[�_�[������
    //-------------------------------------------------------------
    shaderCs_ = gpu::ShaderCs::create("shader/csGaussianBlur5x5_H.fx");

    //-------------------------------------------------------------
    // �N���X�̏�����
    //-------------------------------------------------------------
    player_ = std::make_unique<Player>();
    map_ = std::make_unique<Map>();
    enemy_ = std::make_unique<EnemyManager>();

    textScale_ = 2.2f;
    text_      = u8"�p���v�L�������ނ���I";
    time_ = 50.0f;


    return true;
}

//---------------------------------------------------------------------------
//! �X�V
//---------------------------------------------------------------------------
void GameScene::update()
{
    time_ -= getDeltaTime();

    player_->update();
    enemy_->update(player_->getPos());
    map_->update();

    // �����蔻��
    collisionDetection();

    if(time_ < 0) {
        setMode(SceneMode::TITLE);
    }
}

//---------------------------------------------------------------------------
//! �`��
//---------------------------------------------------------------------------
void GameScene::render(RenderMode mode)
{
    if(mode == RenderMode::Scene) {
        player_->render();
        enemy_->render();
        map_->render();

        gpu::createText(0,0,textScale_, text_.c_str());
        {
            gpu::nextTextPos(900,0);
            s32 m = (s32)time_ / 60;             // ��
            s32 s = (s32)(fmod(time_, 60.0f));   //�@�b
            ImGuiWindowFlags window_flags = 0;
            window_flags                  = 0;
            window_flags |= ImGuiWindowFlags_NoTitleBar;
            window_flags |= ImGuiWindowFlags_NoBackground;
            window_flags |= ImGuiWindowFlags_NoResize;
            window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
            ImGui::Begin("time", (bool*)0, window_flags);
            {
                ImGui::GetWindowPos();
                ImGui::SetWindowFontScale(textScale_);
                ImGui::Text("%02d:%02d", m, s);
            }
            ImGui::End();
        }
    }
    

    //=============================================================
    // ComputeShader�̃e�X�g���s
    //=============================================================
    if(mode == RenderMode::Scene) {
        gpu::setRenderTarget(nullptr);   // �e�N�X�`���ŃA�N�Z�X���邽�߂ɏՓ˂�����邽�߂ɂ�������O��

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
void GameScene::finalize()
{

}

//---------------------------------------------------------------------------
//! imgui�`��
//---------------------------------------------------------------------------
void GameScene::imguiRender()
{
    player_->imguiRender();
    enemy_->imguiRender();
    map_->imguiRender();


    ImGui::Begin("GuideText");
    {
        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
        if(ImGui::CollapsingHeader("GuideText")) {
            //---------------------------------------------------------------------------
            //! �e�L�X�g�̕\��
            //---------------------------------------------------------------------------
            char text[256] = {};
            text_.copy(text, text_.size());
            ImGui::Text("Text");
            ImGui::InputTextMultiline("##text", text, sizeof(text));
            text_ = text;
            //---------------------------------------------------------------------------
            //! �X�P�[���̕\��
            //---------------------------------------------------------------------------
            ImGui::Text("textScale");
            ImGui::SameLine(80);
            ImGui::DragFloat("##textScale", &textScale_, 0.3f, -FLT_MAX, FLT_MAX);


            s32 m = (s32)time_ / 60;             // ��
            s32 s = (s32)(fmod(time_, 60.0f));   //�@�b
            ImGui::Text("time : %f", time_);
            ImGui::Text("m : %02d", m);
            ImGui::Text("s : %02d", s);

        }
    }
    ImGui::End();

}

//---------------------------------------------------------------------------
//! �����蔻��
//---------------------------------------------------------------------------
void GameScene::collisionDetection()
{

    // �v���C���[ �X�e�[�W
    for(int i = 0; i < map_->getColliderSize(); ++i) {
        if(isHit(player_->getBoxCollider(), map_->getBoxCollider(i))) {
            AABB    p   = player_->getBoxCollider()->getAABB();
            AABB    m   = map_->getBoxCollider(i)->getAABB();
            Vector3 pos = player_->getPos();
            // ���������������v�Z
            pos = calculation(p, m, pos);
            // �g���K�[ON
            player_->getBoxCollider()->setTrigger(true);
            // ���W�ݒ�
            player_->setPos(pos);
        }

    }

    // �v���C���[�@�G�l�~�[
    for(int i = 0; i < enemy_->getSize();++i) {

        // �G�l�~�[�Ƃ̓����蔻��
        if(isHit(player_->getBoxCollider(), enemy_->getBoxCollider(i))) {
            
            AABB p = player_->getBoxCollider()->getAABB();
            AABB e = enemy_->getBoxCollider(i)->getAABB();
            Vector3 pos = player_->getPos();
            
            // AABB�ǂ����̓��������������v�Z
            pos = calculation(p,e,pos);

            // �g���K�[ON
            player_->getBoxCollider()->setTrigger(true);
            // ���W�ݒ�
            player_->setPos(pos);
        }

        // �U������
        if (isHit(player_->getAttackCollider(), enemy_->getBoxCollider(i))) {
            // �g���K�[ON
            player_->getAttackCollider()->setTrigger(true);
            if (player_->getIsAttack()) 
            {
                enemy_->getBoxCollider(i)->setTrigger(true);
            }

        }
    }

    // �X�e�[�W�@�G�l�~�[
    for(int i = 0; i < map_->getColliderSize(); ++i) {
        for(int q = 0; q < enemy_->getSize(); ++q) {
            if(isHit(map_->getBoxCollider(i), enemy_->getBoxCollider(q))) {
                AABB    m   = map_->getBoxCollider(i)->getAABB();
                AABB    e   = enemy_->getBoxCollider(q)->getAABB();
                Vector3 pos = enemy_->getPos(q);

                // ���������������v�Z
                pos = calculation(e, m, pos);
                // ���W�ݒ�
                enemy_->setPos(q,pos);
            }
        }
    }


}
