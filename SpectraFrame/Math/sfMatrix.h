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

    constexpr Matrix3();
    constexpr Matrix3(T init);
    constexpr Matrix3(const Vector3<T> r0, const Vector3<T> r1, const Vector3<T> r2);

    template <class TN>
    constexpr Matrix3(const Matrix3<TN>& newMat);

    constexpr Matrix3(const Matrix4<T>& newMat);

    constexpr Vector3<T>& operator[](int col);

    void fill(const T& value);

    constexpr static Matrix3 zero();
    constexpr static Matrix3 one();
    constexpr static Matrix3 identity();

    constexpr static Matrix3 rotateX(const T& radians);
    constexpr static Matrix3 rotateY(const T& radians);
    constexpr static Matrix3 rotateZ(const T& radians);

    constexpr static Matrix3 scale(const Vector3<T>& v);

    constexpr Vector3<T> getXAxis();
    constexpr Vector3<T> getYAxis();
    constexpr Vector3<T> getZAxis();

    constexpr Vector3<T> getScale();

    constexpr Matrix3 transpose();

    constexpr Matrix3 operator+(const Matrix3& a);
    constexpr Matrix3 operator-(const Matrix3& a);

    constexpr Matrix3 operator+=(const Matrix3& a);
    constexpr Matrix3 operator-=(const Matrix3& a);

    constexpr Matrix3 operator*=(const T& a);
    constexpr Matrix3 operator/=(const T& a);

    constexpr Matrix3 operator*(const Matrix3& a);
    constexpr Matrix3 operator*=(const Matrix3& a);

    constexpr Vector3<T> operator*(const Vector3<T>& a);

    constexpr Vector3<T> vectorizePoint(const Vector3<T>& a);

    constexpr Vector3<T> rightMultiply(const Vector3<T>& a);
    constexpr Vector3<T> leftMultiply(const Vector3<T>& a);

    constexpr Vector3<T> direction(const Vector3<T>& a);

    constexpr T minor(int row, int col);
    constexpr Matrix3 cofactorMatrix();
    constexpr Matrix3 inverse();

    constexpr T* ptr();
  };

  typedef Matrix3<float> FMat3;
  typedef Matrix3<double> DMat3;

  template <class T>
  struct alignas(16) Matrix4 {
    Vector4<T> rows[4];

    constexpr Matrix4();
    constexpr Matrix4(T init);
    constexpr Matrix4(const Vector4<T>& r0, const Vector4<T>& r1, const Vector4<T>& r2, const Vector4<T>& r3);

    template <class TN>
    constexpr Matrix4(const Matrix4<TN>& newMat);

    // Makes mat 4x4 with last [4;4] = 1 (Identity)
    constexpr Matrix4(const Matrix3<T>& newMat);

    constexpr Vector4<T>& operator[](int col);

    void fill(const T& value);

    constexpr static Matrix4 zero();
    constexpr static Matrix4 one();
    constexpr static Matrix4 identity();

    constexpr static Matrix4 rotateX(const T& radians);
    constexpr static Matrix4 rotateY(const T& radians);
    constexpr static Matrix4 rotateZ(const T& radians);

    constexpr static Matrix4 translate(const Vector3<T>& v);
    constexpr static Matrix4 scale(const Vector3<T>& v);

    constexpr Vector4<T> getXAxis();
    constexpr Vector4<T> getYAxis();
    constexpr Vector4<T> getZAxis();
    constexpr Vector4<T> getWAxis();

    constexpr Vector4<T> getTranslation();
    constexpr Vector4<T> getScale();

    constexpr Matrix4 transpose();

    constexpr Matrix4 operator+(const Matrix4& a);
    constexpr Matrix4 operator-(const Matrix4& a);

    constexpr Matrix4 operator+=(const Matrix4& a);
    constexpr Matrix4 operator-=(const Matrix4& a);

    constexpr Matrix4 operator*=(const T& a);
    constexpr Matrix4 operator/=(const T& a);

    constexpr Matrix4 operator*(const Matrix4& a);
    constexpr Matrix4 operator*=(const Matrix4& a);

    constexpr Vector4<T> operator*(const Vector4<T>& a);

    constexpr Vector4<T> toPoint(const Vector4<T>& a);
    constexpr Vector4<T> vectorizePoint(const Vector4<T>& a);
    
    constexpr Vector4<T> rightMultiply(const Vector4<T>& a);
    constexpr Vector4<T> leftMultiply(const Vector4<T>& a);

    constexpr Vector4<T> direction(const Vector4<T>& a);

    constexpr T minor(int row, int col);
    constexpr Matrix4 cofactorMatrix();
    constexpr Matrix4 inverse();

    constexpr static Matrix4 perspectiveFOV(const T& fovRadians, const T& width, const T& height, const T& zNear, const T& zFar);
    constexpr static Matrix4 perspective(const T& right, const T& left, const T& top, const T& bottom, const T& zNear, const T& zFar);
    constexpr static Matrix4 perspectiveSymmertical(const T& right, const T& top, const T& zNear, const T& zFar);

    constexpr static Matrix4 orthographic(const T& right, const T& left, const T& top, const T& bottom, const T& zNear, const T& zFar);
    constexpr static Matrix4 orthographicSymmertical(const T& right, const T& top, const T& zNear, const T& zFar);

    constexpr static Matrix4<T> lookAt(const Vector3<T>& eye, const Vector3<T>& at, const Vector3<T>& up);

    constexpr T* ptr();
  };

  typedef Matrix4<float> FMat4;
  typedef Matrix4<double> DMat4;
}

#endif
