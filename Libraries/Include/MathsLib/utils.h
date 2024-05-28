#pragma once


constexpr double M_PI = 3.14159265358979323846;

float DegToRad(float _deg);
float RadToDeg(float _rad);

// https://www.youtube.com/watch?v=NzjF1pdlK7Y&t=1042s for explanation and usage of Lerp, InvLerp and Remap
float Lerp(float _a, float _b, float _t);		// returns linear interpolation of _a and _b depending on parameter _t
float InvLerp(float _a, float _b, float _v);	// returns parameter _t of the lerp depending of _a and _b and the lerp's return value _v
float Remap(float _iMin, float _iMax, float _oMin, float _oMax, float _value);		// return value of a lerp between _oMin and _oMax depending on _value located between _iMin and _iMax

// Quick inverse square root function for faster inverse square root calculations.
float Q_rsqrt(float _number) noexcept;
// Quick inverse square root function for faster inverse square root calculations. If _mode set to 1, the inverse
// square root will show more precision in exchange for slightly longer calculations. 
float Q_rsqrt0(float _number, int _mode = 0);