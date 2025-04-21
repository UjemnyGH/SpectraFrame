/*
	All math type structs are aligned to 4 bytes no matter what
*/

#pragma once
#ifndef _SPECTRAFRAME_MATH_VECTOR_
#define _SPECTRAFRAME_MATH_VECTOR_

namespace sf {
	template <class T>
	struct alignas(16) Vector4;

	template <class T>
	struct alignas(16) Vector3 {
		T x, y, z;

		Vector3();
		Vector3(T init);
		Vector3(T _x, T _y, T _z = static_cast<T>(0));

		template <class TN>
		Vector3(const Vector3<TN>& cast);

		Vector3(const Vector4<T>& newVec);

		inline T& operator[](int axisIndex);

		inline Vector3 operator+(const Vector3& a);
		inline Vector3 operator-(const Vector3& a);
		inline Vector3 operator*(const Vector3& a);
		inline Vector3 operator/(const Vector3& a);

		inline Vector3 operator+(const T& a);
		inline Vector3 operator-(const T& a);
		inline Vector3 operator*(const T& a);
		inline Vector3 operator/(const T& a);

		inline Vector3 operator+=(const Vector3& a);
		inline Vector3 operator-=(const Vector3& a);
		inline Vector3 operator*=(const Vector3& a);
		inline Vector3 operator/=(const Vector3& a);

		inline Vector3 operator+=(const T& a);
		inline Vector3 operator-=(const T& a);
		inline Vector3 operator*=(const T& a);
		inline Vector3 operator/=(const T& a);

		inline void operator=(const Vector3& a);

		inline bool operator==(const Vector3& a);
		inline bool operator!=(const Vector3& a);

		inline T dot(const Vector3& a);
		inline static T dot(const Vector3& a, const Vector3& b);

		inline Vector3 cross(const Vector3& a);
		inline static Vector3 cross(const Vector3& a, const Vector3& b);

		inline T length();
		inline static T length(const Vector3& a);

		inline T distance(const Vector3& a);
		inline static T distance(const Vector3& a, const Vector3& b);

		inline Vector3 normalize();
		inline static Vector3 normalize(const Vector3& a);

		inline Vector3 abs();
		inline static Vector3 abs(const Vector3& a);

		inline Vector3 negate();
		inline static Vector3 negate(const Vector3& a);

		inline Vector3 clamp(const T& min, const T& max);
		inline static Vector3 clamp(const Vector3& vec, const T& min, const T& max);

		inline T* ptr();
	};

	typedef Vector3<float>	FVec3;
	typedef Vector3<double> DVec3;

	template <class T>
	struct alignas(16) Vector4 {
		T x, y, z, w;

		Vector4();
		Vector4(T init);
		Vector4(T _x, T _y, T _z = static_cast<T>(0), T _w = static_cast<T>(0));

		template <class TN>
		Vector4(const Vector4<TN>& cast);

		Vector4(const Vector3<T>& newVec);

		inline T& operator[](int axisIndex);

		inline Vector4 operator+(const Vector4& a);
		inline Vector4 operator-(const Vector4& a);
		inline Vector4 operator*(const Vector4& a);
		inline Vector4 operator/(const Vector4& a);

		inline Vector4 operator+(const T& a);
		inline Vector4 operator-(const T& a);
		inline Vector4 operator*(const T& a);
		inline Vector4 operator/(const T& a);

		inline Vector4 operator+=(const Vector4& a);
		inline Vector4 operator-=(const Vector4& a);
		inline Vector4 operator*=(const Vector4& a);
		inline Vector4 operator/=(const Vector4& a);

		inline Vector4 operator+=(const T& a);
		inline Vector4 operator-=(const T& a);
		inline Vector4 operator*=(const T& a);
		inline Vector4 operator/=(const T& a);

		inline void operator=(const Vector4& a);

		inline bool operator==(const Vector4& a);
		inline bool operator!=(const Vector4& a);
		
		inline T dot(const Vector4& a);
		inline static T dot(const Vector4& a, const Vector4& b);

		inline Vector4 cross(const Vector4& a);
		inline static Vector4 cross(const Vector4& a, const Vector4& b);

		inline T length();
		inline static T length(const Vector4& a);

		inline T distance(const Vector4& a);
		inline static T distance(const Vector4& a, const Vector4& b);

		inline Vector4 normalize();
		inline static Vector4 normalize(const Vector4& a);

		inline Vector4 abs();
		inline static Vector4 abs(const Vector4& a);

		inline Vector4 negate();
		inline static Vector4 negate(const Vector4& a);

		inline Vector4 clamp(const T& min, const T& max);
		inline static Vector4 clamp(const Vector4& vec, const T& min, const T& max);

		inline T* ptr();
	};

	typedef Vector4<float>	FVec4;
	typedef Vector4<double> DVec4;
}

#endif