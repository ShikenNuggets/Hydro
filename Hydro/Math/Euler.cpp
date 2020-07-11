#include "Euler.h"

#include "Math.h"
#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"

using namespace Hydro;

Euler::Euler(const float x_, const float y_, const float z_) : x(x_), y(y_), z(z_){
}

Euler::Euler(const Vector3& v) : x(v.x), y(v.y), z(v.z){
}

Euler::~Euler(){
}

Euler Euler::operator -() const{
	return Euler(-x, -y, -z);
}

Euler Euler::operator +(const Euler& e) const{
	return Euler(x + e.x, y + e.y, z + e.z);
}

Euler Euler::operator -(const Euler& e) const{
	return Euler(x - e.x, y - e.y, z - e.z);
}

Euler Euler::operator *(const float f) const{
	return Euler(x * f, y * f, z * f);
}

Euler Euler::operator /(const float f) const{
	if(Math::NearZero(f)){
		//Debug::LogWarning("Attempted division by zero or near-zero! Returning a zero Euler", __FILE__, __LINE__);
		return Euler(0.0f, 0.0f, 0.0f);
	}

	return Euler(x / f, y / f, z / f);
}

void Euler::operator +=(const Euler& e){
	*this = *this + e;
}

void Euler::operator -=(const Euler& e){
	*this = *this - e;
}

void Euler::operator *=(const float f){
	*this = *this * f;
}

void Euler::operator /=(const float f){
	*this = *this / f;
}

Matrix3 Euler::ToMatrix3() const{
	return Matrix3(this->ToMatrix4());
}

Matrix3 Euler::ToMatrix3(const Euler& e_){
	return Matrix3(e_.ToMatrix4());
}

Matrix4 Euler::ToMatrix4() const{
	return	Matrix4::Rotate(y, Vector3(0.0f, 1.0f, 0.0f)) *
			Matrix4::Rotate(z, Vector3(0.0f, 0.0f, 1.0f)) * 
			Matrix4::Rotate(x, Vector3(1.0f, 0.0f, 0.0f));
}

Matrix4 Euler::ToMatrix4(const Euler& e_){
	return e_.ToMatrix4();
}

Quaternion Euler::ToQuaternion() const{
	float bank = Math::ConvertToRadians(x);
	float heading = Math::ConvertToRadians(y);
	float attitude = Math::ConvertToRadians(z);

	float c1 = cos(heading / 2.0f);
	float c2 = cos(attitude / 2.0f);
	float c3 = cos(bank / 2.0f);

	float s1 = sin(heading / 2.0f);
	float s2 = sin(attitude / 2.0f);
	float s3 = sin(bank / 2.0f);

	float w = c1 * c2 * c3 - s1 * s2 * s3;
	float x = s1 * s2 * c3 + c1 * c2 * s3;
	float y = s1 * c2 * c3 + c1 * s2 * s3;
	float z = c1 * s2 * c3 - s1 * c2 * s3;

	return Quaternion(w, x, y, z).Normalized();
}

Quaternion Euler::ToQuaternion(const Euler& e_){
	return e_.ToQuaternion();
}

std::string Euler::ToString() const{
	return std::string(std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z));
}

Euler::operator const float* () const{
	return static_cast<const float*>(&x);
}

Euler::operator float* (){
	return static_cast<float*>(&x);
}