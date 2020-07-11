#ifndef QUATERNION_H
#define QUATERNION_H

#include <string>

#include "Matrix.h"

namespace Hydro{
	//Forward Declarations
	struct Euler;

	struct Quaternion{
		float w;
		float x;
		float y;
		float z;

		explicit Quaternion(const float w_ = 1.0f, const float x_ = 0.0f, const float y_ = 0.0f, const float z_ = 0.0f);
		Quaternion(const float w_, const Vector3& v_);
		explicit Quaternion(const Vector4& v_);
		~Quaternion();

		inline static Quaternion Identity(){ return Quaternion(1.0f, 0.0f, 0.0f, 0.0f); }

		Quaternion operator +(const Quaternion& q) const;
		Quaternion operator *(const Quaternion& q) const;
		Quaternion operator *(const float f) const;
		Quaternion operator /(const float f) const;

		void operator +=(const Quaternion& q);
		void operator *=(const Quaternion& q);
		void operator *=(const float f);
		void operator /=(const float f);

		static float Dot(const Quaternion& a, const Quaternion& b);
		static float Magnitude(const Quaternion& q);
		static Quaternion Normalize(const Quaternion& q);
		void Normalize();
		Quaternion Normalized() const;
		Quaternion Inverse() const;

		static Quaternion Rotate(const float angle, const Vector3& axis);
		static float GetRotationAngle(const Quaternion& q);
		static Vector3 GetRotationAxis(const Quaternion& q);
		static Quaternion LookAt(const Vector3& source_, const Vector3& destination_);

		static Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, const float t);
		static Quaternion Slerp(const Quaternion& q1_, const Quaternion& q2_, const float t);

		Matrix3 ToMatrix3() const;
		static Matrix3 ToMatrix3(const Quaternion& q_);
		Matrix4 ToMatrix4() const;
		static Matrix4 ToMatrix4(const Quaternion& q_);
		Euler ToEuler() const;
		static Euler ToEuler(const Quaternion& q_);

		std::string ToString() const;
	};
}

#endif //!QUATERNION_H