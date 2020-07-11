#include "Matrix.h"

#include <algorithm>

#include "Math.h"
#include "Euler.h"
#include "Quaternion.h"

using namespace Hydro;

Matrix3::Matrix3() : m(){
	*this = Identity();
}

Matrix3::Matrix3(	const float x1, const float x2, const float x3,
					const float y1, const float y2, const float y3,
					const float z1, const float z2, const float z3){
	m[0] = x1; m[3] = y1; m[6] = z1;
	m[1] = x2; m[4] = y2; m[7] = z2;
	m[2] = x3; m[5] = y3; m[8] = z3;
}

Matrix3::Matrix3(const float fillValue) : m(){
	for(unsigned int i = 0; i < 9; i++){
		m[i] = fillValue;
	}
}


Matrix3::Matrix3(const Matrix2& m_) {
	this->m[0] = m_[0]; this->m[1] = m_[1]; this->m[2] = 0.0f;
	this->m[3] = m_[2]; this->m[4] = m_[3]; this->m[5] = 0.0f;
	this->m[6] = 0.0f;  this->m[7] = 0.0f;	this->m[8] = 1.0f;
}

Matrix3::Matrix3(const Matrix4& m_) {
	this->m[0] = m_[0]; this->m[1] = m_[1]; this->m[2] = m_[2];
	this->m[3] = m_[4]; this->m[4] = m_[5]; this->m[5] = m_[6];
	this->m[6] = m_[8]; this->m[7] = m_[9]; this->m[8] = m_[10];
}

Matrix3::~Matrix3(){
}

Matrix3& Matrix3::operator =(const Matrix4& m_){
	this->m[0] = m_[0]; this->m[1] = m_[1]; this->m[2] = m_[2];
	this->m[3] = m_[4]; this->m[4] = m_[5]; this->m[5] = m_[6];
	this->m[6] = m_[8]; this->m[7] = m_[9]; this->m[8] = m_[10];
	return *this;
}

const float Matrix3::operator [](const int i) const{
	_ASSERT(i > 0 || i < 9);
	return m[i];
}

float& Matrix3::operator [](const int i){
	_ASSERT(i > 0 || i < 9);
	return m[i];
}

Matrix3 Matrix3::operator +(const Matrix3& m_) const{
	Matrix3 result = Matrix3(0.0f);
	for(unsigned int i = 0; i < 9; i++){
		result[i] = m[i] + m_[i];
	}

	return result;
}

Matrix3 Matrix3::operator -(const Matrix3& m_) const{
	Matrix3 result = Matrix3(0.0f);
	for(unsigned int i = 0; i < 9; i++){
		result[i] = m[i] - m_[i];
	}

	return result;
}

Matrix3 Matrix3::operator *(const float s) const{
	if(s <= Math::NearZero() && s >= -Math::NearZero()){
		//Debug::LogWarning("Attempted division by zero or near-zero! Returning a zero matrix!", __FILE__, __LINE__);
		return Matrix3(0.0f);
	}

	return Matrix3(	m[0] * s, m[1] * s, m[2] * s,
					m[3] * s, m[4] * s, m[5] * s,
					m[6] * s, m[7] * s, m[8] * s);
}

Matrix3 Matrix3::operator *(const Matrix3& m_) const{
	Matrix3 result = Matrix3(0.0f);
	result[0] = (m[0] * m_[0]) + (m[3] * m_[1]) + (m[6] * m_[2]);
	result[3] = (m[0] * m_[3]) + (m[3] * m_[4]) + (m[6] * m_[6]);
	result[6] = (m[0] * m_[6]) + (m[3] * m_[7]) + (m[6] * m_[8]);

	result[1] = (m[1] * m_[0]) + (m[4] * m_[1]) + (m[7] * m_[2]);
	result[4] = (m[1] * m_[3]) + (m[4] * m_[4]) + (m[7] * m_[6]);
	result[7] = (m[1] * m_[6]) + (m[4] * m_[7]) + (m[7] * m_[8]);

	result[2] = (m[2] * m_[0]) + (m[5] * m_[1]) + (m[8] * m_[2]);
	result[5] = (m[2] * m_[3]) + (m[5] * m_[4]) + (m[8] * m_[6]);
	result[8] = (m[2] * m_[6]) + (m[5] * m_[7]) + (m[8] * m_[8]);

	return result;
}

Matrix3 Matrix3::operator /(const float s) const{
	if(Math::NearZero(s)){
		//Debug::LogWarning("Attempted division by zero or near-zero! Returning a zero matrix!", __FILE__, __LINE__);
		return Matrix3(0.0f);
	}

	return Matrix3(	m[0] / s, m[1] / s, m[2] / s,
					m[3] / s, m[4] / s, m[5] / s,
					m[6] / s, m[7] / s, m[8] / s);
}

void Matrix3::operator +=(const Matrix3& m_){
	*this = *this + m_;
}

void Matrix3::operator -=(const Matrix3& m_){
	*this = *this - m_;
}

void Matrix3::operator *=(const float s){
	*this = *this * s;
}

void Matrix3::operator *=(const Matrix3& m_){
	*this = *this * m_;
}

void Matrix3::operator /=(const float s){
	*this = *this / s;
}

Matrix3::operator float*(){
	return static_cast<float*>(&m[0]);
}

