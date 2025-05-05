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

		constexpr Vector3();
		constexpr Vector3(T init);
		constexpr Vector3(T _x, T _y, T _z = static_cast<T>(0));

		template <class TN>
		constexpr Vector3(const Vector3<TN>& cast);

		constexpr Vector3(const Vector4<T>& newVec);

		constexpr T& operator[](int axisIndex);

		constexpr Vector3 operator+(const Vector3& a);
		constexpr Vector3 operator-(const Vector3& a);
		constexpr Vector3 operator*(const Vector3& a);
		constexpr Vector3 operator/(const Vector3& a);

		constexpr Vector3 operator+(const T& a);
		constexpr Vector3 operator-(const T& a);
		constexpr Vector3 operator*(const T& a);
		constexpr Vector3 operator/(const T& a);

		constexpr Vector3 operator+=(const Vector3& a);
		constexpr Vector3 operator-=(const Vector3& a);
		constexpr Vector3 operator*=(const Vector3& a);
		constexpr Vector3 operator/=(const Vector3& a);

		constexpr Vector3 operator+=(const T& a);
		constexpr Vector3 operator-=(const T& a);
		constexpr Vector3 operator*=(const T& a);
		constexpr Vector3 operator/=(const T& a);

		constexpr void operator=(const Vector3& a);

		constexpr bool operator==(const Vector3& a);
		constexpr bool operator!=(const Vector3& a);

		constexpr T dot(const Vector3& a);
		constexpr static T dot(const Vector3& a, const Vector3& b);

		constexpr Vector3 cross(const Vector3& a);
		constexpr static Vector3 cross(const Vector3& a, const Vector3& b);

		constexpr T length();
		constexpr static T length(const Vector3& a);

		constexpr T distance(const Vector3& a);
		constexpr static T distance(const Vector3& a, const Vector3& b);

		constexpr Vector3 normalize();
		constexpr static Vector3 normalize(const Vector3& a);

		constexpr Vector3 abs();
		constexpr static Vector3 abs(const Vector3& a);

		constexpr Vector3 negate();
		constexpr static Vector3 negate(const Vector3& a);

		constexpr Vector3 clamp(const T& min, const T& max);
		constexpr static Vector3 clamp(const Vector3& vec, const T& min, const T& max);

		constexpr T* ptr();
	};

	typedef Vector3<float>	FVec3;
	typedef Vector3<double> DVec3;

	template <class T>
	struct alignas(16) Vector4 {
		T x, y, z, w;

		constexpr Vector4();
		constexpr Vector4(T init);
		constexpr Vector4(T _x, T _y, T _z = static_cast<T>(0), T _w = static_cast<T>(0));

		template <class TN>
		constexpr Vector4(const Vector4<TN>& cast);

		constexpr Vector4(const Vector3<T>& newVec);

		constexpr T& operator[](int axisIndex);

		constexpr Vector4 operator+(const Vector4& a);
		constexpr Vector4 operator-(const Vector4& a);
		constexpr Vector4 operator*(const Vector4& a);
		constexpr Vector4 operator/(const Vector4& a);

		constexpr Vector4 operator+(const T& a);
		constexpr Vector4 operator-(const T& a);
		constexpr Vector4 operator*(const T& a);
		constexpr Vector4 operator/(const T& a);

		constexpr Vector4 operator+=(const Vector4& a);
		constexpr Vector4 operator-=(const Vector4& a);
		constexpr Vector4 operator*=(const Vector4& a);
		constexpr Vector4 operator/=(const Vector4& a);

		constexpr Vector4 operator+=(const T& a);
		constexpr Vector4 operator-=(const T& a);
		constexpr Vector4 operator*=(const T& a);
		constexpr Vector4 operator/=(const T& a);

		constexpr void operator=(const Vector4& a);

		constexpr bool operator==(const Vector4& a);
		constexpr bool operator!=(const Vector4& a);
		
		constexpr T dot(const Vector4& a);
		constexpr static T dot(const Vector4& a, const Vector4& b);

		constexpr Vector4 cross(const Vector4& a);
		constexpr static Vector4 cross(const Vector4& a, const Vector4& b);

		constexpr T length();
		constexpr static T length(const Vector4& a);

		constexpr T distance(const Vector4& a);
		constexpr static T distance(const Vector4& a, const Vector4& b);

		constexpr Vector4 normalize();
		constexpr static Vector4 normalize(const Vector4& a);

		constexpr Vector4 abs();
		constexpr static Vector4 abs(const Vector4& a);

		constexpr Vector4 negate();
		constexpr static Vector4 negate(const Vector4& a);

		constexpr Vector4 clamp(const T& min, const T& max);
		constexpr static Vector4 clamp(const Vector4& vec, const T& min, const T& max);

		constexpr T* ptr();
	};

	typedef Vector4<float>	FVec4;
	typedef Vector4<double> DVec4;
}

#endif