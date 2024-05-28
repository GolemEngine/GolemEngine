#include "vector4.h"
#include "matrix4.h"


Vector4::Vector4() { }

Vector4::~Vector4() { }

Vector4::Vector4(float _x, float _y, float _z, float _w)
	: x(_x), y(_y), z(_z), w(_w) { }


float Vector4::Length() const
{
	return sqrtf(x * x + y * y + z * z + w * w);
}

float Vector4::LengthSq() const
{
	return x * x + y * y + z * z + w * w;
}

Vector4 Vector4::Normalize() const
{
	float l = Length();
	if (l != 0)
	{
		return Vector4(x / l, y / l, z / l, w / l);
	}
	else
	{
		return Vector4(0, 0, 0, 0);
	}
}


#pragma region Operators

Vector4 operator+(Vector4 _a, float _b)
{
	return { _a.x + _b, _a.y + _b, _a.z + _b, _a.w + _b };
}

Vector4 operator-(Vector4 _a, float _b)
{
	return { _a.x - _b, _a.y - _b, _a.z - _b, _a.w - _b };
}

Vector4 operator*(Vector4 _a, float _s)
{
	return { _a.x * _s, _a.y * _s, _a.z * _s, _a.w * _s };
}

Vector4 operator/(Vector4 _a, float _s)
{
	if (_s == 0)
	{
		std::cout << "ERROR: Divide by 0 is illegal." << std::endl;
		return { 0.0f, 0.0f, 0.0f, 0.0f };
	}
	return { _a.x / _s, _a.y / _s, _a.z / _s, _a.w / _s };
}

Vector4 operator+(Vector4 _a, Vector4 _b)
{
	return { _a.x + _b.x, _a.y + _b.y, _a.z + _b.z, _a.w + _b.w };
}

Vector4 operator-(Vector4 _a, Vector4 _b)
{
	return { _a.x - _b.x, _a.y - _b.y, _a.z - _b.z, _a.w - _b.w };
}

bool operator==(Vector4 _a, Vector4 _b)
{
	return (_a.x == _b.x && _a.y == _b.y && _a.z == _b.z && _a.w == _b.w) ? true : false;
}

Vector4 operator*(Matrix4 _a, Vector4 _b)
{
	return {
		_a.data[0][0] * _b.x + _a.data[0][1] * _b.y + _a.data[0][2] * _b.z + _a.data[0][3] * _b.w,
		_a.data[1][0] * _b.x + _a.data[1][1] * _b.y + _a.data[1][2] * _b.z + _a.data[1][3] * _b.w,
		_a.data[2][0] * _b.x + _a.data[2][1] * _b.y + _a.data[2][2] * _b.z + _a.data[2][3] * _b.w,
		_a.data[3][0] * _b.x + _a.data[3][1] * _b.y + _a.data[3][2] * _b.z + _a.data[3][3] * _b.w
	};
}

std::ostream& operator<<(std::ostream& _os, const Vector4& _f)
{
	return _os << "{ " << _f.x << " ; " << _f.y << " ; " << _f.z <<  " ; " << _f.w << " }";
}

#pragma endregion Operators