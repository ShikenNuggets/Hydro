#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <string>

namespace Hydro{
	struct Vector2;
	struct Vector3;
	struct Vector4;

	struct Vector2{
		float x;
		float y;

		explicit Vector2(float x_ = 0.0f, float y_ = 0.0f);

		inline static Vector2 Zero(){ return Vector2(0.0f, 0.0f); }
		inline static Vector2 Fill(float v_){ return Vector2(v_, v_); }

		//Operator overloads for performing various math operations with Vectors
		Vector2 operator -() const;
		Vector2 operator +(const Vector2& v_) const;
		Vector2 operator -(const Vector2& v_) const;
		Vector2 operator *(float s_) const;
		Vector2 operator /(float s_) const;

		inline friend Vector2 operator *(float s_, const Vector2& v_){
			return v_ * s_;
		}

		void operator +=(const Vector2& v_);
		void operator -=(const Vector2& v_);
		void operator *=(float s_);
		void operator /=(float s_);

		float SquaredMagnitude() const;
		static float SquaredMagnitude(const Vector2& v_);
		float Magnitude() const;
		static float Magnitude(const Vector2& v_);
		Vector2 Normalized() const;
		void Normalize();
		static Vector2 Normalized(const Vector2& v_);
		static float Dot(const Vector2& a_, const Vector2& b_);

		std::string ToString() const;

		//These allow us to pass this as an array to legacy code or things like OpenGL very easily
		operator const float* () const;
		operator float* ();
	};

	struct Vector3{
		float x;
		float y;
		float z;

		explicit Vector3(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f);

		inline static Vector3 Zero(){ return Vector3(0.0f, 0.0f, 0.0f); }
		inline static Vector3 Fill(float v_){ return Vector3(v_, v_, v_); }

		//Various operator overloads for performing math operations with Vectors
		Vector3 operator -() const;
		Vector3 operator +(const Vector2& v_) const;
		Vector3 operator +(const Vector3& v_) const;
		Vector3 operator -(const Vector2& v_) const;
		Vector3 operator -(const Vector3& v_) const;
		Vector3 operator *(float s_) const;
		Vector3 operator /(float s_) const;

		inline friend Vector3 operator *(float s_, const Vector3& v_){
			return v_ * s_;
		}

		void operator +=(const Vector2& v_);
		void operator +=(const Vector3& v_);
		void operator -=(const Vector2& v_);
		void operator -=(const Vector3& v_);
		void operator *=(float s_);
		void operator /=(float s_);

		float SquaredMagnitude() const;
		static float SquaredMagnitude(const Vector3& v_);
		float Magnitude() const;
		static float Magnitude(const Vector3& v_);
		Vector3 Normalized() const;
		void Normalize();
		static Vector3 Normalized(const Vector3& v_);
		static float Dot(const Vector3& a, const Vector3& b);
		static Vector3 Cross(const Vector3& a, const Vector3& b);
		static Vector3 Lerp(const Vector3& a_, const Vector3& b_, float t_);

		static Vector3 Rotate(const Vector3& n_, float theta_, const Vector3& v_);
		//Reflect off a normal  
		static Vector3 Reflect(const Vector3& v_, const Vector3& p_);

		std::string ToString() const;

		//These allow us to pass this as an array to legacy code or things like OpenGL very easily
		operator const float* () const;
		operator float* ();
	};

	struct Vector4{
		float x;
		float y;
		float z;
		float w;

		explicit Vector4(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f, float w_ = 0.0f);

		inline static Vector4 Zero(){ return Vector4(0.0f, 0.0f, 0.0f, 0.0f); }
		inline static Vector4 Fill(float v_){ return Vector4(v_, v_, v_, v_); }

		//Various operator overloads for performing math operations with Vectors
		Vector4 operator -() const;
		Vector4 operator +(const Vector2& v_) const;
		Vector4 operator +(const Vector3& v_) const;
		Vector4 operator +(const Vector4& v_) const;
		Vector4 operator -(const Vector2& v_) const;
		Vector4 operator -(const Vector3& v_) const;
		Vector4 operator -(const Vector4& v_) const;
		Vector4 operator *(float s_) const;
		Vector4 operator /(float s_) const;

		inline friend Vector4 operator *(float s_, const Vector4& v_){
			return v_ * s_;
		}

		void operator +=(const Vector2& v_);
		void operator +=(const Vector3& v_);
		void operator +=(const Vector4& v_);
		void operator -=(const Vector2& v_);
		void operator -=(const Vector3& v_);
		void operator -=(const Vector4& v_);
		void operator *=(float s_);
		void operator /=(float s_);

		float SquaredMagnitude() const;
		static float SquaredMagnitude(const Vector4& v_);
		float Magnitude() const;
		static float Magnitude(const Vector4& v_);
		Vector4 Normalized() const;
		void Normalize();
		static Vector4 Normalized(const Vector4& v_);
		static float Dot(const Vector4& a_, const Vector4& b_);

		std::string ToString() const;

		//These allow us to pass this as an array to legacy code or things like OpenGL very easily
		operator const float* () const;
		operator float* ();
	};
}

#endif //!VECTOR_H