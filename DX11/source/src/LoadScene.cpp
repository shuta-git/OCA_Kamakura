
//===========================================================================
//!	@file	LoadScene.cpp
//!	@brief	ロードシーン
//===========================================================================
#include "LoadScene.h"

//---------------------------------------------
// 定数の宣言
//---------------------------------------------
const f32 MAX_LOAD_TIME = 1.0f;   //!< 最大ロード時間

//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
LoadScene::LoadScene()
{
    initialize();
}

//---------------------------------------------------------------------------
//! デストラクタ
//---------------------------------------------------------------------------
LoadScene::~LoadScene()
{
    finalize();
}

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool LoadScene::initialize()
{
    setMode(SceneMode::LOAD);

    texture_.reset(gpu::createTexture("data/load/road3.png"));

    time_ = MAX_LOAD_TIME;

    return true;
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void LoadScene::update()
{
    time_ -= getDeltaTime();

    if(time_ < 0) {
        setMode(SceneMode::GAME);
    }
}

//---------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------
void LoadScene::render(RenderMode mode)
{

    if(mode == RenderMode::UI) {
        gpu::setTexture(5, texture_);
    }

}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void LoadScene::finalize()
{
}

//---------------------------------------------------------------------------
//! ImGui描画
//---------------------------------------------------------------------------
void LoadScene::imguiRender()
{
}

//---------------------------------------------------------------------------
//! ファイル読み込み
//---------------------------------------------------------------------------
void LoadScene::loadFile()
{
}
