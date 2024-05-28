#pragma once

#include <iostream>

#include "dll.h"

class Vector3;

class GOLEM_ENGINE_API Quaternion
{
public:
	float x, y, z, w;

public:
	Quaternion();
	Quaternion(float _x, float _y, float _z, float _w);
	Quaternion(float _w, Vector3 _xyz);
	~Quaternion();
	
	float Norm();
	Quaternion Normalized();
	Quaternion UnitNorm();
	Quaternion Conjugate();
	Quaternion Inverse();
	Quaternion RotateQuaternionAroundAxis(float _angle, Vector3 _axis);

	// Angles are in degrees
	static Quaternion EulerToQuaternion(Vector3 _xyzAngles);

	GOLEM_ENGINE_API friend Quaternion operator*(Quaternion _q1, Quaternion _q2);
	GOLEM_ENGINE_API friend Quaternion operator/(Quaternion _q1, float _divider);
	GOLEM_ENGINE_API friend bool operator==(Quaternion _q1, Quaternion _q2);
	GOLEM_ENGINE_API friend bool operator!=(Quaternion _q1, Quaternion _q2);

};

