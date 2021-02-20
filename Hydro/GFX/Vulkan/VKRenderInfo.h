#ifndef HYDRO_VK_RENDER_INFO_H
#define HYDRO_VK_RENDER_INFO_H

#include "GFX/MeshRenderer.h"
#include "GFX/RenderInfo.h"
#include "GFX/Vulkan/LowLevel/VKBuffer.h"
#include "GFX/Vulkan/LowLevel/VKImage.h"

namespace Hydro{
	class VKRenderInfo : public RenderInfo{
	public:
		VKRenderInfo(Model* model_, Texture* texture_, const vk::Device& device_, VKBuffer* vertexBuffer_, VKBuffer* indexBuffer_, VKImage* textureImage_);
		virtual ~VKRenderInfo() override;

	private:
		friend class VKRenderer;

		const vk::Device& device;

		VKBuffer* vertexBuffer;
		VKBuffer* indexBuffer;
		VKImage* image;
	};
}

#endif //!HYDRO_VK_RENDER_INFO_H