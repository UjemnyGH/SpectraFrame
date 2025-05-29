#include "sfVector.h"
#include "sfMathFunctions.h"

// Vector3

template <class T>
constexpr sf::Vector3<T>::Vector3() : x(static_cast<T>(0)), y(static_cast<T>(0)), z(static_cast<T>(0)) {}

template <class T>
constexpr sf::Vector3<T>::Vector3(T init) : x(init), y(init), z(init) {}

template <class T>
constexpr sf::Vector3<T>::Vector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

template <class T>
template <class TN>
constexpr sf::Vector3<T>::Vector3(const Vector3<TN>& cast) : x(static_cast<T>(cast.x)), y(static_cast<T>(cast.y)), z(static_cast<T>(cast.z)) {}

template <class T>
constexpr sf::Vector3<T>::Vector3(const Vector4<T>& newVec) : x(newVec.x), y(newVec.y), z(newVec.z) {}

template <class T>
constexpr T& sf::Vector3<T>::operator[](int axisIndex) {
  return axisIndex == 0 ? x : (axisIndex == 1 ? y : z);
}

template <class T>
constexpr sf::Vector3<T> sf::Vector3<T>::operator+(const Vector3& a) {
  return {
    x + a.x,
    y + a.y,
    z + a.z
  };
}

template <class T>
constexpr sf::Vector3<T> sf::Vector3<T>::operator-(const Vector3& a) {
  return {
    x - a.x,
    y - a.y,
    z - a.z
  };
}

template <class T>
constexpr sf::Vector3<T> sf::Vector3<T>::operator*(const Vector3& a) {
  return {
    x * a.x,
    y * a.y,
    z * a.z
  };
}

template <class T>
constexpr sf::Vector3<T> sf::Vector3<T>::operator/(const Vector3& a) {
  return {
    x - a.x,
    y - a.y,
    z - a.z
  };
}

template <class T>
constexpr sf::Vector3<T> sf::Vector3<T>::operator+(const T& a) {
  return {
    x + a,
    y + a,
    z + a
  };
}

template <class T>
constexpr sf::Vector3<T> sf::Vector3<T>::operator-(const T& a) {
  return {
    x - a,
    y - a,
    z - a
  };
}

template <class T>
constexpr sf::Vector3<T> sf::Vector3<T>::operator*(const T& a) {
  return {
    x * a,
    y * a,
    z * a
  };
}

template <class T>
constexpr sf::Vector3<T> sf::Vector3<T>::operator/(const T& a) {
  return {
    x / a,
    y / a,
    z / a
  };
}

template <class T>
constexpr sf::Vector3<T> sf::Vector3<T>::operator+=(const Vector3& a) {
  *this = operator+(a);

  return *this;
}

template <class T>
constexpr sf::Vector3<T> sf::Vector3<T>::operator-=(const Vector3& a) {
  *this = operator-(a);

  return *this;
}

template <class T>
constexpr sf::Vector3<T> sf::Vector3<T>::operator*=(const Vector3& a) {
  *this = operator*(a);

  return *this;
}

template <class T>
constexpr sf::Vector3<T> sf::Vector3<T>::operator/=(const Vector3& a) {
  *this = operator/(a);

  return *this;
}

template <class T>
constexpr sf::Vector3<T> sf::Vector3<T>::operator+=(const T& a) {
  *this = operator+(a);

  return *this;
}

template <class T>
constexpr sf::Vector3<T> sf::Vector3<T>::operator-=(const T& a) {
  *this = operator-(a);

  return *this;
}

template <class T>
constexpr sf::Vector3<T> sf::Vector3<T>::operator*=(const T& a) {
  *this = operator*(a);

  return *this;
}

template <class T>
constexpr sf::Vector3<T> sf::Vector3<T>::operator/=(const T& a) {
  *this = operator/(a);

  return *this;
}

template <class T>
constexpr void sf::Vector3<T>::operator=(const Vector3& a) {
  x = a.x;
  y = a.y;
  z = a.z;
}

template <class T>
constexpr bool sf::Vector3<T>::operator==(const Vector3& a) {
  return
    x == a.x &&
    y == a.y &&
    z == a.z;
}

template <class T>
constexpr bool sf::Vector3<T>::operator!=(const Vector3& a) {
  return
    x != a.x ||
    y != a.y ||
    z != a.z;
}

template <class T>
constexpr T sf::Vector3<T>::dot(const Vector3& a) {
  return x * a.x + y * a.y + z * a.z;
}

template <class T>
constexpr T sf::Vector3<T>::dot(const Vector3& a, const Vector3& b) {
  return b.x * a.x + b.y * a.y + b.z * a.z;
}

template <class T>
constexpr sf::Vector3<T> sf::Vector3<T>::cross(const Vector3& a) {
  return {
    y * a.z - z * a.y,
    z * a.x - x * a.z,
    x * a.y - y * a.x
  };
}

