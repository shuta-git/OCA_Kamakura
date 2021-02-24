#pragma once
#include "ScreenBase.h"

class OperationScreen : public ScreenBase
{
public:
	OperationScreen();
	~OperationScreen();

	void Update();
	void Draw();
	void ReloadGraph();
};

