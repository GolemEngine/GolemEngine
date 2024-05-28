#pragma once

#include <nlohmann/json.hpp>

#include "Refl/refl.hpp"
#include "Reflection/attributes.h"

using json = nlohmann::json;


class GameObject;

class Component
{
public:
	GameObject* owner = nullptr;

public:
	virtual ~Component();

	virtual void Begin() {};

	virtual void Update() {};

	
	virtual void ToJson(json& _j) const = 0;    // Virtual function to serialize component to JSON
}; 

REFL_AUTO(
	type(Component)
)