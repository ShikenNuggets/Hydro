#ifndef HYDRO_RENDER_INFO_H
#define HYDRO_RENDER_INFO_H

#include "GFX/Model.h"
#include "GFX/Texture.h"

namespace Hydro{
	class RenderInfo{
	public:
		RenderInfo(Model* model_, Texture* texture_) : model(model_), texture(texture_){}
		virtual ~RenderInfo(){}

	protected:
		friend class Renderer;
		friend class VKRenderer;

		Model* model;
		Texture* texture;
	};
}

#endif //!HYDRO_RENDER_INFO_H