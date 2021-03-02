#ifndef HYDRO_COLOR_H
#define HYDRO_COLOR_H

#include "Math/Vector.h"

namespace Hydro{
	struct Color{
		Color(const float r_, const float g_, const float b_, const float a_ = 1.0f);
		explicit Color(const Vector3& rgb);
		explicit Color(const Vector4& rgba);

		//Red, green, blue, and alpha values
		//These values should always be between 0 and 1, since they're effectively used as percentages
		float r, g, b, a;

		//Default color values for convenience
		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color Yellow;
		static const Color Orange;
		static const Color Purple;
		static const Color Black;
		static const Color White;
		static const Color Brown;
		static const Color Gray;
		static const Color DarkGray;

		//Type conversion operators, courtesy of Scott
		operator const float* () const;
		operator float* ();
	};
}

#endif //!HYDRO_COLOR_H