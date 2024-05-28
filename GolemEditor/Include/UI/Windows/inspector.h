#pragma once

#include "UI/Windows/window.h"
#include "Core/gameobject.h"
#include "Components/transform.h"
#include "Refl/refl.hpp"
#include <iostream>
#include "imgui.h"
#include "imgui_stdlib.h"
#include "vector2.h"


class Inspector : public Window
{
public:
	Inspector(std::string _name);
	~Inspector();

	void Update() override;
};
	