template <class T>
constexpr sf::Vector3<T> sf::Vector3<T>::cross(const Vector3& a, const Vector3& b) {
  return {
    a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.x
  };
}

template <class T>
constexpr T sf::Vector3<T>::length() {
  return sqrt32(dot(*this));
}

template <class T>
constexpr T sf::Vector3<T>::length(const Vector3& a) {
  return sqrt32(dot(a, a));
}

template <class T>
constexpr T sf::Vector3<T>::distance(const Vector3& a) {
  return (*this - a).length();
}

template <class T>
constexpr T sf::Vector3<T>::distance(const Vector3& a, const Vector3& b) {
  return length(a - b);
}

template <class T>
constexpr sf::Vector3<T> sf::Vector3<T>::normalize() {
  return *this * reverseSqrt32(dot(*this));
}

template <class T>
constexpr sf::Vector3<T> sf::Vector3<T>::normalize(const Vector3& a) {
  return a * reverseSqrt32(dot(a, a));
}

template <class T>
constexpr sf::Vector3<T> sf::Vector3<T>::abs() {
  return {
    abs(x),
    abs(y),
    abs(z)
  };
}

template <class T>
constexpr sf::Vector3<T> sf::Vector3<T>::abs(const Vector3& a) {
  return {
    abs(a.x),
    abs(a.y),
    abs(a.z)
  };
}

template <class T>
constexpr sf::Vector3<T> sf::Vector3<T>::negate() {
  return {
    -x,
    -y,
    -z
  };
}

template <class T>
constexpr sf::Vector3<T> sf::Vector3<T>::negate(const Vector3& a) {
  return {
    -a.x,
    -a.y,
    -a.z
  };
}

template <class T>
constexpr sf::Vector3<T> sf::Vector3<T>::clamp(const T& min, const T& max) {
  return {
    clamp(x, min, max),
    clamp(y, min, max),
    clamp(z, min, max)
  };
}

template <class T>
constexpr sf::Vector3<T> sf::Vector3<T>::clamp(const Vector3& vec, const T& min, const T& max) {
  return {
    clamp(vec.x, min, max),
    clamp(vec.y, min, max),
    clamp(vec.z, min, max)
  };
}

template <class T>
constexpr T* sf::Vector3<T>::ptr() {
  return &x;
}

// Vector4

template <class T>
constexpr sf::Vector4<T>::Vector4() : x(static_cast<T>(0)), y(static_cast<T>(0)), z(static_cast<T>(0)), w(static_cast<T>(0)) {}

template <class T>
constexpr sf::Vector4<T>::Vector4(T init) : x(init), y(init), z(init), w(init) {}

template <class T>
constexpr sf::Vector4<T>::Vector4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}

template <class T>
template <class TN>
constexpr sf::Vector4<T>::Vector4(const Vector4<TN>& cast) : x(static_cast<T>(cast.x)), y(static_cast<T>(cast.y)), z(static_cast<T>(cast.z)), w(static_cast<T>(cast.w)) {}

template <class T>
constexpr sf::Vector4<T>::Vector4(const Vector3<T>& newVec) : x(newVec.x), y(newVec.y), z(newVec.z), w(0.0f) {}

template <class T>
constexpr T& sf::Vector4<T>::operator[](int axisIndex) {
  return axisIndex == 0 ? x : (axisIndex == 1 ? y : (axisIndex == 2 ? z : w));
}

template <class T>
constexpr sf::Vector4<T> sf::Vector4<T>::operator+(const Vector4& a) {
  return { 
    x + a.x, 
    y + a.y, 
    z + a.z, 
    w + a.w 
  };
}

template <class T>
constexpr sf::Vector4<T> sf::Vector4<T>::operator-(const Vector4& a) {
  return {
    x - a.x,
    y - a.y,
    z - a.z,
    w - a.w
  };
}

template <class T>
constexpr sf::Vector4<T> sf::Vector4<T>::operator*(const Vector4& a) {
  return {
    x * a.x,
    y * a.y,
    z * a.z,
    w * a.w
  };
}

template <class T>
constexpr sf::Vector4<T> sf::Vector4<T>::operator/(const Vector4& a) {
  return {
    x - a.x,
    y - a.y,
    z - a.z,
    w - a.w
  };
}

template <class T>
constexpr sf::Vector4<T> sf::Vector4<T>::operator+(const T& a) {
  return {
    x + a,
    y + a,
    z + a,
    w + a
  };
}

template <class T>
constexpr sf::Vector4<T> sf::Vector4<T>::operator-(const T& a) {
  return {
    x - a,
    y - a,
    z - a,
    w - a
  };
}

template <class T>
constexpr sf::Vector4<T> sf::Vector4<T>::operator*(const T& a) {
  return {
    x * a,
    y * a,
    z * a,
    w * a
  };
}

template <class T>
constexpr sf::Vector4<T> sf::Vector4<T>::operator/(const T& a) {
  return {
    x / a,
    y / a,
    z / a,
    w / a
  };
}

