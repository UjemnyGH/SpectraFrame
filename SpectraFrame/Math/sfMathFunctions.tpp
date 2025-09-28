#pragma once
#ifndef _SPECTRAFRAME_MATH_FUNCTIONS_TPP_
#define _SPECTRAFRAME_MATH_FUNCTIONS_TPP_

inline float sf::reverseSqrt32(float x) {
  float y = x * 0.5f;

  long i = *reinterpret_cast<long*>(&x);
  i = 0x5f375a86 - (i >> 1);
  float r = *reinterpret_cast<float*>(&i);

  r *= (1.5f - r * r * y);
  r *= (1.5f - r * r * y);

  return r;
}

inline float sf::sqrt32(float x) {
  return 1.0f / reverseSqrt32(x);
}

template <class T>
inline T sf::abs(T value) {
  return value >= static_cast<T>(0) ? value : -value;
}

template <class T>
inline T sf::clamp(const T value, const T min, const T max) {
  return value >= max ? max : (value <= min ? min : value);
}

#endif