#include "sfMathFunctions.h"

float sf::reverseSqrt32(float x) {
	float y = x * 0.5f;

	long i = *reinterpret_cast<long*>(&x);
	i = 0x5f375a86 - (i >> 1);
	float r = *reinterpret_cast<float*>(&i);

	r *= (1.5f - r * r * y);
	r *= (1.5f - r * r * y);

	return r;
}

float sf::sqrt32(float x) {
	return 1.0f / reverseSqrt32(x);
}

template <class T>
T sf::abs(T value) {
	return value >= static_cast<T>(0) ? value : -value;
}

template <class T>
T sf::clamp(const T value, const T min, const T max) {
	return value >= max ? max : (value <= min ? min : value);
}

template <class T>
T sf::min(const T a, const T b) {
	return a >= b ? b : a;
}

template <class T>
T sf::max(const T a, const T b) {
	return a >= b ? a : b;
}

template <class T>
T sf::copysign(const T value, const T sign) {
	return sign > static_cast<T>(0) ? value : -value;
}