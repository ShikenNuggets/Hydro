#ifndef HYDRO_VULKAN_H
#define HYDRO_VULKAN_H

#include <optional>

#include <vulkan/vulkan.hpp>

#include "VKState.h"

namespace Hydro{
	struct QueueFamilyIndices{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool IsComplete() const{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails{
		SwapChainSupportDetails() : capabilities(), formats(), presentModes(){}

		vk::SurfaceCapabilitiesKHR capabilities;
		std::vector<vk::SurfaceFormatKHR> formats;
		std::vector<vk::PresentModeKHR> presentModes;
	};

	class Vulkan{
	public:
		static const vk::Instance& Instance(){ _ASSERT(state); return state->instance.get(); }
		static const vk::PhysicalDevice& PhysicalDevice(){ _ASSERT(state); return state->physicalDevice; }
		static const vk::Device& Device(){ _ASSERT(state); return state->device.get(); }
		static const vk::Queue& GraphicsQueue(){ _ASSERT(state); return state->graphicsQueue; }
		static const vk::Queue& PresentQueue(){ _ASSERT(state); return state->presentQueue; }
		static const vk::SurfaceKHR& Surface(){ _ASSERT(state); return state->surface; }

		static uint32_t FindMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);
		static QueueFamilyIndices FindQueueFamilies(const vk::PhysicalDevice& device_ = Vulkan::PhysicalDevice(), const vk::SurfaceKHR& surface_ = Vulkan::Surface());
		static SwapChainSupportDetails QuerySwapChainSupport(const vk::PhysicalDevice& device_ = Vulkan::PhysicalDevice(), const vk::SurfaceKHR& surface_ = Vulkan::Surface());
		static vk::SampleCountFlagBits GetMaxUsableSampleCount(const vk::PhysicalDevice& device_ = Vulkan::PhysicalDevice());

		static vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
		static vk::PresentModeKHR ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
		static vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, Window* window_);

	private:
		friend class VKRenderer;
		static VKState* state;
	};
}

#endif //!HYDRO_VULKAN_H