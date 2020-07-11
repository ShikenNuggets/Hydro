#include "Quaternion.h"

#include <cmath>
#include <iostream>

#include "Euler.h"
#include "Math.h"

using namespace Hydro;

Quaternion::Quaternion(float w_, float x_, float y_, float z_) : w(w_), x(x_), y(y_), z(z_){
}

Quaternion::Quaternion(const float w_, const Vector3& v_) : w(w_), x(v_.x), y(v_.y), z(v_.z){
}

Quaternion::Quaternion(const Vector4& v_) : w(v_.w), x(v_.x), y(v_.y), z(v_.z){
}

Quaternion::~Quaternion(){
}

Quaternion Quaternion::operator +(const Quaternion& q) const{
	return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z);
}

Quaternion Quaternion::operator *(const Quaternion& q) const{
	Quaternion result = Quaternion();
	result.x = x * q.w + y * q.z - z * q.y + w * q.x;
	result.y = -x * q.z + y * q.w + z * q.x + w * q.y;
	result.z = x * q.y - y * q.x + z * q.w + w * q.z;
	result.w = -x * q.x - y * q.y - z * q.z + w * q.w;
	return result;
}

Quaternion Quaternion::operator *(const float f) const{
	return Quaternion(w * f, x * f, y * f, z * f);
}

Quaternion Quaternion::operator /(const float f) const{
	return Quaternion(w / f, x / f, y / f, z / f);
}

void Quaternion::operator +=(const Quaternion& q){
	*this = *this + q;
}

void Quaternion::operator *=(const Quaternion& q){
	*this = *this * q;
}

void Quaternion::operator *=(const float f){
	*this = *this * f;
}

void Quaternion::operator /=(const float f){
	*this = *this / f;
}

float Quaternion::Dot(const Quaternion& a, const Quaternion& b){
	return(a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z);
}

float Quaternion::Magnitude(const Quaternion& q){
	return sqrt((q.w * q.w) + (q.x * q.x) + (q.y * q.y) + (q.z * q.z));
}

Quaternion Quaternion::Normalize(const Quaternion& q){
	return q / Magnitude(q);
}

void Quaternion::Normalize(){
	*this = Quaternion::Normalize(*this);
}

Quaternion Quaternion::Normalized() const{
	return Quaternion::Normalize(*this);
}

Quaternion Quaternion::Inverse() const{
	const Quaternion conjugate = Quaternion(w, -x, -y, -z);
	const float magnitude = Magnitude(conjugate);
	return conjugate / (magnitude * magnitude);
}

Quaternion Quaternion::Rotate(const float angle, const Vector3& axis){
	Quaternion q = Quaternion();
	q.w = cos(Math::ConvertToRadians(angle) / 2.0f);
	q.x = sin(Math::ConvertToRadians(angle) / 2.0f) * axis.x;
	q.y = sin(Math::ConvertToRadians(angle) / 2.0f) * axis.y;
	q.z = sin(Math::ConvertToRadians(angle) / 2.0f) * axis.z;
	return q;
}

float Quaternion::GetRotationAngle(const Quaternion& q){
	return Math::ConvertToDegrees(acos(q.w)) * 2.0f;
}

Quaternion Quaternion::LookAt(const Vector3& source_, const Vector3& destination_){
	Vector3 forwardVector = (destination_ - source_).Normalized();

	float dot = Vector3::Dot(Vector3::worldForward, forwardVector);

	if(Math::Abs(dot - (-1.0f)) < Math::NearZero()){
		return Quaternion(Math::PI(), Vector3::worldUp);
	}
	
	if(Math::Abs(dot - (1.0f)) < Math::NearZero()){
		return Quaternion::Identity();
	}

	float rotAngle = Math::Acos(dot);
	Vector3 rotAxis = Vector3::Cross(Vector3::worldForward, forwardVector).Normalized();
	return Quaternion::Rotate(rotAngle, rotAxis);
}

