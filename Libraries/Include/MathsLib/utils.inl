#include "utils.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <bit>
#include <stdexcept>
#include <limits>

float DegToRad(float _deg) 
{ 
	return (float)(_deg * M_PI / 180.f);
}

float RadToDeg(float _rad) 
{	
	return (float)(_rad * 180.0f / M_PI); 
}

float Lerp(float _a, float _b, float _t) 
{	
	return (1.0f - _t) * _a + _b * _t; 
}

float InvLerp(float _a, float _b, float _v) 
{ 
	return (_v - _a) / (_b - _a); 
}

float Remap(float _iMin, float _iMax, float _oMin, float _oMax, float _value)
{
	float t = InvLerp(_iMin, _iMax, _value);
	float ov = Lerp(_oMin, _oMax, t);
	return ov;
}

float Q_rsqrt(float _number) noexcept
{
	static_assert(std::numeric_limits<float>::is_iec559);			// (enable only on IEEE 754)

	float const y = std::bit_cast<float>(0x5f3759df - (std::bit_cast<std::uint32_t>(_number) >> 1));
	return y * (1.5f - (_number * 0.5f * y * y));
}

float Q_rsqrt0(float _number, int _mode)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5f;

	x2 = _number * 0.5f;
	y = _number;
	i = *(long*)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y));			// 1st iteration
	if (_mode == 1)
		y = y * (threehalfs - (x2 * y * y));		// 2nd iteration, can be removed

	return y;
}