Matrix3::operator const float*() const{
	return static_cast<const float*>(&m[0]);
}

Matrix3 Matrix3::Identity(){
	return Matrix3(	1.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 1.0f);
}

Matrix3 Matrix3::Transpose() const{
	return Matrix3(	m[0], m[3], m[6],
					m[1], m[4], m[7],
					m[2], m[5], m[8]);
}

Matrix3 Matrix3::Transpose(const Matrix3& m_){
	return m_.Transpose();
}

Matrix3 Matrix3::Inverse() const{
	float det =	m[0] * (m[4] * m[8] - m[7] * m[5]) -
				m[1] * (m[3] * m[8] - m[5] * m[6]) +
				m[2] * (m[3] * m[7] - m[4] * m[6]);

	if(Math::NearZero(det)){
		//Debug::LogWarning("Determinant was zero or near zero! Returning identity matrix!");
		return Matrix3::Identity();
	}

	float invdet = 1.0f / det;

	Matrix3 inverse = Matrix3::Identity();

	inverse[0] = (m[4] * m[8] - m[7] * m[5]) * invdet;
	inverse[1] = (m[2] * m[7] - m[1] * m[8]) * invdet;
	inverse[2] = (m[1] * m[5] - m[2] * m[4]) * invdet;
	inverse[3] = (m[5] * m[6] - m[3] * m[8]) * invdet;
	inverse[4] = (m[0] * m[8] - m[2] * m[6]) * invdet;
	inverse[5] = (m[3] * m[2] - m[0] * m[5]) * invdet;
	inverse[6] = (m[3] * m[7] - m[6] * m[4]) * invdet;
	inverse[7] = (m[6] * m[1] - m[0] * m[7]) * invdet;
	inverse[8] = (m[0] * m[4] - m[3] * m[1]) * invdet;

	return inverse;
}

Matrix3 Matrix3::Inverse(const Matrix3& m_){
	return m_.Inverse();
}

Matrix2 Matrix3::ToMatrix2() const{
	Matrix2 m_ = Matrix2::Identity();

	m_[0] = m[0];
	m_[1] = m[1];
	m_[2] = m[3];
	m_[3] = m[4];

	return m_;
}

Matrix2 Matrix3::ToMatrix2(const Matrix3& m_){
	return m_.ToMatrix2();
}

Matrix4 Matrix3::ToMatrix4() const{
	Matrix4 m_ = Matrix4::Identity();

	m_[0] = m[0];
	m_[1] = m[1];
	m_[2] = m[2];
	m_[4] = m[3];
	m_[5] = m[4];
	m_[6] = m[5];
	m_[8] = m[6];
	m_[9] = m[7];
	m_[10] = m[8];

	return m_;
}

Matrix4 Matrix3::ToMatrix4(const Matrix3& m_){
	return m_.ToMatrix4();
}

Euler Matrix3::ToEuler() const{
	float heading;
	float attitude = asin(m[1]);
	float bank;

	if(m[1] >= 0.99f){
		heading = atan2(m[6], m[8]);
		bank = 0;
	}else if(m[1] <= -0.99f){
		heading = atan2(m[6], m[8]);
		bank = 0;
	}else{
		heading = atan2(-m[2], m[0]);
		attitude = asin(m[1]);
		bank = atan2(-m[7], m[4]);
	}

	return Euler(Math::ConvertToDegrees(bank), Math::ConvertToDegrees(heading), Math::ConvertToDegrees(attitude));
}

Euler Matrix3::ToEuler(const Matrix3& m_){
	return m_.ToEuler();
}

Quaternion Matrix3::ToQuaternion() const{
	float qw;
	float qx;
	float qy;
	float qz;

	qw = sqrt(std::max(0.0f, 1.0f + m[0] + m[4] + m[8])) / 2.0f;
	qx = sqrt(std::max(0.0f, 1.0f + m[0] - m[4] - m[8])) / 2.0f;
	qy = sqrt(std::max(0.0f, 1.0f - m[0] + m[4] - m[8])) / 2.0f;
	qz = sqrt(std::max(0.0f, 1.0f - m[0] - m[4] + m[8])) / 2.0f;

	if(m[5] - m[7] != 0.0f){
		qx = copysign(qx, m[5] - m[7]);
	}else{
		qx = 0.0f;
	}

	if(m[6] - m[2] != 0.0f){
		qy = copysign(qy, m[6] - m[2]);
	}else{
		qy = 0.0f;
	}

	if(m[1] - m[3] != 0.0f){
		qz = copysign(qz, m[1] - m[3]);
	}else{
		qz = 0.0f;
	}

	return Quaternion(qw, qx, qy, qz).Normalized();
}

Quaternion Matrix3::ToQuaternion(const Matrix3& m_){
	return m_.ToQuaternion();
}

std::string Matrix3::ToString() const{
	std::string mString;

	mString =	std::to_string(m[0]) + ", " + std::to_string(m[3]) + ", " + std::to_string(m[6]) + ",\n" +
				std::to_string(m[1]) + ", " + std::to_string(m[4]) + ", " + std::to_string(m[7]) + ",\n" +
				std::to_string(m[2]) + ", " + std::to_string(m[5]) + ", " + std::to_string(m[8]);

	return mString;
}