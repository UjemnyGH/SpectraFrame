#include "sfMatrix.h"
#include <cmath>
#include <algorithm>

// Matrix3

template <class T>
sf::Matrix3<T>::Matrix3() { fill(static_cast<T>(0)); }

template <class T>
sf::Matrix3<T>::Matrix3(T init) { fill(init); }

template <class T>
sf::Matrix3<T>::Matrix3(const Vector3<T> r0, const Vector3<T> r1, const Vector3<T> r2) {
	rows[0] = r0;
	rows[1] = r1;
	rows[2] = r2;
}

template <class T>
template <class TN>
sf::Matrix3<T>::Matrix3(const Matrix3<TN>& newMat) {
	for (int i = 0; i < 3; i++) {
		rows[i] = newMat[i];
	}
}

template <class T>
sf::Matrix3<T>::Matrix3(const Matrix4<T>& newMat) {
	rows[0] = Vector3<T>(newMat[0]);
	rows[1] = Vector3<T>(newMat[1]);
	rows[2] = Vector3<T>(newMat[2]);
}

template <class T>
inline sf::Vector3<T>& sf::Matrix3<T>::operator[](int col) {
	return rows[col];
}

template <class T>
void sf::Matrix3<T>::fill(const T& value) {
	for (int i = 0; i < 3; i++) {
		rows[i] = Vector3(value);
	}
}

template <class T>
inline sf::Matrix3<T> sf::Matrix3<T>::zero() {
	return Matrix3(
		Vector3(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0)),
		Vector3(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0)),
		Vector3(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0))
	);
}

template <class T>
inline sf::Matrix3<T> sf::Matrix3<T>::one() {
	return Matrix3(
		Vector3(static_cast<T>(1), static_cast<T>(1), static_cast<T>(1)),
		Vector3(static_cast<T>(1), static_cast<T>(1), static_cast<T>(1)),
		Vector3(static_cast<T>(1), static_cast<T>(1), static_cast<T>(1))
	);
}

template <class T>
inline sf::Matrix3<T> sf::Matrix3<T>::identity() {
	return Matrix3(
		Vector3(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0)),
		Vector3(static_cast<T>(0), static_cast<T>(1), static_cast<T>(0)),
		Vector3(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1))
	);
}

template <class T>
inline sf::Matrix3<T> sf::Matrix3<T>::rotateX(const T& radians) {
	return Matrix3(
		Vector3(static_cast<T>(1), static_cast<T>(0),					static_cast<T>(0)),
		Vector3(static_cast<T>(0), static_cast<T>(std::cos(radians)),	static_cast<T>(-std::sin(radians))),
		Vector3(static_cast<T>(0), static_cast<T>(std::sin(radians)),	static_cast<T>(std::cos(radians))),
	);
}

template <class T>
inline sf::Matrix3<T> sf::Matrix3<T>::rotateY(const T& radians) {
	return Matrix3(
		Vector3(static_cast<T>(std::cos(radians)),	static_cast<T>(0), static_cast<T>(std::sin(radians))),
		Vector3(static_cast<T>(0),					static_cast<T>(1), static_cast<T>(0)),
		Vector3(static_cast<T>(-std::sin(radians)), static_cast<T>(0), static_cast<T>(std::cos(radians)))
	);
}

template <class T>
inline sf::Matrix3<T> sf::Matrix3<T>::rotateZ(const T& radians) {
	return Matrix3(
		Vector3(static_cast<T>(std::cos(radians)),	static_cast<T>(-std::sin(radians)), static_cast<T>(0)),
		Vector3(static_cast<T>(std::sin(radians)),	static_cast<T>(std::cos(radians)),	static_cast<T>(0)),
		Vector3(static_cast<T>(0),					static_cast<T>(0),					static_cast<T>(1)),
	);
}

