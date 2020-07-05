#include "Vector.h"

#include "Math.h"

using namespace Hydro;

Vector2::Vector2(float x_, float y_) : x(x_), y(y_){}

Vector2 Vector2::operator -() const{
	return Vector2(-x, -y);
}

Vector2 Vector2::operator +(const Vector2& v_) const{
	return Vector2(x + v_.x, y + v_.y);
}

Vector2 Vector2::operator -(const Vector2& v_) const{
	return Vector2(x - v_.x, y - v_.y);
}

Vector2 Vector2::operator *(float s_) const{
	return Vector2(x * s_, y * s_);
}

Vector2 Vector2::operator /(float s_) const{
	if(s_ <= Math::NearZero() && s_ >= -Math::NearZero()){
		//Debug::LogWarning("Attempted division by zero or near-zero! Returning a zero vector", __FILE__, __LINE__);
		return Vector2::Zero();
	}

	return Vector2(x / s_, y / s_);
}

void Vector2::operator +=(const Vector2& v_){
	*this = *this + v_;
}

void Vector2::operator -=(const Vector2& v_){
	*this = *this - v_;
}

void Vector2::operator *=(float s_){
	*this = *this * s_;
}

void Vector2::operator /=(float s_){
	//This doesn't need to check for nearZero because the division operator already does
	//If this is changed to not use the / operator then make sure it checks for nearZero
	*this = *this / s_;
}

float Vector2::SquaredMagnitude() const{
	return (x * x) + (y * y);
}

float Vector2::SquaredMagnitude(const Vector2& v_){
	return v_.SquaredMagnitude();
}

float Vector2::Magnitude() const{
	return Math::Sqrt(SquaredMagnitude());
}

float Vector2::Magnitude(const Vector2& v_){
	return v_.Magnitude();
}

Vector2 Vector2::Normalized() const{
	//This doesn't need to check for nearZero because the division operator already does
	//If this is changed to not use the / operator then make sure it checks for nearZero
	return *this / Magnitude();
}

void Vector2::Normalize(){
	*this = Normalized();
}

Vector2 Vector2::Normalized(const Vector2& v_){
	return v_.Normalized();
}

float Vector2::Dot(const Vector2& a_, const Vector2& b_){
	return (a_.x * b_.x) + (a_.y * b_.y);
}

std::string Vector2::ToString() const{
	std::string result = std::to_string(x);
	result += ", " + std::to_string(y);
	return result;
}

Vector2::operator const float* () const{
	return static_cast<const float*>(&x);
}

Vector2::operator float* (){
	return static_cast<float*>(&x);
}