#ifndef HYDRO_VERTEX_H
#define HYDRO_VERTEX_H

#include "GFX/Color.h"
#include "Math/Vector.h"

namespace Hydro{
	struct Vertex{
		Vertex(const Vector3& pos_, const Vector3& normal_, const Vector2& texCoords_) : pos(pos_), normal(normal_), texCoords(texCoords_){}

		Vector3 pos;
		Vector3 normal;
		Vector2 texCoords;
	};
}

#endif //!HYDRO_VERTEX_H