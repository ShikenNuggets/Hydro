#include "Vulkan.h"

#include "VKBuffer.h"

using namespace Hydro;

VKBuffer* Vulkan::CreateVertexBuffer(const std::vector<VKVertex>& vertices_){
	_ASSERT(vertices_.size() > 0);

	vk::DeviceSize bufferSize = sizeof(vertices_[0]) * vertices_.size();
	VKBuffer stagingBuffer = VKBuffer(device, bufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
	stagingBuffer.MapMemory(vertices_.data(), bufferSize);

	VKBuffer* vertexBuffer = new VKBuffer(device, bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal);
	CopyBuffer(stagingBuffer, vertexBuffer, bufferSize);

	return vertexBuffer;
}

void VKRenderer::CreateIndexBuffer(const std::vector<uint32_t>& indices_){
	vk::DeviceSize bufferSize = sizeof(model->indices[0]) * model->indices.size();

	vk::Buffer stagingBuffer;
	vk::DeviceMemory stagingBufferMemory;
	CreateBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, stagingBuffer, stagingBufferMemory);

	void* data = device->mapMemory(stagingBufferMemory, 0, bufferSize);
	memcpy(data, model->indices.data(), (size_t)bufferSize);
	device->unmapMemory(stagingBufferMemory);

	CreateBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, indexBuffer, indexBufferMemory);

	CopyBuffer(stagingBuffer, indexBuffer, bufferSize);

	device->destroyBuffer(stagingBuffer);
	device->freeMemory(stagingBufferMemory);
}

void VKRenderer::CreateUniformBuffers(){
	vk::DeviceSize bufferSize = sizeof(UniformBufferObject);

	uniformBuffers.resize(swapChainImages.size());
	uniformBuffersMemory.resize(swapChainImages.size());

	for(size_t i = 0; i < swapChainImages.size(); i++){
		CreateBuffer(bufferSize, vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, uniformBuffers[i], uniformBuffersMemory[i]);
	}
}

void Vulkan::CopyBuffer(vk::Buffer sourceBuffer, vk::Buffer destBuffer, vk::DeviceSize size){
	vk::CommandBuffer commandBuffer = BeginSingleTimeCommand();

	vk::BufferCopy copyRegion{};
	copyRegion.size = size;
	commandBuffer.copyBuffer(sourceBuffer, destBuffer, copyRegion);

	EndSingleTimeCommand(commandBuffer);
}

uint32_t Vulkan::FindMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties){
	auto memProperties = physicalDevice.getMemoryProperties();

	for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties){
			return i;
		}
	}

	throw std::runtime_error("Failed to find suitable VK memory type!");
}