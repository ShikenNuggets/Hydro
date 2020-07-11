#ifndef EULER_H
#define EULER_H

#include <string>

namespace Hydro{
	//Forward Declarations
	struct Vector3;
	class Matrix3;
	class Matrix4;
	struct Quaternion;

	struct Euler{
		float x;
		float y;
		float z;

		explicit Euler(const float x_ = 0.0f, const float y_ = 0.0f, const float z_ = 0.0f);
		explicit Euler(const Vector3& v);
		~Euler();

		Euler operator -() const;
		Euler operator +(const Euler& e) const;
		Euler operator -(const Euler& e) const;
		Euler operator *(const float f) const;
		Euler operator /(const float f) const;
		void operator +=(const Euler& e);
		void operator -=(const Euler& e);
		void operator *=(const float f);
		void operator /=(const float f);

		Matrix3 ToMatrix3() const;
		static Matrix3 ToMatrix3(const Euler& e_);
		Matrix4 ToMatrix4() const;
		static Matrix4 ToMatrix4(const Euler& e_);
		Quaternion ToQuaternion() const;
		static Quaternion ToQuaternion(const Euler& e_);

		std::string ToString() const;

		operator const float* () const;
		operator float* ();
	};
}

#endif //!EULER_H