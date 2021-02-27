//===========================================================================
//!	@file	Scene.cpp
//!	@brief  �V�[�����
//===========================================================================
#include "Scene.h"

//---------------------------------------------------------------------------
//! �V�[���փW�����v���N�G�X�g
//---------------------------------------------------------------------------
void Scene::sceneJump(Scene* s)
{
    prevMode_ = mode_;
    sceneNext_.reset(s);   // ���̃V�[���̃��N�G�X�g��ݒ�
}

//---------------------------------------------------------------------------
//! �V�[���V�X�e���X�V
//---------------------------------------------------------------------------
void Scene::sceneUpdate()
{
    // �V�[���؂�ւ����N�G�X�g���������猻�݂̃V�[���������
    // �V����������
    if(sceneNext_) {
        if(scene_)
        scene_->finalize();   // ���
        scene_ = std::move(sceneNext_);
        scene_->initialize();   // ������
    }

    if(scene_) {
        // �X�V
        scene_->update();
    }

    // F5Key��Imgui��\���؂�ւ�
    if(GetAsyncKeyState(VK_F5) & 1) {
        showImguiWindow     = showImguiWindow ? false : true;
    }
    // F6Key��ImguiDemoWindow��\���؂�ւ�
    if(GetAsyncKeyState(VK_F6) & 1) {
        showImguiDemoWindow = showImguiDemoWindow ? false : true;
    }
}

//---------------------------------------------------------------------------
//! �V�[���`��
//---------------------------------------------------------------------------
void Scene::sceneRender()
{
    

    //-------------------------------------------------------------
    // �V�[���̐؂�ւ��Ǝ��s
    //-------------------------------------------------------------
    GmRender()->begin();

    
    
    //==========================================================
    // �V���h�E�`��
    //==========================================================
    auto shadowmap = GmRender()->getShadowmap();
    
    shadowmap->begin();
    
    if(scene_) {
        scene_->render(RenderMode::Shadow);
    }

    shadowmap->end();
    
   

    //=============================================================
    //	�V�[���`�� (GBuffer����)
    //=============================================================
    GmRender()->beginScene();
    
    

    if(scene_) {
        scene_->render(RenderMode::Scene);
    }

    GmRender()->endScene();
    
    GmRender()->end();

    //=============================================================
    //	2D�`��
    //=============================================================
    if(scene_) {
        scene_->render(RenderMode::UI);
    }

}

//---------------------------------------------------------------------------
//! ImGuiWindow�\��
//---------------------------------------------------------------------------
void Scene::showImGuiWindow()
{
    if(scene_) {
        if(!showImguiWindow && !showImguiDemoWindow) {
            // �J�[�\��OFF
            ShowCursor(FALSE);
        }
        else {
            // �J�[�\��ON
            ShowCursor(TRUE);
        }

        // Imgui
        if(showImguiWindow) {
            GmRender()->imguiRender();
            scene_->imguiRender();
        }

        // Demo
        if(showImguiDemoWindow) {
            ImGui::ShowDemoWindow(&showImguiDemoWindow);
        }
    }
}
