#ifndef MATRIX_H
#define MATRIX_H

#include "Euler.h"
#include "Vector.h"

namespace Hydro{
	class Matrix2;
	class Matrix3;
	class Matrix4;
	struct Quaternion;

	//Column Major Order
	//[0][2]
	//[1][3]
	class Matrix2{
	public:
		Matrix2();
		Matrix2(const float x1, const float x2, const float y1, const float y2);
		explicit Matrix2(const float fillValue);
		explicit Matrix2(const Matrix3& m_);
		explicit Matrix2(const Matrix4& m_);
		~Matrix2();

		const float operator [](const int i) const; //This allows us to get elements
		float& operator [](const int i); //This allows us to set elements

		Matrix2 operator +(const Matrix2& m_) const;
		Matrix2 operator -(const Matrix2& m_) const;
		Matrix2 operator *(const float s_) const;
		Matrix2 operator *(const Matrix2& m_) const;
		Matrix2 operator /(const float s) const;
		void operator +=(const Matrix2& m_);
		void operator -=(const Matrix2& m_);
		void operator *=(const float s);
		void operator *=(const Matrix2& m_);
		void operator /=(const float s);

		operator float*();
		operator const float*() const;

		static Matrix2 Identity();
		Matrix2 Transpose() const;
		static Matrix2 Transpose(const Matrix2& m_);
		Matrix2 Inverse() const;
		static Matrix2 Inverse(const Matrix2& m_);

		Matrix3 ToMatrix3() const;
		static Matrix3 ToMatrix3(const Matrix2& m_);
		Matrix4 ToMatrix4() const;
		static Matrix4 ToMatrix4(const Matrix2& m_);

		std::string ToString() const;
	private:
		float m[4];
	};

	//Column Major Order
	//[0][3][6]
	//[1][4][7]
	//[2][5][8]
	class Matrix3{
	public:
		Matrix3();
		Matrix3(const float x1, const float x2, const float x3,
			const float y1, const float y2, const float y3,
			const float z1, const float z2, const float z3);
		explicit Matrix3(const float fillValue);
		explicit Matrix3(const Matrix2& m_);
		explicit Matrix3(const Matrix4& m_);
		~Matrix3();

		Matrix3& operator =(const Matrix4& m_);

		const float operator [](const int i) const; //This allows us to get elements
		float& operator [](const int i); //This allows us to set elements

		Matrix3 operator +(const Matrix3& m_) const;
		Matrix3 operator -(const Matrix3& m_) const;
		Matrix3 operator *(const float s) const;
		Matrix3 operator *(const Matrix3& m_) const;
		Matrix3 operator /(const float s) const;
		void operator +=(const Matrix3& m_);
		void operator -=(const Matrix3& m_);
		void operator *=(const float s);
		void operator *=(const Matrix3& m_);
		void operator /=(const float s);

		operator float*();
		operator const float*() const;

		static Matrix3 Identity();
		Matrix3 Transpose() const;
		static Matrix3 Transpose(const Matrix3& m_);
		Matrix3 Inverse() const;
		static Matrix3 Inverse(const Matrix3& m_);

		Matrix2 ToMatrix2() const;
		static Matrix2 ToMatrix2(const Matrix3& m_);
		Matrix4 ToMatrix4() const;
		static Matrix4 ToMatrix4(const Matrix3& m_);
		Euler ToEuler() const;
		static Euler ToEuler(const Matrix3& m_);
		Quaternion ToQuaternion() const;
		static Quaternion ToQuaternion(const Matrix3& m_);

		std::string ToString() const;
	private:
		float m[9];
	};

	//Column Major Order
	//[00][04][08][12]
	//[01][05][09][13]
	//[02][06][10][14]
	//[03][07][11][15]
	class Matrix4{
	public:
		Matrix4();
		Matrix4(const float x1, const float x2, const float x3, const float x4,
			const float y1, const float y2, const float y3, const float y4,
			const float z1, const float z2, const float z3, const float z4,
			const float w1, const float w2, const float w3, const float w4);
		explicit Matrix4(const float fillValue);
		explicit Matrix4(const Matrix2& m_);
		explicit Matrix4(const Matrix3& m_);
		~Matrix4();

		const float operator [](const int i) const; //This allows us to get elements
		float& operator [](const int i); //This allows us to set elements

		Matrix4 operator +(const Matrix4& m_) const;
		Matrix4 operator -(const Matrix4& m_) const;
		Matrix4 operator *(const float s) const;
		Matrix4 operator *(const Matrix4& m_) const;
		Vector3 operator *(const Vector3& v) const;
		Vector4 operator *(const Vector4& v) const;
		Matrix4 operator /(const float s) const;
		void operator +=(const Matrix4& m_);
		void operator -=(const Matrix4& m_);
		void operator *=(const float s);
		void operator *=(const Matrix4& m_);
		void operator /=(const float s);

		operator float*();
		operator const float*() const;

		static Matrix4 Identity();
		Matrix4 Transpose() const;
		static Matrix4 Transpose(const Matrix4& m_);
		Matrix4 Inverse() const;
		static Matrix4 Inverse(const Matrix4& m_);

		static Matrix4 Translate(const Vector3& v_);
		static Matrix4 Rotate(const float angle_, const Vector3& v_);
		static Matrix4 Scale(const Vector3& v_);

		Vector3 GetTranslation();
		Quaternion GetRotation();
		Vector3 GetScale();

		static Matrix4 LookAt(float eyeX, float eyeY, float eyeZ, float atX, float atY, float atZ, float upX, float upY, float upZ);
		static Matrix4 LookAt(const Vector3& eye, const Vector3& at, const Vector3& up);

		static Matrix4 Orthographic(const float left_, const float right_, const float bottom_, const float top_);
		static Matrix4 Orthographic(const float left_, const float right_, const float bottom_, const float top_, const float near_, const float far_);
		static Matrix4 UnOrtho(const Matrix4& ortho);
		static Matrix4 Perspective(const float fov_, const float aspect_, const float nearPlane_, const float farPlane_);
		static Matrix4 ViewportNDC(const int w, const int h);

		Matrix2 ToMatrix2() const;
		static Matrix2 ToMatrix2(const Matrix4& m_);
		Matrix3 ToMatrix3() const;
		static Matrix3 ToMatrix3(const Matrix4& m_);
		Euler ToEuler() const;
		static Euler ToEuler(const Matrix4& m_);
		Quaternion ToQuaternion() const;
		static Quaternion ToQuaternion(const Matrix4& m_);

		std::string ToString() const;
	private:
		float m[16];
	};
}

#endif //!MATRIX_H