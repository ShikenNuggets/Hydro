#ifndef HYDRO_VK_IMAGE_H
#define HYDRO_VK_IMAGE_H

#include "VKBuffer.h"
#include "GFX/Texture.h"

namespace Hydro{
	class VKImage{
	public:
		VKImage(const vk::Device& device_, uint32_t w_, uint32_t h_, uint32_t mipLevels_, vk::SampleCountFlagBits numSamples, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties);
		~VKImage();

	private:
		friend class VKRenderer;

		const vk::Device& device;
		vk::Image image;
		vk::DeviceMemory imageMemory;
		vk::ImageView imageView;
		vk::Sampler sampler;
	};
}

#endif //!HYDRO_VK_IMAGE_H