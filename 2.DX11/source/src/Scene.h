//===========================================================================
//! @file Scene.h
//!	@brief	シーン基底
//===========================================================================
#pragma once

enum class RenderMode
{
    Scene,    //!< シーン描画
    Shadow,   //!< シャドウ生成
    UI,       //!< UI描画
};

enum SceneMode
{
    TITLE,
    GAME,
    LOAD,
    //-----
    MAX
};

namespace {
    bool showImguiWindow     = true;   //!< Imguiを表示するかどうか
    bool showImguiDemoWindow = false;   //!< ImguiDemoWindowを表示するかどうか
} 

//===========================================================================
//!	シーン基底
//===========================================================================
class Scene
{
public:
    Scene()          = default;
    virtual ~Scene() = default;

    //! 初期化
    virtual bool initialize() = 0;

    //! 更新
    virtual void update() = 0;

    //! 描画
    virtual void render(RenderMode mode) = 0;

    //! 解放
    virtual void finalize() = 0;

    //! imgui描画
    virtual void imguiRender() = 0;

public:
    //-------------------------------------------------------------
    //!	@name	シーン管理
    //-------------------------------------------------------------
    //@{

    //! シーンへジャンプリクエスト
    static void sceneJump(Scene* s);

    //! シーンシステム更新
    static void sceneUpdate();

    //! シーン描画
    static void sceneRender();

    //! ImGui表示
    static void showImGuiWindow();

    //! 現在のシーンを取得
    static Scene* getScene() { return scene_.get(); }

    //@}

    static int getMode()  { return mode_; }

    static int getPrevMode() { return prevMode_; }

protected:

    void setMode(int mode) { mode_ = mode; }


private:
    static inline std::unique_ptr<Scene> scene_     = nullptr;   //!< 現在のシーン
    static inline std::unique_ptr<Scene> sceneNext_ = nullptr;   //!< 次のシーンリクエスト

    static inline int mode_ = 0;   //!< シーンモード
    static inline int prevMode_ = 0;   //!< 1フレーム前のシーンモード

protected:

};

//--------------------------------------------------------------
// アクセスインターフェイス
//--------------------------------------------------------------
inline Scene* GmScene()
{
    return Scene::getScene();
}