Vector3 Quaternion::GetRotationAxis(const Quaternion& q){
	Vector3 v = Vector3(q.x, q.y, q.z);
	return v / sin(Math::ConvertToRadians(GetRotationAngle(q) / 2.0f));
}

Quaternion Quaternion::Lerp(const Quaternion& q1, const Quaternion& q2, const float t){
	return Quaternion::Normalize((q1 * (1 - t) + q2 * t));
}

Quaternion Quaternion::Slerp(const Quaternion& q1_, const Quaternion& q2_, const float t){
	if(t <= 0.0f){
		return q1_;
	}else if(t >= 1.0f){
		return q2_;
	}

	Quaternion q1 = Quaternion::Normalize(q1_);
	Quaternion q2 = Quaternion::Normalize(q2_);

	const float dot = Quaternion::Dot(q1, q2);
	const float theta = acos(dot);
	const float sinTheta = sin(theta);

	//Do a regular interpolation if theta is too small
	if(theta < 0.001f || sinTheta < 0.001f){
		return Lerp(q1, q2, t);
	}

	//Final Result
	Quaternion qA = q1 * sin((1.0f - t) * theta) / sinTheta;
	Quaternion qB = q2 * sin(t * theta) / sinTheta; 
	return qA + qB;
}

Matrix3 Quaternion::ToMatrix3() const{
	return Matrix3(ToMatrix4());
}

Matrix3 Quaternion::ToMatrix3(const Quaternion& q_){
	return q_.ToMatrix3();
}

Matrix4 Quaternion::ToMatrix4() const{
	//squared w,x,y,z
	float qw = w * w;
	float qx = x * x;
	float qy = y * y;
	float qz = z * z;

	Matrix4 mat = mat.Identity();
	mat[0] = 1 - 2 * qy - 2 * qz;
	mat[4] = 2 * x * y - 2 * z * w ;
	mat[8] = 2 * x * z + 2 * y * w;

	mat[1] = 2 * x * y + 2 * z * w;
	mat[5] = 1 - 2 * qx - 2 * qz;
	mat[9] = 2 * y * z  - 2 * x * w;

	mat[2] = 2 * x * z - 2 * y * w;
	mat[6] = 2 * y * z + 2 * x * w;
	mat[10] = 1 - 2 * qx - 2 * qy;
	return mat;
}

Matrix4 Quaternion::ToMatrix4(const Quaternion& q_){
	return q_.ToMatrix4();
}

Euler Quaternion::ToEuler() const{
	float heading, attitude, bank;

	float sqw = w * w;
	float sqx = x * x;
	float sqy = y * y;
	float sqz = z * z;
	float unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
	float test = x * y + z * w;

	if (test > 0.499f * unit) { // singularity at north pole
		heading = 2.0f * atan2(x, w);
		attitude = Math::PI() / 2.0f;
		bank = 0.0f;
		return Euler(Math::ConvertToDegrees(bank), Math::ConvertToDegrees(heading), Math::ConvertToDegrees(attitude));
	}
	if (test < -0.499f * unit) { // singularity at south pole
		heading = -2.0f * atan2(x, w);
		attitude = -Math::PI() / 2.0f;
		bank = 0.0f;
		return Euler(Math::ConvertToDegrees(bank), Math::ConvertToDegrees(heading), Math::ConvertToDegrees(attitude));
	}
	heading = atan2(2.0f * y * w - 2.0f * x * z, sqx - sqy - sqz + sqw);
	attitude = asin(2.0f * test / unit);
	bank = atan2(2.0f * x * w - 2.0f * y * z, -sqx + sqy - sqz + sqw);
	return Euler(Math::ConvertToDegrees(bank), Math::ConvertToDegrees(heading), Math::ConvertToDegrees(attitude));\
}

Euler Quaternion::ToEuler(const Quaternion& q_){
	return q_.ToEuler();
}

std::string Quaternion::ToString() const{
	return std::string(std::to_string(w) + ", (" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")");
}