template <class T>
inline sf::Matrix3<T> sf::Matrix3<T>::scale(const Vector3<T>& v) {
	return Matrix3(
		Vector3(v.x,				static_cast<T>(0),	static_cast<T>(0)),
		Vector3(static_cast<T>(0),	v.y,				static_cast<T>(0)),
		Vector3(static_cast<T>(0),	static_cast<T>(0),	v.w)
	);
}

template <class T>
inline sf::Vector3<T> sf::Matrix3<T>::getXAxis() {
	return Vector3(rows[0][0], rows[1][0], rows[2][0]);
}

template <class T>
inline sf::Vector3<T> sf::Matrix3<T>::getYAxis() {
	return Vector3(rows[0][1], rows[1][1], rows[2][1]);
}

template <class T>
inline sf::Vector3<T> sf::Matrix3<T>::getZAxis() {
	return Vector3(rows[0][2], rows[1][2], rows[2][2]);
}

template <class T>
inline sf::Vector3<T> sf::Matrix3<T>::getScale() {
	return Vector3(rows[0][0], rows[1][1], rows[2][2]);
}

template <class T>
inline sf::Matrix3<T> sf::Matrix3<T>::transpose() {
	return Matrix3(
		Vector3(rows[0][0], rows[1][0], rows[2][0]),
		Vector3(rows[0][1], rows[1][1], rows[2][1]),
		Vector3(rows[0][2], rows[1][2], rows[2][2])
	);
}

template <class T>
inline sf::Matrix3<T> sf::Matrix3<T>::operator+(const Matrix3& a) {
	return Matrix3(
		Vector3(rows[0][0] + a[0][0], rows[0][1] + a[0][1], rows[0][2] + a[0][2]),
		Vector3(rows[1][0] + a[1][0], rows[1][1] + a[1][1], rows[1][2] + a[1][2]),
		Vector3(rows[2][0] + a[2][0], rows[2][1] + a[2][1], rows[2][2] + a[2][2])
	);
}

template <class T>
inline sf::Matrix3<T> sf::Matrix3<T>::operator-(const Matrix3& a) {
	return Matrix3(
		Vector3(rows[0][0] - a[0][0], rows[0][1] - a[0][1], rows[0][2] - a[0][2]),
		Vector3(rows[1][0] - a[1][0], rows[1][1] - a[1][1], rows[1][2] - a[1][2]),
		Vector3(rows[2][0] - a[2][0], rows[2][1] - a[2][1], rows[2][2] - a[2][2])
	);
}

template <class T>
inline sf::Matrix3<T> sf::Matrix3<T>::operator+=(const Matrix3& a) {
	*this = operator+(a);

	return *this;
}

template <class T>
inline sf::Matrix3<T> sf::Matrix3<T>::operator-=(const Matrix3& a) {
	*this = operator-(a);

	return *this;
}

template <class T>
inline sf::Matrix3<T> sf::Matrix3<T>::operator*=(const T& a) {
	return (*this = Matrix3(
		Vector3(rows[0][0] * a, rows[0][1] * a, rows[0][2] * a),
		Vector3(rows[1][0] * a, rows[1][1] * a, rows[1][2] * a),
		Vector3(rows[2][0] * a, rows[2][1] * a, rows[2][2] * a)
	));
}

template <class T>
inline sf::Matrix3<T> sf::Matrix3<T>::operator/=(const T& a) {
	*this = operator*=(static_cast<T>(1) / a);

	return *this;
}

template <class T>
inline sf::Matrix3<T> sf::Matrix3<T>::operator*(const Matrix3& a) {
	return Matrix3(
		Vector3(getXAxis().dot(a[0]), getYAxis().dot(a[0]), getZAxis().dot(a[0])),
		Vector3(getXAxis().dot(a[1]), getYAxis().dot(a[1]), getZAxis().dot(a[1])),
		Vector3(getXAxis().dot(a[2]), getYAxis().dot(a[2]), getZAxis().dot(a[2]))
	);
}

