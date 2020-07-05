#include "Vector.h"

#include "Math.h"

using namespace Hydro;

Vector4::Vector4(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_){}

Vector4 Vector4::operator -() const{
	return Vector4(-x, -y, -z, -w);
}

Vector4 Vector4::operator +(const Vector2& v_) const{
	return Vector4(x + v_.x, y + v_.y, z, w);
}

Vector4 Vector4::operator +(const Vector3& v_) const{
	return Vector4(x + v_.x, y + v_.y, z + v_.z, w);
}

Vector4 Vector4::operator +(const Vector4& v_) const{
	return Vector4(x + v_.x, y + v_.y, z + v_.z, w + v_.w);
}

Vector4 Vector4::operator -(const Vector2& v_) const{
	return Vector4(x - v_.x, y - v_.y, z, w);
}

Vector4 Vector4::operator -(const Vector3& v_) const{
	return Vector4(x - v_.x, y - v_.y, z - v_.z, w);
}

Vector4 Vector4::operator -(const Vector4& v_) const{
	return Vector4(x - v_.x, y - v_.y, z - v_.z, w - v_.w);
}

Vector4 Vector4::operator *(float s_) const{
	return Vector4(x * s_, y * s_, z * s_, w * s_);
}

Vector4 Vector4::operator /(float s_) const{
	if(s_ <= Math::NearZero() || s_ >= -Math::NearZero()){
		//Debug::LogWarning("Attempted division by zero or near-zero! Returning a zero vector", __FILE__, __LINE__);
		return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	return Vector4(x / s_, y / s_, z / s_, w / s_);
}

//Add a Vector2 to this
void Vector4::operator +=(const Vector2& v_){
	*this = *this + v_;
}

//Add a Vector3 to this
void Vector4::operator +=(const Vector3& v_){
	*this = *this + v_;
}

//Add another Vector4 to this
void Vector4::operator +=(const Vector4& v_){
	*this = *this + v_;
}

//Subtract a Vector2 from this
void Vector4::operator -=(const Vector2& v_){
	*this = *this - v_;
}

//Subtract a Vector3 from this
void Vector4::operator -=(const Vector3& v_){
	*this = *this - v_;
}

//Subtract another Vector4 from this
void Vector4::operator -=(const Vector4& v_){
	*this = *this - v_;
}

//Multiply a scalar by this
void Vector4::operator *=(const float s_){
	*this = *this * s_;
}

//Divide a scalar by this
void Vector4::operator /=(float s_){
	//This doesn't need to check for nearZero because the division operator already does
	//If this is changed to not use the division operator then make sure it checks for nearZero
	*this = *this / s_;
}

float Vector4::SquaredMagnitude() const{
	return (x * x) + (y * y) + (z * z) + (w * w);
}

float Vector4::SquaredMagnitude(const Vector4& v_){
	return v_.SquaredMagnitude();
}

float Vector4::Magnitude() const{
	return Math::Sqrt(SquaredMagnitude());
}

float Vector4::Magnitude(const Vector4& v_){
	return v_.Magnitude();
}

Vector4 Vector4::Normalized() const{
	//This doesn't need to check for nearZero because the division operator already does
	//If this is changed to not use the division operator then make sure it checks for nearZero
	return *this / Magnitude();
}

void Vector4::Normalize(){
	*this = Normalized();
}

Vector4 Vector4::Normalized(const Vector4& v_){
	return v_.Normalized();
}

float Vector4::Dot(const Vector4& a_, const Vector4& b_){
	return (a_.x * b_.x) + (a_.y * b_.y) + (a_.z * b_.z) + (a_.w * a_.w);
}

std::string Vector4::ToString() const{
	std::string results = std::to_string(x);
	results += ", " + std::to_string(y);
	results += ", " + std::to_string(z);
	results += ", " + std::to_string(w);
	return results;
}

Vector4::operator const float* () const{
	return static_cast<const float*>(&x);
}

Vector4::operator float* (){
	return static_cast<float*>(&x);
}