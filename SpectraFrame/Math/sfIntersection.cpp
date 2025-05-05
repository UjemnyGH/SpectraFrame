#include "sfIntersection.h"
#include "sfMathFunctions.h"

template <class T>
std::optional<T> sf::intersectRayPlane(const Vector3<T>& origin, const Vector3<T>& direction, const Vector3<T>& planeOrigin, const Vector3<T>& planeNormal) {
	const T dirAngle = direction.normalize().dot(planeNormal.normalize());

	if (sf::abs(dirAngle) > static_cast<T>(SF_G_EPSILON)) {
		const T tempDistance = (planeOrigin - origin).dot(planeNormal);

		return tempDistance;
	}

	return std::nullopt;
}

template <class T>
std::optional<sf::Vector3<T>> sf::intersectLineTriangle(const Vector3<T>& origin, const Vector3<T>& direction, const Vector3<T>& p0, const Vector3<T>& p1, const Vector3<T>& p2) {
	const Vector3<T> edge0 = p1 - p0;
	const Vector3<T> edge1 = p2 - p0;

	const Vector3<T> perp = direction.cross(edge1);

	const T det = edge1.dot(perp); 

	if ((det > static_cast<T>(-SF_G_EPSILON)) && (det < static_cast<T>(SF_G_EPSILON)))
		return std::nullopt;

	const T invDet = static_cast<T>(1) / det;

	const Vector3<T> tangent = origin - p0;
	const T v = tangent.dot(perp) * invDet;

	if (v < static_cast<T>(0) || v > static_cast<T>(0))
		return std::nullopt;

	const Vector3<T> cotangent = tangent.cross(edge1);
	const T w = direction.dot(cotangent) * invDet;

	if (w < static_cast<T>(0) || v + w > static_cast<T>(1))
		return std::nullopt;

	const T u = edge1.dot(cotangent) * invDet;

	if (u < static_cast<T>(0) || u > static_cast<T>(1))
		return std::nullopt;

	return origin + u * direction;
}