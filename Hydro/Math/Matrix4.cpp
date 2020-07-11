#include "Matrix.h"

#include "Math.h"
#include "Euler.h"
#include "Quaternion.h"
#include "Matrix.h"

using namespace Hydro;

Matrix4::Matrix4() : m(){
	*this = Identity();
}

Matrix4::Matrix4(	const float x1_, const float x2_, const float x3_, const float x4_,
					const float y1_, const float y2_, const float y3_, const float y4_,
					const float z1_, const float z2_, const float z3_, const float z4_,
					const float w1_, const float w2_, const float w3_, const float w4_){
	m[0] = x1_; m[4] = y1_; m[8] = z1_;  m[12] = w1_;
	m[1] = x2_; m[5] = y2_; m[9] = z2_;  m[13] = w2_;
	m[2] = x3_; m[6] = y3_; m[10] = z3_; m[14] = w3_;
	m[3] = x4_; m[7] = y4_; m[11] = z4_; m[15] = w4_;
}

Matrix4::Matrix4(const float fillValue) : m(){
	for(unsigned int i = 0; i < 16; i++){
		m[i] = fillValue;
	}
}

Matrix4::Matrix4(const Matrix2& m_) {
	this->m[0] = m_[0]; this->m[4] = m_[2]; this->m[8] = 0.0f; this->m[12] = 0.0f;
	this->m[1] = m_[1]; this->m[5] = m_[3]; this->m[9] = 0.0f; this->m[13] = 0.0f;
	this->m[2] = 0.0f;  this->m[6] = 0.0f;	this->m[10] = 1.0f; this->m[14] = 0.0f;
	this->m[3] = 0.0f;  this->m[7] = 0.0f;	this->m[11] = 0.0f; this->m[15] = 1.0f;
}

Matrix4::Matrix4(const Matrix3& m_) {
	this->m[0] = m_[0]; this->m[4] = m_[3];		this->m[8] = m_[6];  this->m[12] = 0.0f;
	this->m[1] = m_[1]; this->m[5] = m_[4];		this->m[9] = m_[7];  this->m[13] = 0.0f;
	this->m[2] = m_[2];  this->m[6] = m_[5];	this->m[10] = m_[8]; this->m[14] = 0.0f;
	this->m[3] = 0.0f;  this->m[7] = 0.0f;		this->m[11] = 0.0f;  this->m[15] = 1.0f;
}

Matrix4::~Matrix4(){
}

const float Matrix4::operator [](const int i) const{
	_ASSERT(i > 0 || i < 16);
	return m[i];
}

float& Matrix4::operator [](const int i){
	_ASSERT(i > 0 || i < 16);
	return m[i];
}

Matrix4 Matrix4::operator +(const Matrix4& m_) const{
	Matrix4 result = Matrix4(0.0f);
	for(int i = 0; i < 16; i++){
		result[i] = m[i] + m_[i];
	}

	return result;
}

Matrix4 Matrix4::operator -(const Matrix4& m_) const{
	Matrix4 result = Matrix4(0.0f);
	for(int i = 0; i < 16; i++){
		result[i] = m[i] - m_[i];
	}

	return result;
}

Matrix4 Matrix4::operator *(const float s) const{
	return Matrix4(	m[0] * s, m[1] * s, m[2] * s, m[3] * s,
					m[4] * s, m[5] * s, m[6] * s, m[7] * s,
					m[8] * s, m[9] * s, m[10] * s, m[11] * s,
					m[12] * s, m[13] * s, m[14] * s, m[15] * s
	);
}

