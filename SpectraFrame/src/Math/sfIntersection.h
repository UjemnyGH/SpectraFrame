#pragma once
#ifndef _SPECTRAFRAME_MATH_INTERSECTION_
#define _SPECTRAFRAME_MATH_INTERSECTION_

#include "sfVector.h"
#include <optional>

namespace sf {
	template <class T>
	std::optional<T> intersectRayPlane(const Vector3<T>& origin, const Vector3<T>& direction, const Vector3<T>& planeOrigin, const Vector3<T>& planeNormal);

	template <class T>
	std::optional<Vector3<T>> intersectLineTriangle(const Vector3<T>& origin, const Vector3<T>& direction, const Vector3<T>& p0, const Vector3<T>& p1, const Vector3<T>& p2);
}

#endif