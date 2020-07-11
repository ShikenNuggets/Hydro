#include "Plane.h"

using namespace PizzaBox;

Plane::Plane(const float x_, const float y_, const float z_, const float d_) : point(x_, y_, z_), d(d_){
}

Plane::Plane(const Vector3& v_, const float d_) : point(v_), d(d_){
}

Plane::Plane(const Vector3& v0, const Vector3& v1, const Vector3& v2){
	const Vector3 a = v1 - v0; 
	const Vector3 b = v2 - v0;
	const Vector3 n = Vector3::Cross(a, b);
	*this = Plane(n.Normalized(), Vector3::Dot(a, b));
}

Plane::~Plane(){
}

Plane Plane::Normalized() const{
	const float magnitude = point.Magnitude();
	return Plane(point.Normalized(), d / magnitude);
}

void Plane::Normalize(){
	*this = Normalized();
}

Plane Plane::Normalized(const Plane& p_){
	return p_.Normalized();
}

std::string Plane::ToString(){
	return std::string(point.ToString() + ", " + std::to_string(d));
}