#ifndef HYDRO_VK_RENDERABLE_H
#define HYDRO_VK_RENDERABLE_H

#include <vulkan/vulkan.hpp>

#include "GFX/Model.h"
#include "GFX/Vulkan/VKBuffer.h"

namespace Hydro{
	class VKRenderable{
	public:
		VKRenderable(const std::string& modelName_);
		~VKRenderable();

	private:
		Model* model;
		VKBuffer* vertexBuffer;
		VKBuffer* indexBuffer;
		std::vector<VKBuffer*> uniformBuffers;
	};
}

#endif //!HYDRO_VK_RENDERABLE_H