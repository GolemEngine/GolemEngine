#include <iomanip>

#include "quaternion.h"
#include "vector3.h"
#include "utils.h"

Quaternion::Quaternion()
	: w(0.0f), x(0.0f), y(0.0f), z(0.0f)
{ 
}

Quaternion::Quaternion(float _w, float _x, float _y, float _z)
	: w(_w), x(_x), y(_y), z(_z)
{
}

Quaternion::Quaternion(float _w, Vector3 _xyz)
	: w(_w), x(_xyz.x), y(_xyz.y), z(_xyz.z)
{
}

Quaternion::~Quaternion() { }

float Quaternion::Norm()
{
	return sqrt(w * w + x * x + y * y + z * z);
}

Quaternion Quaternion::Normalized()
{
	if ((*this).Norm() == 0)
	{
		std::cout << "ERROR: Norm is = to 0." << std::endl;
		return Quaternion(0, 0, 0, 0);
	}
	return (*this) / (*this).Norm();
}

Quaternion Quaternion::UnitNorm()
{
	Quaternion q = (*this);

	float angle = DegToRad(q.w);
	q.w = cosf(angle * 0.5f);

	Vector3 normalize = Vector3(x, y, z).Normalize();
	normalize = normalize * sinf(angle * 0.5f);
	q.x = normalize.x;
	q.y = normalize.y;
	q.z = normalize.z;

	return q;
}

Quaternion Quaternion::Conjugate()
{
	return Quaternion(w, x * (-1), y * (-1), z * (-1));
}

Quaternion Quaternion::Inverse()
{
	float absoluteValue = Norm();
	absoluteValue *= absoluteValue;
	absoluteValue = 1 / absoluteValue;

	Quaternion conjugateValue = Conjugate();

	float scalar = conjugateValue.w * absoluteValue;
	Vector3 imaginary = Vector3(conjugateValue.x * absoluteValue, conjugateValue.y * absoluteValue, conjugateValue.z * absoluteValue);

	return Quaternion(scalar, imaginary);
}

Quaternion Quaternion::RotateQuaternionAroundAxis(float _angle, Vector3 _axis) 
{

	Quaternion p(0, x, y, z);

	Vector3 normalize = _axis.Normalize();

	Quaternion q(_angle, normalize);

	//convert quaternion to unit norm quaternion
	q = q.UnitNorm();

	//Get the inverse of the quaternion
	Quaternion qInverse = q.Inverse();

	//rotate the quaternion
	Quaternion rotatedQuaternion = q * p * qInverse;

	//return the vector part of the quaternion
	return rotatedQuaternion;

}

Quaternion Quaternion::EulerToQuaternion(Vector3 _xyzAngles)
{
	_xyzAngles = Vector3(DegToRad(_xyzAngles.x), DegToRad(_xyzAngles.y), DegToRad(_xyzAngles.z));

	float cr = (float)cos(_xyzAngles.x * 0.5);
	float sr = (float)sin(_xyzAngles.x * 0.5);
	float cp = (float)cos(_xyzAngles.y * 0.5);
	float sp = (float)sin(_xyzAngles.y * 0.5);
	float cy = (float)cos(_xyzAngles.z * 0.5);
	float sy = (float)sin(_xyzAngles.z * 0.5);

	Quaternion q;
	q.w = cr * cp * cy + sr * sp * sy;
	q.x = sr * cp * cy - cr * sp * sy;
	q.y = cr * sp * cy + sr * cp * sy;
	q.z = cr * cp * sy - sr * sp * cy;

	return q;
}

#pragma region Operators

Quaternion operator*(Quaternion _q1, Quaternion _q2)
{
	Vector3 q1i = Vector3(_q1.x, _q1.y, _q1.z);
	Vector3 q2i = Vector3(_q2.x, _q2.y, _q2.z);

	float real = _q1.w * _q2.w - Vector3::Dot(q1i, q2i);

	Vector3 imaginary = q2i * _q1.w + q1i * _q2.w + Vector3::Cross(q1i, q2i);

	return Quaternion(real, imaginary);
}

Quaternion operator/(Quaternion _q1, float _divider)
{
	if (_divider == 0.f)
	{
		std::cout << "ERROR: Divide by 0 is illegal." << std::endl;
		return Quaternion(0, Vector3(0, 0, 0));
	}
	return Quaternion(_q1.w / _divider, _q1.x / _divider, _q1.y / _divider, _q1.z / _divider);
}

bool operator==(Quaternion _q1, Quaternion _q2)
{
	if (_q1.w != _q2.w || _q1.x != _q2.x)
		return false;
	return true;
}

bool operator!=(Quaternion _q1, Quaternion _q2)
{
	return !(_q1 == _q2);
}

#pragma endregion Operators