template <class T>
inline sf::Matrix3<T> sf::Matrix3<T>::operator*=(const Matrix3& a) {
	*this = operator*(a);

	return *this;
}

template <class T>
inline sf::Vector3<T> sf::Matrix3<T>::operator*(const Vector3<T>& a) {
	return Vector3(
		rows[0].dot(a),
		rows[1].dot(a),
		rows[2].dot(a)
	);
}

template <class T>
inline sf::Vector3<T> sf::Matrix3<T>::vectorizePoint(const Vector3<T>& a) {
	return Vector3(
		rows[0].dot(a),
		rows[1].dot(a),
		rows[2].dot(a)
	);
}

template <class T>
inline sf::Vector3<T> sf::Matrix3<T>::rightMultiply(const Vector3<T>& a) {
	Vector3<T> x = getXAxis() * a.x;
	Vector3<T> y = getYAxis() * a.y;
	Vector3<T> z = getZAxis() * a.z;

	return Vector3(
		x.x + x.y + x.z,
		y.x + y.y + y.z,
		z.x + z.y + z.z,
	);
}

template <class T>
inline sf::Vector3<T> sf::Matrix3<T>::leftMultiply(const Vector3<T>& a) {
	Vector3<T> x = rows[0] * a.x;
	Vector3<T> y = rows[1] * a.y;
	Vector3<T> z = rows[2] * a.z;

	return Vector3(
		x.x + x.y + x.z,
		y.x + y.y + y.z,
		z.x + z.y + z.z,
	);
}

template <class T>
inline sf::Vector3<T> sf::Matrix3<T>::direction(const Vector3<T>& a) {
	return Vector3(
		rows[0][0] * a.x + rows[0][1] * a.y + rows[0][2] * a.z,
		rows[1][0] * a.x + rows[1][1] * a.y + rows[1][2] * a.z,
		rows[2][0] * a.x + rows[2][1] * a.y + rows[2][2] * a.z,
	);
}

template <class T>
inline T sf::Matrix3<T>::minor(int row, int col) {
	// submatrix
	T sm[4];

	int index = 0;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i != row && j != col)
				sm[index++] = rows[i][j];
		}
	}

	return sm[0] * sm[2] - sm[1] * sm[3];
}

template <class T>
inline sf::Matrix3<T> sf::Matrix3<T>::cofactorMatrix() {
	Matrix3 cofactor;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			T minorValue = minor(i, j);
			cofactor[i][j] = ((i + j) % 2 == 0 : static_cast<T>(1) : static_cast<T>(-1)) * minorValue;
		}
	}

	return cofactor;
}

template <class T>
inline sf::Matrix3<T> sf::Matrix3<T>::inverse() {
	Matrix3 adj = cofactorMatrix().transpose();

	const T det = rows[0][0] * adj[0][0] + rows[0][1] * adj[1][0] + rows[0][2] * adj[2][0];

	adj /= det;

	return adj;
}

template <class T>
inline T* sf::Matrix3<T>::ptr() {
	return &rows[0][0];
}

// Matrix4

template <class T>
sf::Matrix4<T>::Matrix4() { fill(static_cast<T>(0)); }

template <class T>
sf::Matrix4<T>::Matrix4(T init) { fill(init); }

template <class T>
sf::Matrix4<T>::Matrix4(const Vector4<T>& r0, const Vector4<T>& r1, const Vector4<T>& r2, const Vector4<T>& r3) {
	rows[0] = r0;
	rows[1] = r1;
	rows[2] = r2;
	rows[3] = r3;
}

template <class T>
template <class TN>
sf::Matrix4<T>::Matrix4(const Matrix4<TN>& newMat) {
	for (int i = 0; i < 4; i++) {
		rows[i] = newMat[i];
	}
}

template <class T>
sf::Matrix4<T>::Matrix4(const Matrix3<T>& newMat) {
	rows[0] = Vector4<T>(newMat[0]);
	rows[1] = Vector4<T>(newMat[1]);
	rows[2] = Vector4<T>(newMat[2]);
	rows[3] = Vector4<T>(0, 0, 0, 1);
}

