#ifndef HYDRO_VIEWPORT_RECT_H
#define HYDRO_VIEWPORT_RECT_H

namespace Hydro{
	struct ViewportRect{
		static const ViewportRect fullScreen;
		static const ViewportRect top;
		static const ViewportRect bottom;
		static const ViewportRect left;
		static const ViewportRect right;
		static const ViewportRect topLeft;
		static const ViewportRect topRight;
		static const ViewportRect bottomLeft;
		static const ViewportRect bottomRight;

		float x;
		float y;
		float width;
		float height;

		ViewportRect(const float x_, const float y_, const float w_, const float h_);
		~ViewportRect();

		void CheckValidity();
	};
}

#endif //!VIEWPORT_RECT_H