Matrix4 Matrix4::operator*(const Matrix4& m_) const{
	return Matrix4(
		(m[0 * 4 + 0] * m_[0 * 4 + 0]) + (m[1 * 4 + 0] * m_[0 * 4 + 1]) + (m[2 * 4 + 0] * m_[0 * 4 + 2]) + (m[3 * 4 + 0] * m_[0 * 4 + 3]),
		(m[0 * 4 + 1] * m_[0 * 4 + 0]) + (m[1 * 4 + 1] * m_[0 * 4 + 1]) + (m[2 * 4 + 1] * m_[0 * 4 + 2]) + (m[3 * 4 + 1] * m_[0 * 4 + 3]),
		(m[0 * 4 + 2] * m_[0 * 4 + 0]) + (m[1 * 4 + 2] * m_[0 * 4 + 1]) + (m[2 * 4 + 2] * m_[0 * 4 + 2]) + (m[3 * 4 + 2] * m_[0 * 4 + 3]),
		(m[0 * 4 + 3] * m_[0 * 4 + 0]) + (m[1 * 4 + 3] * m_[0 * 4 + 1]) + (m[2 * 4 + 3] * m_[0 * 4 + 2]) + (m[3 * 4 + 3] * m_[0 * 4 + 3]),
		(m[0 * 4 + 0] * m_[1 * 4 + 0]) + (m[1 * 4 + 0] * m_[1 * 4 + 1]) + (m[2 * 4 + 0] * m_[1 * 4 + 2]) + (m[3 * 4 + 0] * m_[1 * 4 + 3]),
		(m[0 * 4 + 1] * m_[1 * 4 + 0]) + (m[1 * 4 + 1] * m_[1 * 4 + 1]) + (m[2 * 4 + 1] * m_[1 * 4 + 2]) + (m[3 * 4 + 1] * m_[1 * 4 + 3]),
		(m[0 * 4 + 2] * m_[1 * 4 + 0]) + (m[1 * 4 + 2] * m_[1 * 4 + 1]) + (m[2 * 4 + 2] * m_[1 * 4 + 2]) + (m[3 * 4 + 2] * m_[1 * 4 + 3]),
		(m[0 * 4 + 3] * m_[1 * 4 + 0]) + (m[1 * 4 + 3] * m_[1 * 4 + 1]) + (m[2 * 4 + 3] * m_[1 * 4 + 2]) + (m[3 * 4 + 3] * m_[1 * 4 + 3]),
		(m[0 * 4 + 0] * m_[2 * 4 + 0]) + (m[1 * 4 + 0] * m_[2 * 4 + 1]) + (m[2 * 4 + 0] * m_[2 * 4 + 2]) + (m[3 * 4 + 0] * m_[2 * 4 + 3]),
		(m[0 * 4 + 1] * m_[2 * 4 + 0]) + (m[1 * 4 + 1] * m_[2 * 4 + 1]) + (m[2 * 4 + 1] * m_[2 * 4 + 2]) + (m[3 * 4 + 1] * m_[2 * 4 + 3]),
		(m[0 * 4 + 2] * m_[2 * 4 + 0]) + (m[1 * 4 + 2] * m_[2 * 4 + 1]) + (m[2 * 4 + 2] * m_[2 * 4 + 2]) + (m[3 * 4 + 2] * m_[2 * 4 + 3]),
		(m[0 * 4 + 3] * m_[2 * 4 + 0]) + (m[1 * 4 + 3] * m_[2 * 4 + 1]) + (m[2 * 4 + 3] * m_[2 * 4 + 2]) + (m[3 * 4 + 3] * m_[2 * 4 + 3]),
		(m[0 * 4 + 0] * m_[3 * 4 + 0]) + (m[1 * 4 + 0] * m_[3 * 4 + 1]) + (m[2 * 4 + 0] * m_[3 * 4 + 2]) + (m[3 * 4 + 0] * m_[3 * 4 + 3]),
		(m[0 * 4 + 1] * m_[3 * 4 + 0]) + (m[1 * 4 + 1] * m_[3 * 4 + 1]) + (m[2 * 4 + 1] * m_[3 * 4 + 2]) + (m[3 * 4 + 1] * m_[3 * 4 + 3]),
		(m[0 * 4 + 2] * m_[3 * 4 + 0]) + (m[1 * 4 + 2] * m_[3 * 4 + 1]) + (m[2 * 4 + 2] * m_[3 * 4 + 2]) + (m[3 * 4 + 2] * m_[3 * 4 + 3]),
		(m[0 * 4 + 3] * m_[3 * 4 + 0]) + (m[1 * 4 + 3] * m_[3 * 4 + 1]) + (m[2 * 4 + 3] * m_[3 * 4 + 2]) + (m[3 * 4 + 3] * m_[3 * 4 + 3]));
}

Vector3 Matrix4::operator *(const Vector3& v) const{
	float x = v.x * m[0] + v.y * m[4] + v.z * m[8]  + 1.0f * m[12];
	float y = v.x * m[1] + v.y * m[5] + v.z * m[9]  + 1.0f * m[13];
	float z = v.x * m[2] + v.y * m[6] + v.z * m[10] + 1.0f * m[14];
	return Vector3(x, y, z);
}

