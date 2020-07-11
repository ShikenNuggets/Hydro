#include "Matrix.h"

#include "Math.h"

using namespace Hydro;

Matrix2::Matrix2() : m(){
	*this = Identity();
}

Matrix2::Matrix2(const float x1, const float x2, const float y1, const float y2){
	m[0] = x1;
	m[1] = x2;
	m[2] = y1;
	m[3] = y2;
}

Matrix2::Matrix2(const float fillValue) : m(){
	for(unsigned int i = 0; i < 4; i++){
		m[i] = fillValue;
	}
}

Matrix2::Matrix2(const Matrix3& m_) {
	this->m[0] = m_[0]; this->m[2] = m_[3];
	this->m[1] = m_[1]; this->m[3] = m_[4];
	
}

Matrix2::Matrix2(const Matrix4& m_) {
	this->m[0] = m_[0]; this->m[2] = m_[4];
	this->m[1] = m_[1]; this->m[3] = m_[5];
}

Matrix2::~Matrix2(){
}

const float Matrix2::operator [](const int i) const{
	_ASSERT(i > 0 || i < 4);
	return m[i];
}

float& Matrix2::operator [](const int i){
	_ASSERT(i > 0 || i < 4);
	return m[i];
}

Matrix2 Matrix2::operator +(const Matrix2& m_) const{
	Matrix2 result = Matrix2(0.0f);
	for(unsigned int i = 0; i < 4; i++){
		result[i] = m[i] + m_[i];
	}

	return result;
}

Matrix2 Matrix2::operator -(const Matrix2& m_) const{
	Matrix2 result = Matrix2(0.0f);
	for(unsigned int i = 0; i < 4; i++){
		result[i] = m[i] - m_[i];
	}

	return result;
}

Matrix2 Matrix2::operator *(const float s) const{
	return Matrix2(m[0] * s, m[1] * s, m[2] * s, m[3] * s);
}

Matrix2 Matrix2::operator *(const Matrix2& m_) const{
	Matrix2 result = Matrix2(0.0f);
	result[0] = (m[0] * m_[0]) + (m[2] * m_[1]);
	result[2] = (m[0] * m_[2]) + (m[2] * m_[3]);
	
	result[1] = (m[1] * m_[0]) + (m[3] * m_[1]);
	result[3] = (m[1] * m_[2]) + (m[3] * m_[3]);
	
	return result;
}

Matrix2 Matrix2::operator /(const float s) const{
	if(Math::NearZero(s)){
		//Debug::LogWarning("Attempted division by zero or near-zero! Returning a zero matrix!", __FILE__, __LINE__);
		return Matrix2(0.0f);
	}

	return Matrix2(m[0] / s, m[1] / s, m[2] / s, m[3] / s);
}

void Matrix2::operator +=(const Matrix2& m_){
	*this = *this + m_;
}

void Matrix2::operator -=(const Matrix2& m_){
	*this = *this - m_;
}

void Matrix2::operator *=(const float s){
	*this = *this * s;
}

void Matrix2::operator *=(const Matrix2& m_){
	*this = *this * m_;
}

void Matrix2::operator /=(const float s){
	*this = *this / s;
}

Matrix2::operator float*(){
	return static_cast<float*>(&m[0]);
}

Matrix2::operator const float*() const{
	return static_cast<const float*>(&m[0]);
}

Matrix2 Matrix2::Identity(){
	return Matrix2(	1.0f, 0.0f, 
					0.0f, 1.0f);
}

Matrix2 Matrix2::Transpose() const{
	return Matrix2(	m[0], m[2],
					m[1], m[3]);
}

Matrix2 Matrix2::Transpose(const Matrix2& m_){
	return m_.Transpose();
}

Matrix2 Matrix2::Inverse() const{
	float det = m[0] * m[3] - m[2] * m[1];
	if(Math::NearZero(det)){
		//Debug::LogWarning("Determinant was zero or near zero! Returning identity matrix!");
		return Matrix2::Identity();
	}

	float invdet = 1.0f / det;

	return Matrix2(	m[3] * invdet, -m[1] * invdet,
		-m[2] * invdet, m[0] * invdet);
}

Matrix2 Matrix2::Inverse(const Matrix2& m_){
	return m_.Inverse();
}

Matrix3 Matrix2::ToMatrix3() const{
	Matrix3 m_ = Matrix3::Identity();

	m_[0] = m[0];
	m_[1] = m[1];
	m_[3] = m[2];
	m_[4] = m[3];
	
	return m_;
}

Matrix3 Matrix2::ToMatrix3(const Matrix2& m_){
	return m_.ToMatrix3();
}

Matrix4 Matrix2::ToMatrix4() const{
	Matrix4 m_ = Matrix4::Identity();

	m_[0] = m[0];
	m_[1] = m[1];
	m_[4] = m[2];
	m_[5] = m[3];

	return m_;
}

Matrix4 Matrix2::ToMatrix4(const Matrix2& m_){
	return m_.ToMatrix4();
}

std::string Matrix2::ToString() const{
	std::string mString;

	mString =	std::to_string(m[0]) + ", " + std::to_string(m[2]) + ",\n" +
				std::to_string(m[1]) + ", " + std::to_string(m[3]);

	return mString;
}