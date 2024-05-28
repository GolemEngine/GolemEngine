#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

#include "dll.h"

using json = nlohmann::json;

class Quaternion;


class GOLEM_ENGINE_API Vector3
{
public:
	float x, y, z;

public:
	Vector3();
	Vector3(float _xyz);
	Vector3(float _x, float _y, float _z);
	~Vector3();

	float Length() const;
	// Used for comparing purpose only because doesn't sqrt to make less operations.
	float LengthSq() const;
	Vector3 Normalize() const;
	static float Dot(Vector3 _a, Vector3 _b);
	static Vector3 Cross(Vector3 _a, Vector3 _b);

	/*
	- From Quaternion angles to Euler : assuming Quaternion is normalized
	- Returns a Vector3 of angles
	*/
	static Vector3 QuaternionToEuler(Quaternion _q);

	GOLEM_ENGINE_API friend Vector3 operator+(Vector3 _a, float b);
	GOLEM_ENGINE_API friend Vector3 operator-(Vector3 _a, float b);
	GOLEM_ENGINE_API friend Vector3 operator*(Vector3 _a, float b);
	GOLEM_ENGINE_API friend Vector3 operator/(Vector3 _a, float b);
	GOLEM_ENGINE_API friend Vector3 operator+(Vector3 _a, Vector3 _b);
	GOLEM_ENGINE_API friend Vector3 operator-(Vector3 _a, Vector3 _b);
	GOLEM_ENGINE_API friend Vector3 operator+=(Vector3& _a, Vector3 _b);
	GOLEM_ENGINE_API friend Vector3 operator-=(Vector3& _a, Vector3 _b);
	GOLEM_ENGINE_API friend bool operator==(Vector3 _a, Vector3 _b);
	
	GOLEM_ENGINE_API friend std::ostream& operator<<(std::ostream& os, const Vector3& _vector3);

	
	// For JSON parsing
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Vector3,
		x, y, z)
};


