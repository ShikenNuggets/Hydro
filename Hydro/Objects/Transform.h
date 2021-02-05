#ifndef HYDRO_TRANSFORM_H
#define HYDRO_TRANSFORM_H

#include "Math/Euler.h"
#include "Math/Quaternion.h"
#include "Math/Vector.h"

namespace Hydro{
	class Transform{
	public:
		Vector3 position;
		Quaternion rotation;
		Vector3 scale;

		Matrix4 GetTransformation() const;
	};
}

#endif //!HYDRO_TRANSFORM_H