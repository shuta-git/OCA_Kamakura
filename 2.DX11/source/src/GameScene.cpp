//===========================================================================
//!	@file	GameScene.cpp
//!	@brief	ゲームシーン
//===========================================================================
#include "GameScene.h"

//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
GameScene::GameScene()
{
    initialize();
}

//---------------------------------------------------------------------------
//! デストラクタ
//---------------------------------------------------------------------------
GameScene::~GameScene()
{
    finalize();
}

//---------------------------------------------------------------------------
//! 初期化
//!	@retval	true	正常終了	(成功)
//!	@retval	false	エラー終了	(失敗)
//---------------------------------------------------------------------------
bool GameScene::initialize()
{
    setMode(SceneMode::GAME);

    //-------------------------------------------------------------
    // シェーダー初期化
    //-------------------------------------------------------------
    shaderCs_ = gpu::ShaderCs::create("shader/csGaussianBlur5x5_H.fx");

    //-------------------------------------------------------------
    // クラスの初期化
    //-------------------------------------------------------------
    player_ = std::make_unique<Player>();
    map_ = std::make_unique<Map>();
    enemy_ = std::make_unique<EnemyManager>();

    textScale_ = 2.2f;
    text_      = u8"パンプキンを撃退しろ！";
    time_ = 50.0f;


    return true;
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void GameScene::update()
{
    time_ -= getDeltaTime();

    player_->update();
    enemy_->update(player_->getPos());
    map_->update();

    // 当たり判定
    collisionDetection();

    if(time_ < 0) {
        setMode(SceneMode::TITLE);
    }
}

//---------------------------------------------------------------------------
//! 描画
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
            s32 m = (s32)time_ / 60;             // 分
            s32 s = (s32)(fmod(time_, 60.0f));   //　秒
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
    // ComputeShaderのテスト実行
    //=============================================================
    if(mode == RenderMode::Scene) {
        gpu::setRenderTarget(nullptr);   // テクスチャでアクセスするために衝突を避けるためにいったん外す

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
void GameScene::finalize()
{

}

//---------------------------------------------------------------------------
//! imgui描画
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
            //! テキストの表示
            //---------------------------------------------------------------------------
            char text[256] = {};
            text_.copy(text, text_.size());
            ImGui::Text("Text");
            ImGui::InputTextMultiline("##text", text, sizeof(text));
            text_ = text;
            //---------------------------------------------------------------------------
            //! スケールの表示
            //---------------------------------------------------------------------------
            ImGui::Text("textScale");
            ImGui::SameLine(80);
            ImGui::DragFloat("##textScale", &textScale_, 0.3f, -FLT_MAX, FLT_MAX);


            s32 m = (s32)time_ / 60;             // 分
            s32 s = (s32)(fmod(time_, 60.0f));   //　秒
            ImGui::Text("time : %f", time_);
            ImGui::Text("m : %02d", m);
            ImGui::Text("s : %02d", s);

        }
    }
    ImGui::End();

}

//---------------------------------------------------------------------------
//! 当たり判定
//---------------------------------------------------------------------------
void GameScene::collisionDetection()
{

    // プレイヤー ステージ
    for(int i = 0; i < map_->getColliderSize(); ++i) {
        if(isHit(player_->getBoxCollider(), map_->getBoxCollider(i))) {
            AABB    p   = player_->getBoxCollider()->getAABB();
            AABB    m   = map_->getBoxCollider(i)->getAABB();
            Vector3 pos = player_->getPos();
            // 当たった長さを計算
            pos = calculation(p, m, pos);
            // トリガーON
            player_->getBoxCollider()->setTrigger(true);
            // 座標設定
            player_->setPos(pos);
        }

    }

    // プレイヤー　エネミー
    for(int i = 0; i < enemy_->getSize();++i) {

        // エネミーとの当たり判定
        if(isHit(player_->getBoxCollider(), enemy_->getBoxCollider(i))) {
            
            AABB p = player_->getBoxCollider()->getAABB();
            AABB e = enemy_->getBoxCollider(i)->getAABB();
            Vector3 pos = player_->getPos();
            
            // AABBどうしの当たった長さを計算
            pos = calculation(p,e,pos);

            // トリガーON
            player_->getBoxCollider()->setTrigger(true);
            // 座標設定
            player_->setPos(pos);
        }

        // 攻撃判定
        if (isHit(player_->getAttackCollider(), enemy_->getBoxCollider(i))) {
            // トリガーON
            player_->getAttackCollider()->setTrigger(true);
            if (player_->getIsAttack()) 
            {
                enemy_->getBoxCollider(i)->setTrigger(true);
            }

        }
    }

    // ステージ　エネミー
    for(int i = 0; i < map_->getColliderSize(); ++i) {
        for(int q = 0; q < enemy_->getSize(); ++q) {
            if(isHit(map_->getBoxCollider(i), enemy_->getBoxCollider(q))) {
                AABB    m   = map_->getBoxCollider(i)->getAABB();
                AABB    e   = enemy_->getBoxCollider(q)->getAABB();
                Vector3 pos = enemy_->getPos(q);

                // 当たった長さを計算
                pos = calculation(e, m, pos);
                // 座標設定
                enemy_->setPos(q,pos);
            }
        }
    }


}
