#pragma once

class PauseScene : public SceneBase
{
public:
	PauseScene();
	~PauseScene();

	void Update() override;
	void Draw() override;

private:

	int select_type;		// セレクトタイプ
	void ChangeSelect();	// セレクトを変える
	void Decision();		// 決定
	// 文字の描画
	void DrawStringType(int type_, int y_, string str_);

	enum PauseType {
		PAUSE_GAME = 0,
		PAUSE_LOAD,
		PAUSE_OPERATION,
		PAUSE_SELECT,
		PAUSE_TITLE,

		PAUSET_MAX,
	};

	enum FontType {
		FONT_50 = 0,
		FONT_60,

		FONT_MAX,
	};

};

