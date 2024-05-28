#pragma once

#include <iostream>

#include "dll.h"
#include "vector3.h"
#include "quaternion.h"


class GOLEM_ENGINE_API Matrix4
{
public:
	float data[4][4];

public:
	Matrix4();
	~Matrix4();
	Matrix4(
		float _a, float _b, float _c, float _d,
		float _e, float _f, float _g, float _h,
		float _i, float _j, float _k, float _l,
		float _m, float _n, float _o, float _p
	);
	explicit Matrix4(float _number);

	static Matrix4 Identity();
	Matrix4 Transpose() const;
	float GetDeterminant() const;
	Matrix4 Inverse() const;
	Matrix4 Translate(Vector3 _t) const;
	Matrix4 Rotate(Vector3 _XYZrad) const;
	Matrix4 Rotate(Quaternion _q) const;
	Matrix4 Scale(Vector3 _scale) const;
	Matrix4 ExtractRotationAndScale(const Matrix4& _viewMatrix) const;
	
	Vector3 TrsToPosition();
	Quaternion TrsToRotation();
	Vector3 TrsToScaling();
	static Matrix4 TRS(Vector3 _translate, Vector3 _rotate, Vector3 _scale);
	static Matrix4 TRS(Vector3 _translate, Quaternion _rotate, Vector3 _scale);
	static Matrix4 Projection(float _fov, float _aspectRatio, float _zNear, float _zFar);
	static Matrix4 LookAt(Vector3 _cameraPos, Vector3 _targetPos, Vector3 _up);
	
	GOLEM_ENGINE_API friend Matrix4 operator+(Matrix4 _A, float _n);
	GOLEM_ENGINE_API friend Matrix4 operator-(Matrix4 _A, float _n);
	GOLEM_ENGINE_API friend Matrix4 operator*(Matrix4 _A, float _s);
	GOLEM_ENGINE_API friend Matrix4 operator/(Matrix4 _A, float _s);
	GOLEM_ENGINE_API friend Matrix4 operator+(Matrix4 _A, Matrix4 _B);
	GOLEM_ENGINE_API friend Matrix4 operator-(Matrix4 _A, Matrix4 _B);
	GOLEM_ENGINE_API friend Matrix4 operator*(Matrix4 _A, Matrix4 _B);
	GOLEM_ENGINE_API friend bool operator==(Matrix4 _A, Matrix4 _B);
	
	GOLEM_ENGINE_API friend std::ostream& operator<<(std::ostream& _os, const Matrix4& _matrix4);
};


