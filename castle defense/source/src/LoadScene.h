#pragma once

class LoadScene : public SceneBase
{
public:
	LoadScene();
	~LoadScene();

	void Update() override;
	void Draw() override;

private:

	//=======================================
	// 変数
	//=======================================
	int handle_count;	// ハンドルカウント
	int load_count;		// ロードカウント
	int gauge_count;	// ゲージ用のカウント
	int load_type;		// ロードタイプ
	int load_time;		// ロード時間
	int load_str_num;	// ロード文字の番号


	// ファイル読み込み用
	fstream file;
	//　モデルパスを保存用
	std::vector<string>model_path;
	// 画像パスを保存用
	std::vector<string>graph_path;
	// エフェクトパス保存用
	std::vector<string>effect_path;
	// ロードの文字用の
	std::vector<string>load_str;

	// 読み込み画面描画関数
	void DrawLoadingScreen();
	// ロードモデルハンドル
	void LoadModelHandle(string str);
	// ロードグラフィックハンドル
	void LoadGraphHandle(string str);
	// ロードエフェクトハンドル
	void LoadEffectHandle(string str);
	// モデルパスファイルを読み込み
	void ReadModelFilePath();
	// 画像パスファイルを読み込み
	void ReadGraphFilePath();
	// エフェクトパスファイルを読み込み
	void ReadEffectFilePath();
	// ロードタイムの更新処理
	// load_str_num がサイズ分超えたときに何番に調整するか
	void LoadUpdate(int num);

	enum LOAD_TYPE {
		MODEL = 0,
		GRAPH,
		EFFECT,

		FINISH,
	};
};

