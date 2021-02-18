#ifndef HYDRO_VULKAN_H
#define HYDRO_VULKAN_H

#include <vulkan/vulkan.hpp>

#include "GFX/Vulkan/VKBuffer.h"
#include "GFX/Vulkan/VKVertex.h"

namespace Hydro{
	class Vulkan{
	public:
		static VKBuffer* CreateVertexBuffer(const std::vector<VKVertex>& vertices_);
		static VKBuffer* CreateIndexBuffer(const std::vector<uint32_t>& indices_);
		static VKBuffer* CreateUniformBuffer();

		static uint32_t FindMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);

	private:
		static void CopyBuffer();
	};
}

#endif //!HYDRO_VULKAN_H