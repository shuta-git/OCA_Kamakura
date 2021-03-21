#pragma once

//---------------------------------------------
// SceneManagerクラス
//---------------------------------------------
class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void Update();
	void Draw();

private:

	int scene_type;		// シーンの種類
	int old_scene;		// 一つ前のシーン

	SceneBase* scenes;
	SceneBase* operation;
	SceneBase* pause;

	// scenesのデリート処理
	void ScenesDelete();
	// operationのデリート処理
	void OperationDelete();
	// pauseのデリート処理
	void PauseDelete();
	// シーン切り替え
	void ChangeScene();
	// シーンを生成
	void CreatScene();
	// シーンが変わってるかチェック
	void CheckChangeScene(SceneBase *b, int i);
};

