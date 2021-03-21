#pragma once

class TitleScene : public SceneBase
{
public:
	TitleScene();
	~TitleScene();

	void Update() override;
	void Draw() override;

private:
	int flush;		
	
};

