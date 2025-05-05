#pragma once
#ifndef _SPECRTRAFRAME_MATH_FUNCTIONS_
#define _SPECRTRAFRAME_MATH_FUNCTIONS_

namespace sf {
#define SF_G_EPSILON	1e-6

#ifndef M_PI
#define M_PI 3.14159265359
#endif

	float reverseSqrt32(float x);

	float sqrt32(float x);

	template <class T>
	T abs(T value);

	template <class T>
	T clamp(const T value, const T min, const T max);

	template <class T>
	T min(const T a, const T b);

	template <class T>
	T max(const T a, const T b);

	template <class T>
	T copysign(const T value, const T sign);
}

#endif