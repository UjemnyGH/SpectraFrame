#include "sfQuaterion.h"
#include "sfMathFunctions.h"
#include <cmath>

template <class T>
sf::Quaterion<T>::Quaterion() : w(static_cast<T>(1)), x(static_cast<T>(0)), y(static_cast<T>(0)), z(static_cast<T>(0)) {}

template <class T>
sf::Quaterion<T>::Quaterion(T _w, T _x, T _y, T _z) : w(_w), x(_x), y(_y), z(_z) {}

template <class T>
sf::Quaterion<T>::Quaterion(T radians, const Vector3<T>& axis) {
	const T half = radians * static_cast<T>(0.5);
	const T halfSin = std::sin(half);
	w = std::cos(half);
	x = axis.x * halfSin;
	y = axis.y * halfSin;
	z = axis.z * halfSin;
}

template <class T>
inline T sf::Quaterion<T>::length() {
	return sf::sqrt32(w * w + x * x + y * y + z * z);
}

template <class T>
inline sf::Quaterion<T> sf::Quaterion<T>::normalize() {
	return *this / length();
}

template <class T>
inline sf::Quaterion<T> sf::Quaterion<T>::conjuged() {
	return Quaterion(w, -x, -y, -z);
}

template <class T>
inline sf::Quaterion<T> sf::Quaterion<T>::invert() {
	return conjuged() / (w * w + x * x + y * y + z * z);
}

template <class T>
inline T sf::Quaterion<T>::dot(const Quaterion& a) {
	return w * a.w + x * a.x + y * a.y + z * a.z;
}

template <class T>
inline T sf::Quaterion<T>::length(const Quaterion& a) {
	return sf::sqrt32(a.w * a.w + a.x * a.x + a.y * a.y + a.z * a.z);
}

template <class T>
inline sf::Quaterion<T> sf::Quaterion<T>::normalize(const Quaterion& a) {
	return a / length(a);
}

template <class T>
inline sf::Quaterion<T> sf::Quaterion<T>::conjuged(const Quaterion& a) {
	return Quaterion(a.w, -a.x, -a.y, -a.z);
}

template <class T>
inline sf::Quaterion<T> sf::Quaterion<T>::invert(const Quaterion& a) {
	return conjuged(a) / (a.w * a.w + a.x * a.x + a.y * a.y + a.z * a.z);
}

template <class T>
inline T sf::Quaterion<T>::dot(const Quaterion& a, const Quaterion& b) {
	return b.w * a.w + b.x * a.x + b.y * a.y + b.z * a.z;
}

template <class T>
inline sf::Vector3<T> sf::Quaterion<T>::operator*(const Vector3<T>& a) {
	const Vector3<T> qv(x, y, z);
	const Vector3<T> t = qv.cross(a) * static_cast<T>(2);

	return a + t * w + qv.cross(t);
}

template <class T>
inline sf::Quaterion<T> sf::Quaterion<T>::operator*(const Quaterion& a) {
	return {
		w * a.w - x * a.x - y * a.y - z * a.z,
		w * a.x + x * a.w + y * a.z - z * a.y,
		w * a.y - x * a.z + y * a.w + z * a.x,
		w * a.z + x * a.y - y * a.x + z * a.w
	};
}

template <class T>
inline sf::Quaterion<T> sf::Quaterion<T>::operator*(const T& a) {
	return {
		w * a,
		x * a,
		y * a,
		z * a
	};
}

template <class T>
inline sf::Quaterion<T> sf::Quaterion<T>::operator/(const T& a) {
	return {
		w / a,
		x / a,
		y / a,
		z / a
	}
}

template <class T>
inline sf::Matrix3<T> sf::Quaterion<T>::toMat3() {
	const T xx = x * x, yy = y * y, zz = z * z;
	const T xy = x * y, xz = x * z, yz = y * z;
	const T wx = w * x, wy = w * y, wz = w * z;

	return {
		static_cast<T>(1) - static_cast<T>(2) * (yy + zz), static_cast<T>(2) * (xy - wz), static_cast<T>(2) * (xz + wy),
		static_cast<T>(2) * (xy + wz), static_cast<T>(1) - static_cast<T>(2) * (xx + zz), static_cast<T>(2) * (yz - wx),
		static_cast<T>(2) * (xz - wy), static_cast<T>(2) * (yz + wx), static_cast<T>(1) - static_cast<T>(2) * (xx + yy)
	};
}

template <class T>
inline sf::Quaterion<T> sf::Quaterion<T>::toQuat(const Matrix3<T>& mat) {
	const T w = mat.m[0] + mat.m[4] + mat.m[8];
	const T x = mat.m[0] - mat.m[4] - mat.m[8];
	const T y = mat.m[4] - mat.m[0] - mat.m[8];
	const T z = mat.m[8] - mat.m[0] - mat.m[4];

	uint8_t index = 0;
	T biggest = w;

	if (x > biggest) {
		biggest = x;
		index = 1;
	}
	if (y > biggest) {
		biggest = y;
		index = 2;
	}
	if (z > biggest) {
		biggest = z;
		index = 3;
	}

	T biggestVal = sf::sqrt32(biggest + static_cast<T>(1)) * static_cast<T>(0.5);
	T mul = static_cast<T>(0.25) / biggestVal;

	switch (index) {
	case 0: return Quaterion(biggestVal, (mat.m[5] - mat.m[7]) * mul, (mat.m[6] - mat.m[2]) * mul, (mat.m[1] - mat.m[3]) * mul);
	case 1: return Quaterion((mat.m[5] - mat.m[7]) * mul, biggestVal, (mat.m[1] + mat.m[3]) * mul, (mat.m[6] + mat.m[2]) * mul);
	case 2: return Quaterion((mat.m[6] - mat.m[2]) * mul, (mat.m[1] + mat.m[3]) * mul, biggestVal, (mat.m[5] + mat.m[7]) * mul);
	case 3: return Quaterion((mat.m[1] - mat.m[3]) * mul, (mat.m[6] + mat.m[2]) * mul, (mat.m[5] + mat.m[7]) * mul, biggestVal);
	default: break;
	}

	return Quaterion();
}

