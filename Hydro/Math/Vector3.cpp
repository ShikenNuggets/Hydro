#include "Vector.h"

#include <iostream>

#include "Math.h"

using namespace Hydro;

const Vector3 Vector3::worldUp =		Vector3(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::worldForward =	Vector3(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::worldRight =		Vector3(1.0f, 0.0f, 0.0f);

Vector3::Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_){}

Vector3 Vector3::operator -() const{
	return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator +(const Vector2& v_) const{
	return Vector3(x + v_.x, y + v_.y, z);
}

Vector3 Vector3::operator +(const Vector3& v_) const{
	return Vector3(x + v_.x, y + v_.y, z + v_.z);
}

Vector3 Vector3::operator -(const Vector2& v_) const{
	return Vector3(x - v_.x, y - v_.y, z);
}

Vector3 Vector3::operator -(const Vector3& v_) const{
	return Vector3(x - v_.x, y - v_.y, z - v_.z);
}

Vector3 Vector3::operator *(float s_) const{
	return Vector3(x * s_, y * s_, z * s_);
}

Vector3 Vector3::operator /(float s_) const{
	if(s_ <= Math::NearZero() && s_ >= -Math::NearZero()){
		//Debug::LogWarning("Attempted division by zero or near-zero! Returning a zero vector", __FILE__, __LINE__);
		return Vector3::Zero();
	}

	return Vector3(x / s_, y / s_, z / s_);
}

void Vector3::operator +=(const Vector2& v_){
	*this = *this + v_;
}

void Vector3::operator +=(const Vector3& v_){
	*this = *this + v_;
}

void Vector3::operator -=(const Vector2& v_){
	*this = *this - v_;
}

void Vector3::operator -=(const Vector3& v_){
	*this = *this - v_;
}

void Vector3::operator *=(float s_){
	*this = *this * s_;
}

void Vector3::operator /=(float s_){
	//This doesn't need to check for nearZero because the division operator already does
	//If this is changed to not use the / operator then make sure it checks for nearZero
	*this = *this / s_;
}

float Vector3::SquaredMagnitude() const{
	return (x * x) + (y * y) + (z * z);
}

float Vector3::SquaredMagnitude(const Vector3& v_){
	return v_.SquaredMagnitude();
}

float Vector3::Magnitude() const{
	return Math::Sqrt(SquaredMagnitude());
}

float Vector3::Magnitude(const Vector3& v_){
	return v_.Magnitude();
}

Vector3 Vector3::Normalized() const{
	//This doesn't need to check for nearZero because the division operator already does
	//If this is changed to not use the / operator then make sure it checks for nearZero
	return *this / Magnitude();
}

void Vector3::Normalize(){
	*this = Normalized();
}

Vector3 Vector3::Normalized(const Vector3& v_){
	return v_.Normalized();
}

Vector3 Vector3::Rotate(const Vector3& n_, float theta_, const Vector3& v_){
	return v_ * cos(theta_) + Vector3::Dot(v_, n_) * n_ * (1.0f - cos(theta_)) + Vector3::Cross(n_, v_) * sin(theta_);
}

Vector3 Vector3::Reflect(const Vector3& v_, const Vector3& n_){
	return ((2.0f * Vector3::Dot(v_, n_)) * n_ - v_);
}

std::string Vector3::ToString() const{
	std::string results = std::to_string(x);
	results += ", " + std::to_string(y);
	results += ", " + std::to_string(z);
	return results;
}

float Vector3::Dot(const Vector3& a_, const Vector3& b_){
	return(a_.x * b_.x + a_.y * b_.y + a_.z * b_.z);
}

Vector3 Vector3::Cross(const Vector3& a_, const Vector3& b_){
	return Vector3(a_.y * b_.z - a_.z * b_.y,
		a_.z * b_.x - a_.x * b_.z,
		a_.x * b_.y - a_.y * b_.x);
}

Vector3 Vector3::Lerp(const Vector3& a_, const Vector3& b_, float t_){
	return a_ + t_ * (b_ - a_);
}

Vector3::operator const float* () const{
	return static_cast<const float*>(&x);
}

Vector3::operator float* (){
	return static_cast<float*>(&x);
}