template <class T>
inline sf::Vector4<T>& sf::Matrix4<T>::operator[](int col) {
	return rows[col];
}

template <class T>
void sf::Matrix4<T>::fill(const T& value) {
	for (int i = 0; i < 4; i++) {
		rows[i] = Vector4(value);
	}
}

template <class T>
inline sf::Matrix4<T> sf::Matrix4<T>::zero() {
	return Matrix4(
		Vector4(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0)),
		Vector4(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0)),
		Vector4(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0)),
		Vector4(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0))
	);
}

template <class T>
inline sf::Matrix4<T> sf::Matrix4<T>::one() {
	return Matrix4(
		Vector4(static_cast<T>(1), static_cast<T>(1), static_cast<T>(1), static_cast<T>(1)),
		Vector4(static_cast<T>(1), static_cast<T>(1), static_cast<T>(1), static_cast<T>(1)),
		Vector4(static_cast<T>(1), static_cast<T>(1), static_cast<T>(1), static_cast<T>(1)),
		Vector4(static_cast<T>(1), static_cast<T>(1), static_cast<T>(1), static_cast<T>(1))
	);
}

template <class T>
inline sf::Matrix4<T> sf::Matrix4<T>::identity() {
	return Matrix4(
		Vector4(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0)),
		Vector4(static_cast<T>(0), static_cast<T>(1), static_cast<T>(0), static_cast<T>(0)),
		Vector4(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), static_cast<T>(0)),
		Vector4(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1))
	);
}

template <class T>
inline sf::Matrix4<T> sf::Matrix4<T>::rotateX(const T& radians) {
	return Matrix4(
		Vector4(static_cast<T>(1), static_cast<T>(0),					static_cast<T>(0),						static_cast<T>(0)),
		Vector4(static_cast<T>(0), static_cast<T>(std::cos(radians)),	static_cast<T>(-std::sin(radians)),		static_cast<T>(0)),
		Vector4(static_cast<T>(0), static_cast<T>(std::sin(radians)),	static_cast<T>(std::cos(radians)),		static_cast<T>(0)),
		Vector4(static_cast<T>(0), static_cast<T>(0),					static_cast<T>(0),						static_cast<T>(1))
	);
}

template <class T>
inline sf::Matrix4<T> sf::Matrix4<T>::rotateY(const T& radians) {
	return Matrix4(
		Vector4(static_cast<T>(std::cos(radians)),		static_cast<T>(0), static_cast<T>(std::sin(radians)),	static_cast<T>(0)),
		Vector4(static_cast<T>(0),						static_cast<T>(1), static_cast<T>(0),					static_cast<T>(0)),
		Vector4(static_cast<T>(-std::sin(radians)),		static_cast<T>(0), static_cast<T>(std::cos(radians)),	static_cast<T>(0)),
		Vector4(static_cast<T>(0),						static_cast<T>(0), static_cast<T>(0),					static_cast<T>(1))
	);
}

template <class T>
inline sf::Matrix4<T> sf::Matrix4<T>::rotateZ(const T& radians) {
	return Matrix4(
		Vector4(static_cast<T>(std::cos(radians)),	static_cast<T>(-std::sin(radians)), static_cast<T>(0), static_cast<T>(0)),
		Vector4(static_cast<T>(std::sin(radians)),	static_cast<T>(std::cos(radians)),	static_cast<T>(0), static_cast<T>(0)),
		Vector4(static_cast<T>(0),					static_cast<T>(0),					static_cast<T>(1), static_cast<T>(0)),
		Vector4(static_cast<T>(0),					static_cast<T>(0),					static_cast<T>(0), static_cast<T>(1))
	);
}

