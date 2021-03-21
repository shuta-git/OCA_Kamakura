//===========================================================================
//!	@file	Scene.cpp
//!	@brief  シーン基底
//===========================================================================
#include "Scene.h"

//---------------------------------------------------------------------------
//! シーンへジャンプリクエスト
//---------------------------------------------------------------------------
void Scene::sceneJump(Scene* s)
{
    prevMode_ = mode_;
    sceneNext_.reset(s);   // 次のシーンのリクエストを設定
}

//---------------------------------------------------------------------------
//! シーンシステム更新
//---------------------------------------------------------------------------
void Scene::sceneUpdate()
{
    // シーン切り替えリクエストがあったら現在のシーンを解放し
    // 新しく初期化
    if(sceneNext_) {
        if(scene_)
        scene_->finalize();   // 解放
        scene_ = std::move(sceneNext_);
        scene_->initialize();   // 初期化
    }

    if(scene_) {
        // 更新
        scene_->update();
    }

    // F5KeyでImguiを表示切り替え
    if(GetAsyncKeyState(VK_F5) & 1) {
        showImguiWindow     = showImguiWindow ? false : true;
    }
    // F6KeyでImguiDemoWindowを表示切り替え
    if(GetAsyncKeyState(VK_F6) & 1) {
        showImguiDemoWindow = showImguiDemoWindow ? false : true;
    }
}

//---------------------------------------------------------------------------
//! シーン描画
//---------------------------------------------------------------------------
void Scene::sceneRender()
{
    

    //-------------------------------------------------------------
    // シーンの切り替えと実行
    //-------------------------------------------------------------
    GmRender()->begin();

    
    
    //==========================================================
    // シャドウ描画
    //==========================================================
    auto shadowmap = GmRender()->getShadowmap();
    
    shadowmap->begin();
    
    if(scene_) {
        scene_->render(RenderMode::Shadow);
    }

    shadowmap->end();
    
   

    //=============================================================
    //	シーン描画 (GBuffer生成)
    //=============================================================
    GmRender()->beginScene();
    
    

    if(scene_) {
        scene_->render(RenderMode::Scene);
    }

    GmRender()->endScene();
    
    GmRender()->end();

    //=============================================================
    //	2D描画
    //=============================================================
    if(scene_) {
        scene_->render(RenderMode::UI);
    }

}

//---------------------------------------------------------------------------
//! ImGuiWindow表示
//---------------------------------------------------------------------------
void Scene::showImGuiWindow()
{
    if(scene_) {
        if(!showImguiWindow && !showImguiDemoWindow) {
            // カーソルOFF
            ShowCursor(FALSE);
        }
        else {
            // カーソルON
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
