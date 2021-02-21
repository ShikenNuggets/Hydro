#include "Vulkan.h"

using namespace Hydro;

VKState* Vulkan::state = nullptr;

uint32_t Vulkan::FindMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties){
	auto memoryProperties = PhysicalDevice().getMemoryProperties();

	for(uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
		if((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties){
			return i;
		}
	}

	throw std::runtime_error("Failed to find suitable VK memory type!");
}

QueueFamilyIndices Vulkan::FindQueueFamilies(const vk::PhysicalDevice& device_, const vk::SurfaceKHR& surface_){
	QueueFamilyIndices indices;

	int i = 0;
	for(const auto& queueFamily : device_.getQueueFamilyProperties()){
		if(queueFamily.queueFlags & vk::QueueFlagBits::eGraphics){
			indices.graphicsFamily = i;
		}

		if(device_.getSurfaceSupportKHR(i, surface_)){
			indices.presentFamily = i;
		}

		if(indices.IsComplete()){
			break;
		}

		i++;
	}

	return indices;
}

SwapChainSupportDetails Vulkan::QuerySwapChainSupport(const vk::PhysicalDevice& device_, const vk::SurfaceKHR& surface_){
	SwapChainSupportDetails details;
	details.capabilities = device_.getSurfaceCapabilitiesKHR(surface_);
	details.formats = device_.getSurfaceFormatsKHR(surface_);
	details.presentModes = device_.getSurfacePresentModesKHR(surface_);

	return details;
}

vk::SampleCountFlagBits Vulkan::GetMaxUsableSampleCount(const vk::PhysicalDevice& device_){
	vk::PhysicalDeviceProperties physicalDeviceProperties = device_.getProperties();
	vk::SampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;

	if(counts & vk::SampleCountFlagBits::e64){ return vk::SampleCountFlagBits::e64; }
	if(counts & vk::SampleCountFlagBits::e32){ return vk::SampleCountFlagBits::e32; }
	if(counts & vk::SampleCountFlagBits::e16){ return vk::SampleCountFlagBits::e16; }
	if(counts & vk::SampleCountFlagBits::e8){ return vk::SampleCountFlagBits::e8; }
	if(counts & vk::SampleCountFlagBits::e4){ return vk::SampleCountFlagBits::e4; }
	if(counts & vk::SampleCountFlagBits::e2){ return vk::SampleCountFlagBits::e2; }

	return vk::SampleCountFlagBits::e1;
}