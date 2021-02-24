#pragma once
class OperationScene : public SceneBase
{
public:
	OperationScene(int old_scene_);
	~OperationScene();

	void Update() override;
	void Draw() override;
};

