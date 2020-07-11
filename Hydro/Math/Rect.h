#ifndef RECT_H
#define RECT_H

#include <string>

namespace PizzaBox{
	struct Rect{
		float x;
		float y;
		float w;
		float h;
		
		Rect() : x(0.0f), y(0.0f), w(0.0f), h(0.0f){};
		Rect(float x_, float y_, float w_, float h_) : x(x_), y(y_), w(w_), h(h_){};

		inline bool Intersects(float x_, float y_){
			float halfWidth = w / 2.0f;
			float halfHeight = h / 2.0f;
			return (x_ > x - halfWidth && x_ < x + halfWidth) && (y_ < (1.0f - y) + halfHeight && y_ >(1.0f - y) - halfHeight);
		}
	};
}

#endif //!RECT_H