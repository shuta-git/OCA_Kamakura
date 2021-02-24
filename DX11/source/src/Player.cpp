//===========================================================================
//!	@file	Player.cpp
//!	@brief	プレイヤー
//===========================================================================
#include "Player.h"

//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
Player::Player()
{
    initialize();
}

//---------------------------------------------------------------------------
//! デストラクタ
//---------------------------------------------------------------------------
Player::~Player()
{
    finalize();
}

//---------------------------------------------------------------------------
//! 初期化
//!	@retval	true	正常終了	(成功)
//!	@retval	false	エラー終了	(失敗)
//---------------------------------------------------------------------------
bool Player::initialize()
{
    //-------------------------------------------------------------
    // モデル初期化
    //-------------------------------------------------------------
    modelFbx_ = gpu::loadFBX("data/Hero/Swordsman/swordsman_Idle.fbx", 1.0f);
    if(modelFbx_ == nullptr) {
        MessageBox(nullptr, "fbxモデルの読み込みに失敗しました.", nullptr, MB_OK);
    }

    pos_ = { 0.0f, 0.1f, -9.0f };

    modelFbx_->setRotateX(radian(180.0f));
    modelFbx_->setRotateY(radian(180.0f));
    modelFbx_->setPos(pos_);

    moveSpeed_ = 1.5f;
    isAttack_ = false;

    //-------------------------------------------------------------
    // Debugカメラ初期化
    //-------------------------------------------------------------
    cameraDebug_ = std::make_unique<CameraDebug>();
    cameraDebug_->setPosition(Vector3(0.0f, 1.2f,1.5f));
    cameraDebug_->setLookAt(Vector3(0.0f,0.5f, 0.0f));
    cameraDebug_->setNearZ(0.01f);
    cameraDebug_->setFarZ(1000.0f);
    
    //-------------------------------------------------------------
    // プレイヤーカメラ初期化
    //-------------------------------------------------------------
    cameraPlayer_ = std::make_unique<CameraPlayer>();
    cameraPlayer_->setNearZ(0.01f);
    cameraPlayer_->setFarZ(1000.0f);

    //-------------------------------------------------------------
    //! コライダー
    //-------------------------------------------------------------
    boxCollider_ = gpu::createBoxCollider();
    boxCollider_->setDepth(0.5f);
    boxCollider_->setWigth(0.5f);
    // 攻撃用
    attackCollider_ = gpu::createBoxCollider();
    attackCollider_->setDepth(0.5f);
    attackCollider_->setWigth(0.5f);

    return true;
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void Player::update()
{

    rotate();   // 回転
    move();     // 移動
    attack();   // 攻撃


    //-------------------------------------------------------------
    // Debugカメラの更新
    //-------------------------------------------------------------
    cameraDebug_->update();

    //-------------------------------------------------------------
    // プレイヤーカメラの更新
    //-------------------------------------------------------------
    cameraPlayer_->setPlayerPos(Vector3(pos_.x_, pos_.y_ + 1.5f, pos_.z_));
    cameraPlayer_->setLookAt(Vector3(pos_.x_, pos_.y_ + 1.0f, pos_.z_));
    cameraPlayer_->update();

    //-------------------------------------------------------------
    // コライダー更新
    //-------------------------------------------------------------
    Vector3 boxPos = pos_;
    boxPos.x_ -= boxCollider_->getWigth() / 2;
    boxPos.z_ -= boxCollider_->getDepth() / 2;
    boxCollider_->setPos(boxPos);
    boxCollider_->setTrigger(false);

    Vector3 attackPos = pos_;
    attackPos.x_ -= attackCollider_->getWigth() / 2;
    attackPos.z_ -= attackCollider_->getDepth() / 2;
    attackPos.x_ -= sinf(rotate_.y_) * 0.5f;
    attackPos.z_ -= cosf(rotate_.y_) * 0.5f;
    attackCollider_->setPos(attackPos);
    attackCollider_->setTrigger(false);

}

//---------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------
void Player::render()
{
    //-------------------------------------------------------------
    // シーン用のカメラ行列を設定
    //-------------------------------------------------------------
    //cameraPlayer_
    //cameraDebug_
    GmRender()->setCamera(cameraPlayer_.get());

    // モデル描画
    modelFbx_->render();

    // コライダー描画
    attackCollider_->render();
    if(colliderRender) {
        boxCollider_->render();
    }

}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void Player::finalize()
{
    cameraPlayer_.release();
}

//---------------------------------------------------------------------------
//! imgui描画
//---------------------------------------------------------------------------
void Player::imguiRender()
{
    //-------------------------------------------------------------
    // プレイヤーのデバッグウィンドウ
    //-------------------------------------------------------------
    ImGui::Begin(u8"プレイヤー");
    
    if(ImGui::BeginTabBar(u8"プレイヤー")) {
        
        if(ImGui::BeginTabItem(u8"データ")) {
            POINT cursor;   // 現在位置
            GetCursorPos(&cursor);
            ImGui::Text("cursor.x : %d", cursor.x);
            ImGui::Text("cursor.y : %d", cursor.y);

            double fps = getFPS();

            ImGui::Text("FPS : %lf", fps);
            ImGui::Checkbox(u8"コライダー", &colliderRender);

            ImGui::EndTabItem();

        }

        // ボックスコライダーのImGui描画
        boxCollider_->imguiRender();
        attackCollider_->imguiRender();
         
        modelFbx_->imguiRender();

        ImGui::EndTabBar();

    }
    
    
    ImGui::End();

    //-------------------------------------------------------------
    // モデル情報表示
    //-------------------------------------------------------------

}

