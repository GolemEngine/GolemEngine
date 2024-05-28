#pragma once

#include "Components/script.h"
#include "Core/camera.h"
#include "Refl/refl.hpp"

class PlayerCamera : public Script
{
public:
	PlayerCamera();
	~PlayerCamera();

	void Begin() override;
	void Update() override;

	void ToJson(json& j) const {}
};

REFL_AUTO(
	type(PlayerCamera, bases<Script>)
)