#include "Color.h"

using namespace Hydro;

const Color Color::Red = Color(1.0f, 0.0f, 0.0f, 1.0f);
const Color Color::Green = Color(0.0f, 1.0f, 0.0f, 1.0f);
const Color Color::Blue = Color(0.0f, 0.0f, 1.0f, 1.0f);
const Color Color::Yellow = Color(1.0f, 1.0f, 0.0f, 1.0f);
const Color Color::Orange = Color(1.0f, 0.5f, 0.0f, 1.0f);
const Color Color::Purple = Color(0.4f, 0.0f, 0.8f, 1.0f);
const Color Color::Black = Color(0.0f, 0.0f, 0.0f, 1.0f);
const Color Color::White = Color(1.0f, 1.0f, 1.0f, 1.0f);
const Color Color::Brown = Color(0.4f, 0.2f, 0.0f, 1.0f);
const Color Color::Gray = Color(0.5f, 0.5f, 0.5f, 1.0f);
const Color Color::DarkGray = Color(0.2f, 0.2f, 0.2f, 1.0f);

Color::Color(const float r_, const float g_, const float b_, const float a_) : r(r_), g(g_), b(b_), a(a_){}

Color::Color(const Vector3& rgb) : r(rgb.x), g(rgb.y), b(rgb.z), a(1.0f){}

Color::Color(const Vector4& rgba) : r(rgba.x), g(rgba.y), b(rgba.z), a(rgba.w){}

Color::operator const float* () const{
	return static_cast<const float*>(&r);
}

Color::operator float* (){
	return static_cast<float*>(&r);
}