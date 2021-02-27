#pragma once
#include "ScreenBase.h"
class TutorialScreen :
	public ScreenBase
{
public:
	TutorialScreen();
	~TutorialScreen();

	void Update();
	void Draw();
	void ReloadGraph();
};

