#ifndef HYDRO_VK_BUFFER_H
#define HYDRO_VK_BUFFER_H

#include <vulkan/vulkan.hpp>

namespace Hydro{
	class VKBuffer{
	public:
		VKBuffer(const vk::Device& device_, vk::DeviceSize size_, vk::BufferUsageFlags usage_, vk::MemoryPropertyFlags properties_);
		~VKBuffer();

		void MapMemory(const void* data_, vk::DeviceSize bufferSize_);

	private:
		const vk::Device& device;
		vk::Buffer buffer;
		vk::DeviceMemory memory;
	};
}

#endif //!HYDRO_VK_BUFFER_H