Vector4 Matrix4::operator *(const Vector4& v) const{
	float x  =  v.x * m[0] + v.y * m[4] + v.z * m[8]  + v.w * m[12];
	float y  =  v.x * m[1] + v.y * m[5] + v.z * m[9]  + v.w * m[13];
	float z  =  v.x * m[2] + v.y * m[6] + v.z * m[10] + v.w * m[14];
	float w  =  v.x * m[3] + v.y * m[7] + v.z * m[11] + v.w * m[15];
	return Vector4(x, y, z, w);
}

Matrix4 Matrix4::operator /(const float s) const{
	if(Math::NearZero(s)){
		//Debug::LogWarning("Attempted division by zero or near-zero! Returning a zero matrix!", __FILE__, __LINE__);
		return Matrix4(0.0f);
	}

	return Matrix4(	m[0] / s, m[1] / s, m[2] / s, m[3] / s,
					m[4] / s, m[5] / s, m[6] / s, m[7] / s,
					m[8] / s, m[9] / s, m[10] / s, m[11] / s,
					m[12] / s, m[13] / s, m[14] / s, m[15] / s
	);
}

void Matrix4::operator +=(const Matrix4& m_){
	*this = *this + m_;
}

void Matrix4::operator -=(const Matrix4& m_){
	*this = *this - m_;
}

void Matrix4::operator *=(const float s){
	*this = *this * s;
}

void Matrix4::operator *=(const Matrix4& m_){
	*this = *this * m_;
}

void Matrix4::operator /=(const float s){
	*this = *this / s;
}

Matrix4::operator float*(){
	return static_cast<float*>(&m[0]);
}

Matrix4::operator const float*() const{
	return static_cast<const float*>(&m[0]);
}

