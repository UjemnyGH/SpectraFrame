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
		
		Quaterion();
		Quaterion(T _w, T _x, T _y, T _z);
		Quaterion(T radians, const Vector3<T>& axis);

		inline T length();

		inline Quaterion normalize();
		inline Quaterion conjuged();
		inline Quaterion invert();
		inline T dot(const Quaterion& a);

		inline static T length(const Quaterion& a);

		inline static Quaterion normalize(const Quaterion& a);
		inline static Quaterion conjuged(const Quaterion& a);
		inline static Quaterion invert(const Quaterion& a);
		inline static T dot(const Quaterion& a, const Quaterion& b);

		inline Vector3<T> operator*(const Vector3<T>& a);
		inline Quaterion operator*(const Quaterion& a);
		inline Quaterion operator*(const T& a);
		inline Quaterion operator/(const T& a);

		inline Matrix3<T> toMat3();

		inline static Quaterion toQuat(const Matrix3<T>& mat);

		inline static Quaterion slerp(const Quaterion& a, const Quaterion& b, const T& t);
		inline static Quaterion lookRotation(const Vector3<T>& forward, const Vector3<T>& up);
		inline static Quaterion lookAt(const Vector3<T>& forward, const Vector3<T>& up);

		inline static Quaterion fromEuler(const T& pitch, const T& yaw, const T& roll);
		inline Vector3<T> toEuler();
	};

	typedef Quaterion<float> FQuat;
	typedef Quaterion<double> DQuat;
}

#endif