template <class T>
inline sf::Matrix4<T> sf::Matrix4<T>::translate(const Vector3<T>& v) {
	return Matrix4(
		Vector4(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), static_cast<T>(v.x)),
		Vector4(static_cast<T>(0), static_cast<T>(1), static_cast<T>(0), static_cast<T>(v.y)),
		Vector4(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), static_cast<T>(v.z)),
		Vector4(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1))
	);
}

template <class T>
inline sf::Matrix4<T> sf::Matrix4<T>::scale(const Vector3<T>& v) {
	return Matrix4(
		Vector4(static_cast<T>(v.x),	static_cast<T>(0),		static_cast<T>(0),		static_cast<T>(0)),
		Vector4(static_cast<T>(0),		static_cast<T>(v.y),	static_cast<T>(0),		static_cast<T>(0)),
		Vector4(static_cast<T>(0),		static_cast<T>(0),		static_cast<T>(v.z),	static_cast<T>(0)),
		Vector4(static_cast<T>(0),		static_cast<T>(0),		static_cast<T>(0),		static_cast<T>(1))
	);
}

template <class T>
inline sf::Vector4<T> sf::Matrix4<T>::getXAxis() {
	return Vector4(rows[0][0], rows[1][0], rows[2][0], rows[3][0]);
}

template <class T>
inline sf::Vector4<T> sf::Matrix4<T>::getYAxis() {
	return Vector4(rows[0][1], rows[1][1], rows[2][1], rows[3][1]);
}

template <class T>
inline sf::Vector4<T> sf::Matrix4<T>::getZAxis() {
	return Vector4(rows[0][2], rows[1][2], rows[2][2], rows[3][2]);
}

template <class T>
inline sf::Vector4<T> sf::Matrix4<T>::getWAxis() {
	return Vector4(rows[0][3], rows[1][3], rows[2][3], rows[3][3]);
}

template <class T>
inline sf::Vector4<T> sf::Matrix4<T>::getTranslation() {
	return getWAxis();
}

template <class T>
inline sf::Vector4<T> sf::Matrix4<T>::getScale() {
	return Vector4(rows[0][0], rows[1][1], rows[2][2], rows[3][3]);
}

template <class T>
inline sf::Matrix4<T> sf::Matrix4<T>::transpose() {
	return Matrix4(
		Vector4(rows[0][0], rows[1][0], rows[2][0], rows[3][0]),
		Vector4(rows[0][1], rows[1][1], rows[2][1], rows[3][1]),
		Vector4(rows[0][2], rows[1][2], rows[2][2], rows[3][2]),
		Vector4(rows[0][3], rows[1][3], rows[2][3], rows[3][3])
	);
}

template <class T>
inline sf::Matrix4<T> sf::Matrix4<T>::operator+(const Matrix4& a) {
	return Matrix4(
		Vector4(rows[0][0] + a[0][0], rows[0][1] + a[0][1], rows[0][2] + a[0][2], rows[0][3] + a[0][3]),
		Vector4(rows[1][0] + a[1][0], rows[1][1] + a[1][1], rows[1][2] + a[1][2], rows[1][3] + a[1][3]),
		Vector4(rows[2][0] + a[2][0], rows[2][1] + a[2][1], rows[2][2] + a[2][2], rows[2][3] + a[2][3]),
		Vector4(rows[3][0] + a[3][0], rows[3][1] + a[3][1], rows[3][2] + a[3][2], rows[3][3] + a[3][3])
	);
}

template <class T>
inline sf::Matrix4<T> sf::Matrix4<T>::operator-(const Matrix4& a) {
	return Matrix4(
		Vector4(rows[0][0] - a[0][0], rows[0][1] - a[0][1], rows[0][2] - a[0][2], rows[0][3] - a[0][3]),
		Vector4(rows[1][0] - a[1][0], rows[1][1] - a[1][1], rows[1][2] - a[1][2], rows[1][3] - a[1][3]),
		Vector4(rows[2][0] - a[2][0], rows[2][1] - a[2][1], rows[2][2] - a[2][2], rows[2][3] - a[2][3]),
		Vector4(rows[3][0] - a[3][0], rows[3][1] - a[3][1], rows[3][2] - a[3][2], rows[3][3] - a[3][3])
	);
}