template <class T>
inline sf::Quaterion<T> sf::Quaterion<T>::slerp(const Quaterion& a, const Quaterion& b, const T& t) {
	Quaterion q0 = ((Quaterion)a).normalize();
	Quaterion q1 = ((Quaterion)b).normalize();

	T dot = q0.dot(q1);

	if (dot < static_cast<T>(0)) {
		q1 = q1 * static_cast<T>(-1);

		dot = -dot;
	}

	if (dot - static_cast<T>(1) > -SF_G_EPSILON) {
		Quaterion result = q0 * (static_cast<T>(1) - t) + q2 * t;

		return result.normalize();
	}

	const Quaterion q2 = (q1 - q0 * dot).normalize();
	const T theta = std::acos(dot) * t;
	
	return q0 * std::cos(theta) + q2 * std::sin(theta);
}

template <class T>
inline sf::Quaterion<T> sf::Quaterion<T>::lookRotation(const Vector3<T>& forward, const Vector3<T>& up) {
	const Vector3<T> fwd = ((Vector3<T>)forward).normalize();
	const Vector3<T> right = ((Vector3<T>)up).cross(fwd).normalize();
	const Vector3<T> localUp = fwd.cross(right);

	const T m00 = right.x, m01 = localUp.x, m02 = fwd.x;
	const T m10 = right.y, m11 = localUp.y, m12 = fwd.y;
	const T m20 = right.z, m21 = localUp.z, m22 = fwd.z;

	const T trace = m00 + m11 + m22;

	Quaterion q;

	if (trace > static_cast<T>(0)) {
		T s = static_cast<T>(sf::reverseSqrt32(trace + 1.0)) * static_cast<T>(0.5);

		q.w = s * static_cast<T>(4);
		q.x = (m21 - m12) * s;
		q.y = (m02 - m20) * s;
		q.z = (m10 - m01) * s;
	}
	else if (m00 > m11 && m00 > m22) {
		T s = static_cast<T>(sf::reverseSqrt32(1.0 + m00 - m11 - m22)) * static_cast<T>(0.5);

		q.w = (m21 - m12) * s;
		q.x = s * static_cast<T>(4);
		q.y = (m10 + m01) * s;
		q.z = (m02 + m20) * s;
	}
	else if (m00 > m11 && m00 > m22) {
		T s = static_cast<T>(sf::reverseSqrt32(1.0 + m11 - m00 - m22)) * static_cast<T>(0.5);

		q.w = (m02 - m20) * s;
		q.x = (m10 + m01) * s;
		q.y = s * static_cast<T>(4);
		q.z = (m12 + m21) * s;
	}
	else {
		T s = static_cast<T>(sf::reverseSqrt32(1.0 + m22 - m00 - m11)) * static_cast<T>(0.5);

		q.w = (m10 - m01) * s;
		q.x = (m02 + m20) * s;
		q.y = (m12 + m21) * s;
		q.z = s * static_cast<T>(4);
	}

	return q.normalize();
}

template <class T>
inline sf::Quaterion<T> sf::Quaterion<T>::lookAt(const Vector3<T>& forward, const Vector3<T>& up) {
	Matrix3<T> result;

	// Dont negate for left handed coordinates
	result[2] = Vector3<T>::normalize(forward).negate();
	Vector3 right = up.cross(forward).normalize();
	result[0] = right * reverseSqrt32(max(SF_G_EPSILON, right.dot(right)));
	result[1] = result[2].cross(result[0]);

	return toQuat(result);
}

template <class T>
inline sf::Quaterion<T> sf::Quaterion<T>::fromEuler(const T& pitch, const T& yaw, const T& roll) {
	const T halfPitch = pitch * static_cast<T>(0.5);
	const T halfYaw = yaw * static_cast<T>(0.5);
	const T halfRoll = roll * static_cast<T>(0.5);

	const T sp = std::sin(halfPitch);
	const T cp = std::cos(halfPitch);
	const T sy = std::sin(halfYaw);
	const T cy = std::cos(halfYaw);
	const T sr = std::sin(halfRoll);
	const T cr = std::cos(halfRoll);

	Quaterion q;
	q.w = cy * cp * cr + sy * sp * sr;
	q.x = cy * sp * cr + sy * cp * sr;
	q.y = sy * cp * cr - cy * sp * sr;
	q.z = cy * cp * sr - sy * sp * cr;

	return q;
}

template <class T>
inline sf::Vector3<T> sf::Quaterion<T>::toEuler() {
	Vector3<T> angles;

	const T sp = static_cast<T>(2) * (w * x + y * z);
	const T cp = static_cast<T>(1) - static_cast<T>(2) * (x * x + y * y);
	angles.x = std::atan2(sp, cp);

	const T sy = static_cast<T>(2) * (w * y - z * x);

	if (sf::abs(sy) >= static_cast<T>(1))
		angles.y = copysign(static_cast<T>(M_PI / 2.0), sy);
	else
		angles.y = std::asin(sy);

	const T sr = static_cast<T>(2) * (w * z + x * y);
	const T cr = static_cast<T>(1) - static_cast<T>(2) * (y * y + z * z);

	angles.z = std::atan2(sr, cr);

	return angles;
}