#include "VKBuffer.h"

#include "GFX/Vulkan/Core/Vulkan.h"

using namespace Hydro;

VKBuffer::VKBuffer(const vk::Device& device_, vk::DeviceSize size_, vk::BufferUsageFlags usage_, vk::MemoryPropertyFlags properties_) : device(device_){
	vk::BufferCreateInfo bufferInfo{};
	bufferInfo.size = size_;
	bufferInfo.usage = usage_;
	bufferInfo.sharingMode = vk::SharingMode::eExclusive;

	try{
		buffer = device.createBuffer(bufferInfo);
	}catch(vk::SystemError err){
		throw std::runtime_error("Failed to create vertex buffer! VK Error: " + std::string(err.what()));
	}

	vk::MemoryRequirements memRequirements = device.getBufferMemoryRequirements(buffer);

	vk::MemoryAllocateInfo allocInfo{};
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = Vulkan::FindMemoryType(memRequirements.memoryTypeBits, properties_);

	try{
		memory = device.allocateMemory(allocInfo);
	}catch(vk::SystemError err){
		throw std::runtime_error("Failed to allocate vertex buffer memory! VK Error: " + std::string(err.what()));
	}

	device.bindBufferMemory(buffer, memory, 0);
}

VKBuffer::~VKBuffer(){
	device.destroyBuffer(buffer);
	device.freeMemory(memory);
}

void VKBuffer::MapMemory(const void* data_, vk::DeviceSize bufferSize_){
	void* data = device.mapMemory(memory, 0, bufferSize_);
		memcpy(data, data_, static_cast<size_t>(bufferSize_));
	device.unmapMemory(memory);
}