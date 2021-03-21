#pragma once
class SelectScene : public SceneBase
{
public:
	SelectScene();
	~SelectScene();

	void Update() override;
	void Draw() override;

private:
	
	int select_type;		// セレクトタイプ

	void ChangeSelect();	// セレクトを変える
	void Decision();		// 決定
	// 文字の描画
	void DrawStringType(int type_, int y_, string str_);

	// ランキング
	SceneBase *ranking; 

	enum SelectType {
		SELECT_LOAD = 0,
		SELECT_OPERATION,
		SELECT_RANKING,
		SELECT_TITLE,

		SELECT_MAX,
	};

	enum FontType {
		FONT_70 = 0,
		FONT_90,

		FONT_MAX,
	};
};

