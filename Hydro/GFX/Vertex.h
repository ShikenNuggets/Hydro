#ifndef HYDRO_VERTEX_H
#define HYDRO_VERTEX_H

#include <array>

#include "GFX/Color.h"
#include "Math/Vector.h"

namespace Hydro{
	struct Vertex{
		Vertex(const Vector3& pos_, const Color& color_, const Vector2& texCoords_) : pos(pos_), color(color_), texCoords(texCoords_){}

		Vector3 pos;
		Color color;
		Vector2 texCoords;
	};
}

#endif //!VK_VERTEX_H