template <class T>
inline sf::Matrix4<T> sf::Matrix4<T>::operator+=(const Matrix4& a) {
	*this = operator+(a);

	return *this;
}

template <class T>
inline sf::Matrix4<T> sf::Matrix4<T>::operator-=(const Matrix4& a) {
	*this = operator-(a);

	return *this;
}

template <class T>
inline sf::Matrix4<T> sf::Matrix4<T>::operator*=(const T& a) {
	return (*this = Matrix4(
		Vector4(m[0][0] * a, m[0][1] * a, m[0][2] * a, m[0][3] * a),
		Vector4(m[1][0] * a, m[1][1] * a, m[1][2] * a, m[1][3] * a),
		Vector4(m[2][0] * a, m[2][1] * a, m[2][2] * a, m[2][3] * a),
		Vector4(m[3][0] * a, m[3][1] * a, m[3][2] * a, m[3][3] * a)
	));
}

template <class T>
inline sf::Matrix4<T> sf::Matrix4<T>::operator/=(const T& a) {
	*this = operator*=(static_cast<T>(1) / a);

	return *this;
}

template <class T>
inline sf::Matrix4<T> sf::Matrix4<T>::operator*(const Matrix4& a) {
	return {
		getXAxis().dot(a[0]), getYAxis().dot(a[0]), getZAxis().dot(a[0]), getWAxis().dot(a[0]),
		getXAxis().dot(a[1]), getYAxis().dot(a[1]), getZAxis().dot(a[1]), getWAxis().dot(a[1]),
		getXAxis().dot(a[2]), getYAxis().dot(a[2]), getZAxis().dot(a[2]), getWAxis().dot(a[2]),
		getXAxis().dot(a[3]), getYAxis().dot(a[3]), getZAxis().dot(a[3]), getWAxis().dot(a[3])
 	};
}

template <class T>
inline sf::Matrix4<T> sf::Matrix4<T>::operator*=(const Matrix4& a) {
	*this = operator*(a);

	return *this;
}

template <class T>
inline sf::Vector4<T> sf::Matrix4<T>::operator*(const Vector4<T>& a) {
	return Vector4<T> {
		rows[0].dot(a),
		rows[1].dot(a),
		rows[2].dot(a),
		rows[3].dot(a)
	};
}

template <class T>
inline sf::Vector4<T> sf::Matrix4<T>::toPoint(const Vector4<T>& a) {
	Vector4<T> p(
		rows[0][0] * a.x + rows[0][1] * a.y + rows[0][2] * a.z,
		rows[1][0] * a.x + rows[1][1] * a.y + rows[1][2] * a.z,
		rows[2][0] * a.x + rows[2][1] * a.y + rows[2][2] * a.z,
		static_cast<T>(1)
	);

	T w = rows[3][0] * a.x + rows[3][1] * a.y + rows[3][2] * a.z + rows[3][3];

	return p / w;
}

template <class T>
inline sf::Vector4<T> sf::Matrix4<T>::vectorizePoint(const Vector4<T>& a) {
	return Vector4<T>(rows[0].dot(a), rows[1].dot(a), rows[2].dot(a), rows[3].dot(a));
}

template <class T>
inline sf::Vector4<T> sf::Matrix4<T>::rightMultiply(const Vector4<T>& a) {
	Vector4<T> x = getXAxis() * a.x;
	Vector4<T> y = getYAxis() * a.y;
	Vector4<T> z = getZAxis() * a.z;
	Vector4<T> w = getWAxis() * a.w;

	return Vector4<T>(
		x.x + x.y + x.z + x.w,
		y.x + y.y + y.z + y.w,
		z.x + z.y + z.z + z.w,
		w.x + w.y + w.z + w.w
	);
}

