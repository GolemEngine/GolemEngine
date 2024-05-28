#pragma once

#include "UI/Windows/window.h"


class DebugWindow : public Window
{
public:
	DebugWindow(std::string _name);
	~DebugWindow();

	void Update() override;
	void DrawFpsGraph();
};