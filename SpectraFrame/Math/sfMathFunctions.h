#pragma once
#ifndef _SPECRTRAFRAME_MATH_FUNCTIONS_
#define _SPECRTRAFRAME_MATH_FUNCTIONS_

namespace sf {
#define SF_G_EPSILON  1e-6

#ifndef M_PI
#define M_PI 3.14159265359
#endif

  inline float reverseSqrt32(float x);

  inline float sqrt32(float x);

  template <class T>
  inline T abs(T value);

  template <class T>
  inline T clamp(const T value, const T min, const T max);
}

#include "sfMathFunctions.tpp"

#endif
