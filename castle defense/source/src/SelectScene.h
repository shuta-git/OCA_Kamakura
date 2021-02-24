#pragma once
class SelectScene : public SceneBase
{
public:
	SelectScene();
	~SelectScene();

	void Update() override;
	void Draw() override;

private:
	
	int select_type;		// �Z���N�g�^�C�v

	void ChangeSelect();	// �Z���N�g��ς���
	void Decision();		// ����
	// �����̕`��
	void DrawStringType(int type_, int y_, string str_);

	// �����L���O
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

