#ifndef HYDRO_MESH_RENDERER_H
#define HYDRO_MESH_RENDERER_H

#include <string>

#include "GFX/Model.h"
#include "GFX/RenderInfo.h"
#include "GFX/Texture.h"
#include "Objects/Component.h"

namespace Hydro{
	class MeshRenderer : public Component{
	public:
		MeshRenderer(const std::string& modelName_, const std::string& textureName_);
		~MeshRenderer();

		Model* GetModel() const{ return model; }
		Texture* GetTexture() const{ return texture; }

	private:
		friend class Renderer;
		friend class VKRenderer;

		Model* model;
		Texture* texture;
		RenderInfo* renderInfo;

		void SetRenderInfo(RenderInfo* info_);
	};
}

#endif //!HYDRO_MESH_RENDERER_H