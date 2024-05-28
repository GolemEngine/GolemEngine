#pragma once

#include "UI/Windows/window.h"


class BasicActorsWindow : public Window
{
public:
	BasicActorsWindow(std::string _name);
	~BasicActorsWindow();

	void Update() override;
};