Matrix4 Matrix4::Identity(){
	return Matrix4(	1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4 Matrix4::Transpose() const{
	return Matrix4(	m[0], m[4], m[8], m[12],
					m[1], m[5], m[9], m[13],
					m[2], m[6], m[10], m[14],
					m[3], m[7], m[11], m[15]);

}

Matrix4 Matrix4::Transpose(const Matrix4& m_){
	return m_.Transpose();
}

Matrix4 Matrix4::Inverse() const{
	Matrix4 inverseM;
	inverseM[0] =  m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
	inverseM[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
	inverseM[2] =  m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
	inverseM[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
	inverseM[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
	inverseM[5] =  m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
	inverseM[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
	inverseM[7] =  m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
	inverseM[8] =  m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
	inverseM[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
	inverseM[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
	inverseM[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
	inverseM[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
	inverseM[13] =  m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
	inverseM[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
	inverseM[15] =  m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

	float determinant = m[0] * inverseM[0] + m[1] * inverseM[4] + m[2] * inverseM[8] + m[3] * inverseM[12];

	if(Math::NearZero(determinant)){
		//Debug::LogWarning("Determinant was zero or near zero! Returning identity matrix!");
		return Matrix4::Identity();
	}

	inverseM /= determinant;

	return inverseM;
}

Matrix4 Matrix4::Inverse(const Matrix4& m_){
	return m_.Inverse();
}

Matrix4 Matrix4::Translate(const Vector3& v_){
	return Matrix4(	1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		v_.x, v_.y, v_.z, 1.0f);
}

Matrix4 Matrix4::LookAt(	float eyeX, float eyeY, float eyeZ,
	float atX, float atY, float atZ,
	float upX, float upY, float upZ) {

	Vector3 at(atX,atY,atZ);
	Vector3 up(upX,upY,upZ);
	Vector3 eye(eyeX,eyeY,eyeZ);

	Matrix4 result;

	Vector3 forward = (at - eye).Normalized();
	up.Normalize();
	Vector3 side = Vector3::Cross(forward, up).Normalized();
	up = Vector3::Cross(side,forward);

	result[0] = side.x;
	result[1] = side.y;
	result[2] = side.z;
	result[3] = 0.0;

	result[4] = up.x;
	result[5] = up.y;
	result[6] = up.z;
	result[7] = 0.0;

	result[8]  = -forward.x;
	result[9]  = -forward.y;
	result[10] = -forward.z;
	result[11] = 0.0;

	result[12] = -Vector3::Dot(side,eye);	
	result[13] = -Vector3::Dot(up,eye);
	result[14] =  Vector3::Dot(forward,eye);
	result[15] = 1.0;

	return result;
}

Matrix4 Matrix4::LookAt(const Vector3& eye, const Vector3& at, const Vector3& up) {
	return LookAt(eye.x, eye.y, eye.z, at.x, at.y, at.z, up.x, up.y, up.z);
}

Matrix4 Matrix4::Rotate(const float angle_, const Vector3& v_) {
	float cosang, sinang, cosm, TempAngle;
	Vector3 rotAxis(v_.x, v_.y, v_.z);

	rotAxis.Normalize();
	TempAngle = Math::ConvertToRadians(angle_);
	cosang = cos(TempAngle);
	sinang = sin(TempAngle);
	cosm = (1.0f - cosang);

	Matrix4 m;

	m[0] = (rotAxis.x * rotAxis.x * cosm) + cosang;
	m[1] = (rotAxis.x * rotAxis.y * cosm) + (rotAxis.z * sinang);
	m[2] = (rotAxis.x * rotAxis.z * cosm) - (rotAxis.y * sinang);
	m[3] = 0.0;
	m[4] = (rotAxis.x * rotAxis.y * cosm) - (rotAxis.z * sinang);
	m[5] = (rotAxis.y * rotAxis.y * cosm) + cosang;
	m[6] = (rotAxis.y * rotAxis.z * cosm) + (rotAxis.x * sinang);
	m[7] = 0.0;
	m[8] = (rotAxis.x * rotAxis.z * cosm) + (rotAxis.y * sinang);
	m[9] = (rotAxis.y * rotAxis.z * cosm) - (rotAxis.x * sinang);
	m[10] = (rotAxis.z * rotAxis.z * cosm) + cosang;
	m[11] = 0.0;
	m[12] = 0.0;
	m[13] = 0.0;
	m[14] = 0.0;
	m[15] = 1.0;
	return m;
}

Matrix4 Matrix4::Scale(const Vector3& v_){
	return Matrix4(	v_.x, 0.0f, 0.0f, 0.0f,
		0.0f, v_.y, 0.0f, 0.0f,
		0.0f, 0.0f, v_.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

Vector3 Matrix4::GetTranslation(){
	return Vector3(m[12], m[13], m[14]);
}

Quaternion Matrix4::GetRotation(){
	return ToQuaternion();
}

Vector3 Matrix4::GetScale(){
	return Vector3(m[0], m[5], m[10]);
}

Matrix4 Matrix4::Orthographic(const float left_, const float right_, const float bottom_, const float top_) {
	Matrix4 ortho = Matrix4(0);

	ortho[0] = 2.0f / (right_ - left_);
	ortho[5] = 2.0f / (top_ - bottom_);
	ortho[10] = -2.0f / (1.0f - (-1.0f));
	ortho[12] = -(right_ + left_) / (right_ - left_);
	ortho[13] = -(top_ + bottom_) / (top_ - bottom_);
	ortho[14] = -(1.0f + -1.0f) / (1.0f - (-1.0f));
	ortho[15] = 1.0f;

	return ortho;
}

Matrix4 Matrix4::Orthographic(const float left_, const float right_, const float bottom_, const float top_, const float near_, const float far_) {
	Matrix4 ortho = Matrix4(0);

	ortho[0] = 2.0f / (right_ - left_);
	ortho[5] = 2.0f / (top_ - bottom_);
	ortho[10] = -2.0f / (far_ - near_);
	ortho[12] = -(right_ + left_) / (right_ - left_);
	ortho[13] = -(top_ + bottom_) / (top_ - bottom_);
	ortho[14] = -(far_ + near_) / (far_ - near_);
	ortho[15] = 1.0f;

	return ortho;
}

//The orthographic projection matrix is linear and affine but nothing else so there is has no inverse
//Therefore, it is labeled singular or non-invertable.
//I would still like to back transform from screen space to world space though
//Here's my unOrtho - It undoes what the orthographic matrix creates
//Multiply screen coordinates by this matrix and you should get x and y world coordinates
Matrix4 Matrix4::UnOrtho(const Matrix4& ortho){
	Matrix4 m;
	m[0] = 1.0f / ortho[0];
	m[5] = 1.0f/ ortho[5];
	m[10] = 1.0f / ortho[10];
	m[12] = -ortho[12] * m[0];
	m[13] = -ortho[13] * m[5];
	m[14] = -ortho[14] * m[10];
	m[15] = 1.0f;
	return m;
}

//Courtesy of Scott
Matrix4 Matrix4::Perspective(const float fov_, const float aspect_, const float nearPlane_, const float farPlane_){
	float cot = 1.0f / tan(Math::ConvertToRadians(fov_* 0.5f));
	//Don't forget, this looks row centric but it really is a column matrix - right-hand rule rules
	Matrix4 result(cot/aspect_, 0.0f,   0.0f,                             0.0f,
		0.0f,		  cot,	  0.0f, 	                        0.0f,
		0.0f,       0.0f,   (nearPlane_ + farPlane_)/(nearPlane_ - farPlane_),       -1.0,
		0.0,        0.0,    (2.0f*nearPlane_*farPlane_)/(nearPlane_-farPlane_),   0.0);
	return result;

	//Old one by Carter which probably doesn't work
	/*
	const float tanHalfFovy = tan(fov_ / 2.0f);

	Matrix4 perspective(0);
	perspective[0] = 1.0f / (aspect_ * tanHalfFovy);
	perspective[5] = 1.0f / (tanHalfFovy);
	perspective[10] = - (farPlane_ + nearPlane_) / (farPlane_ - nearPlane_);
	perspective[14] = - 1.0f;
	perspective[11] = - (2.0f * farPlane_ * nearPlane_) / (farPlane_ - nearPlane_);
	return perspective;
	*/
}

Matrix4 Matrix4::ViewportNDC(const int w, const int h){
	float minZ = 0.0f;
	float maxZ = 1.0f;

	Matrix4 m;
	Matrix4 m1 = Scale(Vector3(1.0f, -1.0f, 1.0f));
	Matrix4 m2 = Scale(Vector3(float(w) / 2.0f, float(h) / 2.0f, maxZ - minZ));
	Matrix4 m3 = Translate(Vector3(float(w) / 2.0f, float(h) / 2.0f, minZ));
	m = m3 * m2 * m1;

	///This is the slightly faster way but who cares we do it rarely 
	/***
	m[0] = float(w)/2.0f;
	m[5] = -float(h)/2.0f; 
	m[10] =  maxZ - minZ;
	m[12] = float(w)/2.0f;
	m[13] = float(h)/2.0f;
	m[14] = minZ;
	m[15] = 1.0f; ***/

	return m;
}

Matrix2 Matrix4::ToMatrix2() const{
	Matrix2 m_ = Matrix2::Identity();

	m_[0] = m[0];
	m_[1] = m[1];
	m_[2] = m[3];
	m_[3] = m[4];

	return m_;
}

Matrix2 Matrix4::ToMatrix2(const Matrix4& m_){
	return m_.ToMatrix2();
}

Matrix3 Matrix4::ToMatrix3() const{
	Matrix3 m_ = Matrix3::Identity();

	m_[0] = m[0];
	m_[1] = m[1];
	m_[2] = m[2];
	m_[3] = m[4];
	m_[4] = m[5];
	m_[5] = m[6];
	m_[6] = m[8];
	m_[7] = m[9];
	m_[8] = m[10];

	return m_;
}

Matrix3 Matrix4::ToMatrix3(const Matrix4& m_){
	return m_.ToMatrix3();
}

Euler Matrix4::ToEuler() const{
	return Matrix3::ToEuler(Matrix3(*this));
}

Euler Matrix4::ToEuler(const Matrix4& m_){
	return m_.ToEuler();
}

Quaternion Matrix4::ToQuaternion() const{
	return this->ToMatrix3().ToQuaternion();
}

Quaternion Matrix4::ToQuaternion(const Matrix4& m_){
	return m_.ToMatrix3().ToQuaternion();
}

std::string Matrix4::ToString() const{
	std::string mString;

	mString =	std::to_string(m[0]) + ", " + std::to_string(m[4]) + ", " + std::to_string(m[8]) + ", " + std::to_string(m[12]) + ",\n" +
				std::to_string(m[1]) + ", " + std::to_string(m[5]) + ", " + std::to_string(m[9]) + ", " + std::to_string(m[13]) + ",\n" +
				std::to_string(m[2]) + ", " + std::to_string(m[6]) + ", " + std::to_string(m[10]) + ", " + std::to_string(m[14]) + ",\n" + 
				std::to_string(m[3]) + ", " + std::to_string(m[7]) + ", " + std::to_string(m[11]) + ", " + std::to_string(m[15]);

	return mString;
}