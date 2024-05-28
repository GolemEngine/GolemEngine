#pragma once

#include "Refl/refl.hpp"

struct HideInInspector : refl::attr::usage::field, refl::attr::usage::function
{
};

struct Range : refl::attr::usage::field
{
	float min;
	float max;
	
	constexpr Range(float _min, float _max)
	{ 
		min = _min;
		max = _max;
	}
};
