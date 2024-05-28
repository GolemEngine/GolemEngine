#include "vector2.h"

Vector2::Vector2() { }

Vector2::~Vector2() { }

Vector2::Vector2(float _x, float _y)
{
	x = _x;
	y = _y;
} 

Vector2 Vector2::Normalize() const
{
	float l = Length();
	if (l != 0)
	{
		return Vector2(x / l, y / l);
	}
	else
	{
		return Vector2(0, 0);
		std::cout << "ERROR: Divide by 0 is illegal." << std::endl;
	}
}

float Vector2::Length() const
{
	return sqrtf(x * x + y * y);
}

float Vector2::LengthSq() const
{
	return (x * x) + (y * y);
}

float Vector2::Dot(Vector2 _a, Vector2 _b)
{
	return _a.x * _b.x + _a.y * _b.y;
}


#pragma region Operators

Vector2 operator+(Vector2 _a, float _b)
{
	return { _a.x + _b, _a.y + _b };
}

Vector2 operator-(Vector2 _a, float _b)
{
	return { _a.x - _b, _a.y - _b };
}

Vector2 operator*(Vector2 _a, float _s)
{
	return { _a.x * _s, _a.y * _s };
}

Vector2 operator/(Vector2 _a, float _s)
{
	if (_s == 0.0f)
	{
		std::cout << "ERROR: Divide by 0 is illegal." << std::endl;
		return { 0.0f, 0.0f };
	}
	return { _a.x / _s, _a.y / _s };
}

Vector2 operator+(Vector2 _a, Vector2 _b)
{
	return { _a.x + _b.x, _a.y + _b.y };
}

Vector2 operator-(Vector2 _a, Vector2 _b)
{
	return { _a.x - _b.x, _a.y - _b.y };
}

bool operator==(Vector2 _a, Vector2 _b)
{
	if (_a.x == _b.x && _a.y == _b.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

std::ostream& operator<<(std::ostream& _os, const Vector2& _f)
{
	return _os << "{ " << _f.x << " ; " << _f.y << " }";
}

#pragma endregion Operators
