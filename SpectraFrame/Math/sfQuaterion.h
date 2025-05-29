/*
  All math type structs are aligned to 4 bytes no matter what
*/

#pragma once
#ifndef _SPECTRAFRAME_MATH_QUATERION_
#define _SPECTRAFRAME_MATH_QUATERION_

#include "sfVector.h"
#include "sfMatrix.h"

namespace sf {
  template <class T>
  struct alignas(16) Quaterion {
    T w, x, y, z;
    
    constexpr Quaterion();
    constexpr Quaterion(T _w, T _x, T _y, T _z);
    constexpr Quaterion(T radians, const Vector3<T>& axis);

    constexpr T length();

    constexpr Quaterion normalize();
    constexpr Quaterion conjuged();
    constexpr Quaterion invert();
    constexpr T dot(const Quaterion& a);

    constexpr static T length(const Quaterion& a);

    constexpr static Quaterion normalize(const Quaterion& a);
    constexpr static Quaterion conjuged(const Quaterion& a);
    constexpr static Quaterion invert(const Quaterion& a);
    constexpr static T dot(const Quaterion& a, const Quaterion& b);

    constexpr Vector3<T> operator*(const Vector3<T>& a);
    constexpr Quaterion operator*(const Quaterion& a);
    constexpr Quaterion operator*(const T& a);
    constexpr Quaterion operator/(const T& a);

    constexpr Matrix3<T> toMat3();

    constexpr static Quaterion toQuat(const Matrix3<T>& mat);

    constexpr static Quaterion slerp(const Quaterion& a, const Quaterion& b, const T& t);
    constexpr static Quaterion lookRotation(const Vector3<T>& forward, const Vector3<T>& up);
    constexpr static Quaterion lookAt(const Vector3<T>& forward, const Vector3<T>& up);

    constexpr static Quaterion fromEuler(const T& pitch, const T& yaw, const T& roll);
    constexpr Vector3<T> toEuler();
  };

  typedef Quaterion<float> FQuat;
  typedef Quaterion<double> DQuat;
}

#endif
