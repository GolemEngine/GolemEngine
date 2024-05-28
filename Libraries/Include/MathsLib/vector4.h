#pragma once

#include <nlohmann/json.hpp>

#include "dll.h"

using json = nlohmann::json;

class Matrix4;


class GOLEM_ENGINE_API Vector4
{
public:
	float x, y, z, w;

public:
	Vector4();
	~Vector4();
	Vector4(float _x, float _y, float _z, float _w);
	
	float Length() const;
	// Used for comparing purpose only because doesn't sqrt to make less operations.
	float LengthSq() const;
	Vector4 Normalize() const;
	
	GOLEM_ENGINE_API friend Vector4 operator+(Vector4 _a, float _b);
	GOLEM_ENGINE_API friend Vector4 operator-(Vector4 _a, float _b);
	GOLEM_ENGINE_API friend Vector4 operator*(Vector4 _a, float _b);
	GOLEM_ENGINE_API friend Vector4 operator/(Vector4 _a, float _b);
	GOLEM_ENGINE_API friend Vector4 operator+(Vector4 _a, Vector4 _b);
	GOLEM_ENGINE_API friend Vector4 operator-(Vector4 _a, Vector4 _b);
	GOLEM_ENGINE_API friend bool operator==(Vector4 _a, Vector4 _b);
	GOLEM_ENGINE_API friend Vector4 operator*(Matrix4 _a, Vector4 _b);

	GOLEM_ENGINE_API friend std::ostream& operator<<(std::ostream& _os, const Vector4& _Vector4);


	// For JSON parsing
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Vector4,
		x, y, z, w)
};


