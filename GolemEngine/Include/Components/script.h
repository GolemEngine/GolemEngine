#pragma once

#include "component.h"
#include "Refl/refl.hpp"

class Script : public Component
{
public:
	void Begin() override;

	void Update() override;

	virtual void ToJson(json& j) const {}

};


REFL_AUTO(
	type(Script, bases<Component>)
)