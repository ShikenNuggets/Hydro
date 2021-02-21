#include "VKImage.h"

#include "GFX/Vulkan/Core/Vulkan.h"

using namespace Hydro;

VKImage::VKImage(const vk::Device& device_, uint32_t w_, uint32_t h_, uint32_t mipLevels_, vk::SampleCountFlagBits numSamples, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties) : device(device_){
	vk::ImageCreateInfo imageInfo{};
	imageInfo.imageType = vk::ImageType::e2D;
	imageInfo.extent.width = w_;
	imageInfo.extent.height = h_;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = mipLevels_;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = vk::ImageLayout::eUndefined;
	imageInfo.usage = usage;
	imageInfo.samples = numSamples;
	imageInfo.sharingMode = vk::SharingMode::eExclusive;

	try{
		image = device.createImage(imageInfo);
	}catch(vk::SystemError err){
		throw std::runtime_error("Failed to create image! VK Error: " + std::string(err.what()));
	}

	vk::MemoryRequirements memRequirements = device.getImageMemoryRequirements(image);

	vk::MemoryAllocateInfo allocInfo{};
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = Vulkan::FindMemoryType(memRequirements.memoryTypeBits, properties);

	try{
		imageMemory = device.allocateMemory(allocInfo);
	}catch(vk::SystemError err){
		throw std::runtime_error("Failed to allocate image memory! VK Error: " + std::string(err.what()));
	}

	device.bindImageMemory(image, imageMemory, 0);
}

VKImage::~VKImage(){
	device.destroySampler(sampler);
	device.destroyImageView(imageView);
	device.destroyImage(image);
	device.freeMemory(imageMemory);
}