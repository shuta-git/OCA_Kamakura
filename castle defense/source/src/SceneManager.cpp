#include "SceneManager.h"

//---------------------------------------------
// コンストラクタ
//---------------------------------------------
SceneManager::SceneManager()
{
	scenes = new TitleScene();
	operation = nullptr;
}
//---------------------------------------------
// デストラクタ
//---------------------------------------------
SceneManager::~SceneManager()
{
}
//---------------------------------------------
// 更新処理
//---------------------------------------------
void SceneManager::Update()
{
	// シーンを切り替え
	ChangeScene();
	// 切り替えてすぐにシーンを生成
	CreatScene();

	if (scenes != nullptr) {
		// シーンがポーズか操作説明じゃなかったら
		if (scene_type != PAUSE &&
			scene_type != OPERATION)
		{
			// 更新処理
			scenes->Update();
		}
	}
	// 更新処理
	if (operation != nullptr) {
		operation->Update();
	}
	if (pause != nullptr) {
		pause->Update();
	}

	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer2D();
}
//---------------------------------------------
// 描画処理
//---------------------------------------------
void SceneManager::Draw()
{
	if (scenes != nullptr) {
		scenes->Draw();
	}
	if (operation != nullptr) {
		operation->Draw();
	}
	if (pause != nullptr) {
		pause->Draw();
	}
}
//---------------------------------------------
// シーンの切り替え
//---------------------------------------------
void SceneManager::ChangeScene()
{
	switch (scene_type)
	{
		// タイトル
	case TITLE:
		CheckChangeScene(scenes, TITLE);
		break;
		// セレクト
	case SELECT:
		CheckChangeScene(scenes, SELECT);
		break;
		// ロード
	case LOAD:
		CheckChangeScene(scenes, LOAD);
		break;
		// ゲームメイン
	case GAME_MAIN:
		if (scenes->GetSceneType() == PAUSE) {
			scene_type = scenes->GetSceneType();
			break;
		}
		CheckChangeScene(scenes, GAME_MAIN);
		break;
		// 操作説明
	case OPERATION:
		CheckChangeScene(operation, OPERATION);
		break;
		// ポーズ
	case PAUSE:
		CheckChangeScene(pause, PAUSE);
		break;
		// リザルト
	case RESULT:
		CheckChangeScene(scenes, RESULT);
		break;
	}
}
//---------------------------------------------
// シーンを生成
//---------------------------------------------
void SceneManager::CreatScene()
{
	switch (scene_type)
	{
		// タイトル
	case TITLE:
		if (scenes == nullptr) {
			scenes = new TitleScene();
		}
		break;
		// セレクト
	case SELECT:
		if (scenes == nullptr) {
			scenes = new SelectScene();
		}
		break;
		// ロード
	case LOAD:
		if (scenes == nullptr) {
			scenes = new LoadScene();
		}
		break;
		// ゲームメイン
	case GAME_MAIN:
		if (scenes == nullptr) {
			scenes = new GameMain();
		}
		break;
		// 操作説明
	case OPERATION:
		if (operation == nullptr) {
			operation = new OperationScene(old_scene);
		}
		break;
		// ポーズ
	case PAUSE:
		if (pause == nullptr) {
			pause = new PauseScene();
		}
		break;
		// リザルト
	case RESULT:
		if (scenes == nullptr) {
			scenes = new ResultScene();
		}
		break;
	}
}
//---------------------------------------------
// シーンが変わってるかチェック
//---------------------------------------------
void SceneManager::CheckChangeScene(SceneBase *b, int i)
{
	// シーンが同じだったらこの関数を抜ける
	if (b->GetSceneType() == i) { return; }

	// シーンがロードじゃないとき
	if (scene_type != LOAD && scene_type != PAUSE && scene_type != OPERATION) {
		// resourceに保存しているデータをすべて消去
		resource::DeleteHandleTable();
	}

	// シーンを保存
	old_scene = scene_type;
	// 次のシーンをシーンタイプに入れる
	scene_type = b->GetSceneType();

	// 格シーン
	if (b == scenes) {
		ScenesDelete();
	}
	// 操作
	if (b == operation) {
		OperationDelete();
	}
	// ポーズ
	if (b == pause) {
		// シーンがメインだったら
		if (pause->GetSceneType() == GAME_MAIN) {
			// scenesにシーンタイプを設定
			scenes->SetSceneType(GAME_MAIN);
			// ポーズの消去
			PauseDelete();
			return;
		}
		// ポーズ中のシーンが操作画面になったら
		if (pause->GetSceneType() == OPERATION) {
			// ポーズ画面の消去
			PauseDelete();
			return;
		}
		PauseDelete();
		ScenesDelete();
	}
}
//---------------------------------------------
// scenesのデリート処理
//---------------------------------------------
void SceneManager::ScenesDelete()
{
	if (scenes == nullptr) { return; }

	scenes->DeleteFontHandle();

	delete scenes;
	scenes = nullptr;
}
//---------------------------------------------
// operationのデリート処理
//---------------------------------------------
void SceneManager::OperationDelete()
{
	if (operation == nullptr) { return; }

	operation->DeleteFontHandle();

	delete operation;
	operation = nullptr;
}
//---------------------------------------------
// pauseのデリート処理
//---------------------------------------------
void SceneManager::PauseDelete()
{
	if (pause == nullptr) { return; }

	pause->DeleteFontHandle();

	delete pause;
	pause = nullptr;
}