template <class T>
inline sf::Vector4<T> sf::Matrix4<T>::leftMultiply(const Vector4<T>& a) {
	Vector4<T> x = rows[0] * a.x;
	Vector4<T> y = rows[1] * a.y;
	Vector4<T> z = rows[2] * a.z;
	Vector4<T> w = rows[3] * a.w;

	return Vector4<T>(
		x.x + x.y + x.z + x.w,
		y.x + y.y + y.z + y.w,
		z.x + z.y + z.z + z.w,
		w.x + w.y + w.z + w.w
	);
}

template <class T>
inline sf::Vector4<T> sf::Matrix4<T>::direction(const Vector4<T>& a) {
	return Vector4<T>(
		rows[0][0] * a.x + rows[0][1] * a.y + rows[0][2] * a.z,
		rows[1][0] * a.x + rows[1][1] * a.y + rows[1][2] * a.z,
		rows[2][0] * a.x + rows[2][1] * a.y + rows[2][2] * a.z,
		static_cast<T>(1)
	);
}

template <class T>
inline T sf::Matrix4<T>::minor(int row, int col) {
	// submatrix
	T sm[9];

	int index = 0;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i != row && j != col) {
				sm[index++] = rows[i][j];
			}
		}
	}

	return sm[0] * (sm[4] * sm[8] - sm[5] * sm[7]) - sm[1] * (sm[3] * sm[8] - sm[5] * sm[6]) + sm[2] * (sm[3] * sm[7] - sm[4] * sm[6]);
}

template <class T>
inline sf::Matrix4<T> sf::Matrix4<T>::cofactorMatrix() {
	Matrix4 cofactor;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			T minorValue = minor(i, j);
			cofactor[i][j] = ((i + j) % 2 == 0 : static_cast<T>(1) : static_cast<T>(-1)) * minorValue;
		}
	}

	return cofactor;
}

template <class T>
inline sf::Matrix4<T> sf::Matrix4<T>::inverse() {
	Matrix4 adj = cofactorMatrix().transpose();

	const T det = rows[0][0] * adj[0][0] + rows[0][1] * adj[1][0] + rows[0][2] * adj[2][0] + rows[0][3] * adj[3][0];

	adj /= det;

	return adj;
}

template <class T>
inline sf::Matrix4<T> sf::Matrix4<T>::perspectiveFOV(const T& fovRadians, const T& width, const T& height, const T& zNear, const T& zFar) {
	const T fov = static_cast<T>(1) / std::tan(fovRadians / static_cast<T>(2));
	const T zAxis = static_cast<T>(1) / (zFar - zNear);
	
	return Matrix4(
		Vector4(fov * (height / width), static_cast<T>(0),	static_cast<T>(0),							static_cast<T>(0)),
		Vector4(static_cast<T>(0),		f,					static_cast<T>(0),							static_cast<T>(0)),
		Vector4(static_cast<T>(0),		static_cast<T>(0),	-(zFar + zNear) * zAxis,					-static_cast<T>(1)),
		Vector4(static_cast<T>(0),		static_cast<T>(0),	-(static_cast<T>(2) * zFar * zNear) * zAxis, static_cast<T>(1))
	);
}

template <class T>
inline sf::Matrix4<T> sf::Matrix4<T>::perspective(const T& right, const T& left, const T& top, const T& bottom, const T& zNear, const T& zFar) {
	const T xAxis = static_cast<T>(1) / (right - left);
	const T yAxis = static_cast<T>(1) / (top - bottom);
	const T zAxis = static_cast<T>(1) / (zFar - zNear);

	return Matrix4(
		Vector4((static_cast<T>(2) * zNear) * xAxis,	static_cast<T>(0),						(right + left) * xAxis,							static_cast<T>(0)),
		Vector4(static_cast<T>(0),						(static_cast<T>(2) * zNear) * yAxis,	(top + bottom) * yAxis,							static_cast<T>(0)),
		Vector4(static_cast<T>(0),						static_cast<T>(0),						-(zFar + zNear) * zAxis,						-static_cast<T>(1)),
		Vector4(static_cast<T>(0),						static_cast<T>(0),						-(static_cast<T>(2) * zFar * zNear) * zAxis,	static_cast<T>(0))
	);
}