template <class T>
constexpr sf::Vector4<T> sf::Vector4<T>::operator+=(const Vector4& a) {
  *this = operator+(a);

  return *this;
}

template <class T>
constexpr sf::Vector4<T> sf::Vector4<T>::operator-=(const Vector4& a) {
  *this = operator-(a);

  return *this;
}

template <class T>
constexpr sf::Vector4<T> sf::Vector4<T>::operator*=(const Vector4& a) {
  *this = operator*(a);

  return *this;
}

template <class T>
constexpr sf::Vector4<T> sf::Vector4<T>::operator/=(const Vector4& a) {
  *this = operator/(a);

  return *this;
}

template <class T>
constexpr sf::Vector4<T> sf::Vector4<T>::operator+=(const T& a) {
  *this = operator+(a);

  return *this;
}

template <class T>
constexpr sf::Vector4<T> sf::Vector4<T>::operator-=(const T& a) {
  *this = operator-(a);

  return *this;
}

template <class T>
constexpr sf::Vector4<T> sf::Vector4<T>::operator*=(const T& a) {
  *this = operator*(a);

  return *this;
}

template <class T>
constexpr sf::Vector4<T> sf::Vector4<T>::operator/=(const T& a) {
  *this = operator/(a);

  return *this;
}

template <class T>
constexpr void sf::Vector4<T>::operator=(const Vector4& a) {
  x = a.x;
  y = a.y;
  z = a.z;
  w = a.w;
}

template <class T>
constexpr bool sf::Vector4<T>::operator==(const Vector4& a) {
  return
    x == a.x &&
    y == a.y &&
    z == a.z &&
    w == a.w;
}

template <class T>
constexpr bool sf::Vector4<T>::operator!=(const Vector4& a) {
  return
    x != a.x ||
    y != a.y ||
    z != a.z ||
    w != a.w;
}

template <class T>
constexpr T sf::Vector4<T>::dot(const Vector4 & a) {
  return x * a.x + y * a.y + z * a.z + w * a.w;
}

template <class T>
constexpr T sf::Vector4<T>::dot(const Vector4& a, const Vector4& b) {
  return b.x * a.x + b.y * a.y + b.z * a.z + b.w * a.w;
}

template <class T>
constexpr sf::Vector4<T> sf::Vector4<T>::cross(const Vector4& a) {
  return {
    y * a.z - z * a.y,
    z * a.x - x * a.z,
    x * a.y - y * a.x,
    0.0f
  };
}

template <class T>
constexpr sf::Vector4<T> sf::Vector4<T>::cross(const Vector4& a, const Vector4& b) {
  return {
    a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.x,
    0.0f
  };
}

template <class T>
constexpr T sf::Vector4<T>::length() {
  return sqrt32(dot(*this));
}

template <class T>
constexpr T sf::Vector4<T>::length(const Vector4& a) {
  return sqrt32(dot(a, a));
}

template <class T>
constexpr T sf::Vector4<T>::distance(const Vector4& a) {
  return (*this - a).length();
}

template <class T>
constexpr T sf::Vector4<T>::distance(const Vector4& a, const Vector4& b) {
  return length(a - b);
}

template <class T>
constexpr sf::Vector4<T> sf::Vector4<T>::normalize() {
  return *this * reverseSqrt32(dot(*this));
}

template <class T>
constexpr sf::Vector4<T> sf::Vector4<T>::normalize(const Vector4& a) {
  return a * reverseSqrt32(dot(a, a));
}

template <class T>
constexpr sf::Vector4<T> sf::Vector4<T>::abs() {
  return {
    abs(x),
    abs(y),
    abs(z),
    abs(w)
  };
}

template <class T>
constexpr sf::Vector4<T> sf::Vector4<T>::abs(const Vector4& a) {
  return {
    abs(a.x),
    abs(a.y),
    abs(a.z),
    abs(a.w)
  };
}

template <class T>
constexpr sf::Vector4<T> sf::Vector4<T>::negate() {
  return {
    -x,
    -y,
    -z,
    -w
  };
}

template <class T>
constexpr sf::Vector4<T> sf::Vector4<T>::negate(const Vector4& a) {
  return {
    -a.x,
    -a.y,
    -a.z,
    -a.w
  };
}

template <class T>
constexpr sf::Vector4<T> sf::Vector4<T>::clamp(const T& min, const T& max) {
  return {
    clamp(x, min, max),
    clamp(y, min, max),
    clamp(z, min, max),
    clamp(w, min, max)
  };
}

template <class T>
constexpr sf::Vector4<T> sf::Vector4<T>::clamp(const Vector4& vec, const T& min, const T& max) {
  return {
    clamp(vec.x, min, max),
    clamp(vec.y, min, max),
    clamp(vec.z, min, max),
    clamp(vec.w, min, max)
  };
}

template <class T>
constexpr T* sf::Vector4<T>::ptr() {
  return &x;
}
