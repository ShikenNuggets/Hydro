#include "ScreenCoordinate.h"

using namespace Hydro;

ScreenCoordinate::ScreenCoordinate(const unsigned int x_, const unsigned int y_) : x(x_), y(y_){
}

ScreenCoordinate::~ScreenCoordinate(){
}

bool ScreenCoordinate::operator ==(const ScreenCoordinate& sc) const{
	if(x == sc.x && y == sc.y){
		return true;
	}
	else{
		return false;
	}
}

bool ScreenCoordinate::operator !=(const ScreenCoordinate& sc) const{
	return !(*this == sc);
}

ScreenCoordinate ScreenCoordinate::operator +(const ScreenCoordinate& sc) const{
	return ScreenCoordinate(x + sc.x, y + sc.y);
}

ScreenCoordinate ScreenCoordinate::operator -(const ScreenCoordinate& sc) const{
	return ScreenCoordinate(x - sc.x, y - sc.y);
}

void ScreenCoordinate::operator +=(const ScreenCoordinate& sc){
	*this = *this + sc;
}

void ScreenCoordinate::operator -=(const ScreenCoordinate& sc){
	*this = *this - sc;
}