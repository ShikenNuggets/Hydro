#include "Sphere.h"

using namespace PizzaBox;

Sphere::Sphere(const float x_, const float y_, const float z_, const float r_) : point(x_, y_, z_), radius(r_){
}

Sphere::Sphere(const Vector3& v_, const float r_) : point(v_), radius(r_){
}

Sphere::~Sphere(){
}

std::string Sphere::ToString(){
	return std::string(point.ToString() + ", " + std::to_string(radius));
}