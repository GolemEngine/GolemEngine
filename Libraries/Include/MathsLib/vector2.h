#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

#include "dll.h"

using json = nlohmann::json;


class GOLEM_ENGINE_API Vector2
{
public:
	float x, y;

public:
	Vector2();
	~Vector2();
	Vector2(float _x, float _y);

	Vector2 Normalize() const;
	float Length() const;
	// only used to compare for optimization
	float LengthSq() const;			
	static float Dot(Vector2 _a, Vector2 _b);
	
	GOLEM_ENGINE_API friend Vector2 operator+(Vector2 _a, float _b);
	GOLEM_ENGINE_API friend Vector2 operator-(Vector2 _a, float _b);
	GOLEM_ENGINE_API friend Vector2 operator*(Vector2 _a, float _b);
	GOLEM_ENGINE_API friend Vector2 operator/(Vector2 _a, float _b);
	GOLEM_ENGINE_API friend Vector2 operator+(Vector2 _a, Vector2 _b);
	GOLEM_ENGINE_API friend Vector2 operator-(Vector2 _a, Vector2 _b);
	GOLEM_ENGINE_API friend bool operator==(Vector2 _a, Vector2 _b);

	GOLEM_ENGINE_API friend std::ostream& operator<<(std::ostream& _os, const Vector2& _vector2);


	// For JSON parsing
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Vector2,
		x, y)
};


