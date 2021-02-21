#ifndef HYDRO_VK_STATE_H
#define HYDRO_VK_STATE_H

#include <iostream>

#include "Window.h"

namespace Hydro{
	class VKState{
	public:
		VKState(Window* window_);
		~VKState();

	private:
		friend class Vulkan;

		vk::UniqueInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;
		vk::PhysicalDevice physicalDevice;
		vk::UniqueDevice device;
		vk::Queue graphicsQueue;
		vk::Queue presentQueue;
		vk::SurfaceKHR surface;

		void CreateInstance(Window* window_);
		void SelectPhysicalDevice();
		void CreateLogicalDevice();

		std::vector<const char*> GetRequiredExtensions(Window* window_);

		bool IsDeviceSuitable(const vk::PhysicalDevice& device_);
		bool DeviceSupportsExtensions(const vk::PhysicalDevice& device_);

		#ifdef _DEBUG
		bool CheckValidationLayerSupport();
		void CreateDebugMessenger();

		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData){
			std::cerr << "VK Validation Layer: " << pCallbackData->pMessage << std::endl; //TODO - Better message output (use severity, type, etc)
			return VK_FALSE;
		}
		#endif //_DEBUG
	};
}

#endif //!HYDRO_VK_STATE_H