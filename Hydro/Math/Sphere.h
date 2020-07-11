#ifndef SPHERE_H
#define SPHERE_H

#include "Vector.h"

namespace PizzaBox{
	struct Sphere{
		Vector3 point;
		float radius;

		explicit Sphere(const float x_ = 0.0f, const float y_ = 0.0f, const float z_ = 0.0f, const float r_ = 1.0f);
		explicit Sphere(const Vector3& v_, const float r_ = 1.0f);
		~Sphere();

		std::string ToString();
	};
}

#endif //!SPHERE_H