template <class T>
inline sf::Matrix4<T> sf::Matrix4<T>::perspectiveSymmertical(const T& right, const T& top, const T& zNear, const T& zFar) {
	const T zAxis = static_cast<T>(1) / (zFar - zNear);

	return Matrix4(
		Vector4(zNear / right,		static_cast<T>(0),	static_cast<T>(0),								static_cast<T>(0)),
		Vector4(static_cast<T>(0),	zNear / top,		static_cast<T>(0),								static_cast<T>(0)),
		Vector4(static_cast<T>(0),	static_cast<T>(0),	-(zFar + zNear) * zAxis,						static_cast<T>(1)),
		Vector4(static_cast<T>(0),	static_cast<T>(0),	-(static_cast<T>(2) * zFar * zNear) * zAxis,	static_cast<T>(0))
	);
}

template <class T>
inline sf::Matrix4<T> sf::Matrix4<T>::orthographic(const T& right, const T& left, const T& top, const T& bottom, const T& zNear, const T& zFar) {
	const T xAxis = static_cast<T>(1) / (right - left);
	const T yAxis = static_cast<T>(1) / (top - bottom);
	const T zAxis = static_cast<T>(1) / (zFar - zNear);

	return Matrix4(
		Vector4(static_cast<T>(2) * xAxis,	static_cast<T>(0),			static_cast<T>(0),			-(right + left) * xAxis),
		Vector4(static_cast<T>(0),			static_cast<T>(2) * yAxis,	static_cast<T>(0),			-(top + bottom) * yAxis),
		Vector4(static_cast<T>(0),			static_cast<T>(0),			-static_cast<T>(2) * zAxis,	-(zFar + zNear) * zAxis),
		Vector4(static_cast<T>(0),			static_cast<T>(0),			static_cast<T>(0),			static_cast<T>(1))
	);
}

template <class T>
inline sf::Matrix4<T> sf::Matrix4<T>::orthographicSymmertical(const T& right, const T& top, const T& zNear, const T& zFar) {
	const T zAxis = static_cast<T>(1) / (zFar - zNear);

	return Matrix4(
		Vector4(static_cast<T>(1) / right,	static_cast<T>(0),			static_cast<T>(0),			static_cast<T>(0)),
		Vector4(static_cast<T>(0),			static_cast<T>(1) / top,	static_cast<T>(0),			static_cast<T>(0)),
		Vector4(static_cast<T>(0),			static_cast<T>(0),			-static_cast<T>(2) * zAxis, -(zFar + zNear) * zAxis),
		Vector4(static_cast<T>(0),			static_cast<T>(0),			static_cast<T>(0),			static_cast<T>(1))
	);
}

template <class T>
inline sf::Matrix4<T> sf::Matrix4<T>::lookAt(const Vector3<T>& eye, const Vector3<T>& at, const Vector3<T>& up) {
	Vector3<T> zAxis = (at - eye).normalize();
	Vector3<T> xAxis = up.cross(zAxis).normalize();
	Vector3<T> yAxis = zAxis.cross(xAxis);

	return Matrix4(
		Vector4(xAxis.x,			yAxis.x,			zAxis.x,			static_cast<T>(0)),
		Vector4(xAxis.y,			yAxis.y,			zAxis.y,			static_cast<T>(0)),
		Vector4(xAxis.z,			yAxis.z,			zAxis.z,			static_cast<T>(0)),
		Vector4(-xAxis.dot(eye),	-yAxis.dot(eye),	-zAxis.dot(eye),	static_cast<T>(1))
	);
}

template <class T>
inline T* sf::Matrix4<T>::ptr() {
	return &rows[0][0];
}