/*
	All math type structs are aligned to 4 bytes no matter what
*/

#pragma once
#ifndef _SPECTRAFRAME_MATH_MATRIX_
#define _SPECTRAFRAME_MATH_MATRIX_

#include "sfVector.h"

namespace sf {
	template <class T>
	struct alignas(16) Matrix4;

	template <class T>
	struct alignas(16) Matrix3 {
		Vector3<T> rows[3];

		Matrix3();
		Matrix3(T init);
		Matrix3(const Vector3<T> r0, const Vector3<T> r1, const Vector3<T> r2);

		template <class TN>
		Matrix3(const Matrix3<TN>& newMat);

		Matrix3(const Matrix4<T>& newMat);

		inline Vector3<T>& operator[](int col);

		void fill(const T& value);

		inline static Matrix3 zero();
		inline static Matrix3 one();
		inline static Matrix3 identity();

		inline static Matrix3 rotateX(const T& radians);
		inline static Matrix3 rotateY(const T& radians);
		inline static Matrix3 rotateZ(const T& radians);

		inline static Matrix3 scale(const Vector3<T>& v);

		inline Vector3<T> getXAxis();
		inline Vector3<T> getYAxis();
		inline Vector3<T> getZAxis();

		inline Vector3<T> getScale();

		inline Matrix3 transpose();

		inline Matrix3 operator+(const Matrix3& a);
		inline Matrix3 operator-(const Matrix3& a);

		inline Matrix3 operator+=(const Matrix3& a);
		inline Matrix3 operator-=(const Matrix3& a);

		inline Matrix3 operator*=(const T& a);
		inline Matrix3 operator/=(const T& a);

		inline Matrix3 operator*(const Matrix3& a);
		inline Matrix3 operator*=(const Matrix3& a);

		inline Vector3<T> operator*(const Vector3<T>& a);

		inline Vector3<T> vectorizePoint(const Vector3<T>& a);

		inline Vector3<T> rightMultiply(const Vector3<T>& a);
		inline Vector3<T> leftMultiply(const Vector3<T>& a);

		inline Vector3<T> direction(const Vector3<T>& a);

		inline T minor(int row, int col);
		inline Matrix3 cofactorMatrix();
		inline Matrix3 inverse();

		inline T* ptr();
	};

	typedef Matrix3<float> FMat3;
	typedef Matrix3<double> DMat3;

	template <class T>
	struct alignas(16) Matrix4 {
		Vector4<T> rows[4];

		Matrix4();
		Matrix4(T init);
		Matrix4(const Vector4<T>& r0, const Vector4<T>& r1, const Vector4<T>& r2, const Vector4<T>& r3);

		template <class TN>
		Matrix4(const Matrix4<TN>& newMat);

		// Makes mat 4x4 with last [4;4] = 1 (Identity)
		Matrix4(const Matrix3<T>& newMat);

		inline Vector4<T>& operator[](int col);

		void fill(const T& value);

		inline static Matrix4 zero();
		inline static Matrix4 one();
		inline static Matrix4 identity();

		inline static Matrix4 rotateX(const T& radians);
		inline static Matrix4 rotateY(const T& radians);
		inline static Matrix4 rotateZ(const T& radians);

		inline static Matrix4 translate(const Vector3<T>& v);
		inline static Matrix4 scale(const Vector3<T>& v);

		inline Vector4<T> getXAxis();
		inline Vector4<T> getYAxis();
		inline Vector4<T> getZAxis();
		inline Vector4<T> getWAxis();

		inline Vector4<T> getTranslation();
		inline Vector4<T> getScale();

		inline Matrix4 transpose();

		inline Matrix4 operator+(const Matrix4& a);
		inline Matrix4 operator-(const Matrix4& a);

		inline Matrix4 operator+=(const Matrix4& a);
		inline Matrix4 operator-=(const Matrix4& a);

		inline Matrix4 operator*=(const T& a);
		inline Matrix4 operator/=(const T& a);

		inline Matrix4 operator*(const Matrix4& a);
		inline Matrix4 operator*=(const Matrix4& a);

		inline Vector4<T> operator*(const Vector4<T>& a);

		inline Vector4<T> toPoint(const Vector4<T>& a);
		inline Vector4<T> vectorizePoint(const Vector4<T>& a);
		
		inline Vector4<T> rightMultiply(const Vector4<T>& a);
		inline Vector4<T> leftMultiply(const Vector4<T>& a);

		inline Vector4<T> direction(const Vector4<T>& a);

		inline T minor(int row, int col);
		inline Matrix4 cofactorMatrix();
		inline Matrix4 inverse();

		inline static Matrix4 perspectiveFOV(const T& fovRadians, const T& width, const T& height, const T& zNear, const T& zFar);
		inline static Matrix4 perspective(const T& right, const T& left, const T& top, const T& bottom, const T& zNear, const T& zFar);
		inline static Matrix4 perspectiveSymmertical(const T& right, const T& top, const T& zNear, const T& zFar);

		inline static Matrix4 orthographic(const T& right, const T& left, const T& top, const T& bottom, const T& zNear, const T& zFar);
		inline static Matrix4 orthographicSymmertical(const T& right, const T& top, const T& zNear, const T& zFar);

		inline static Matrix4 lookAt(const Vector3<T>& eye, const Vector3<T>& at, const Vector3<T>& up);

		inline T* ptr();
	};

	typedef Matrix4<float> FMat4;
	typedef Matrix4<double> DMat4;
}

#endif