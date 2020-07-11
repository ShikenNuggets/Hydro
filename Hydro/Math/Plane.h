#ifndef PLANE_H
#define PLANE_H

#include "Vector.h"

namespace PizzaBox{
	struct Plane{

		Vector3 point;
		float d; //The distance displaced along the normal

		explicit Plane(const float x_ = 0.0f, const float y_ = 0.0f, const float z_ = 0.0f, const float d_ = 0.0f);
		explicit Plane(const Vector3& v_, const float d_ = 0.0f);
		Plane(const Vector3& v0, const Vector3& v1, const Vector3& v2); //This creates a normalized equation of a plane
		~Plane();

		Plane Normalized() const;
		void Normalize();
		static Plane Normalized(const Plane& p_);

		std::string ToString();
	};
}

#endif //!PLANE_H