#ifndef VK_RENDERER_H
#define VK_RENDERER_H

#include "FileSystem.h"
#include "Window.h"

#include <iostream>
#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

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

		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class VKRenderer{
	public:
		VKRenderer(Window* window_);
		~VKRenderer();

		void Render();

		static void OnResize(int width, int height);

	private:
		Window* window;
		VkInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;
		VkPhysicalDevice physicalDevice;
		VkDevice device;
		VkQueue graphicsQueue;
		VkQueue presentQueue;
		VkSurfaceKHR surface;
		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		std::vector<VkImageView> swapChainImageViews;
		std::vector<VkFramebuffer> swapChainFramebuffers;
		VkRenderPass renderPass;
		VkPipelineLayout pipelineLayout;
		VkPipeline graphicsPipeline;
		VkCommandPool commandPool;
		std::vector<VkCommandBuffer> commandBuffers;
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		std::vector<VkFence> imagesInFlight;
		size_t currentFrame;
		static volatile bool windowResized; //TODO - This is a hack
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;

		static constexpr int MAX_FRAMES_IN_FLIGHT = 3;
		static const std::vector<const char*> validationLayers;
		static const std::vector<const char*> deviceExtensions;

		void CreateInstance();
		bool CheckValidationLayerSupport();
		void CreateDebugMessenger();
		void CreateSurface();
		void SelectPhysicalDevice();
		void CreateLogicalDevice();
		void CreateSwapChain();
		void CreateImageViews();
		void CreateRenderPass();
		void CreateGraphicsPipeline();
		void CreateFramebuffers();
		void CreateCommandPool();
		void CreateVertexBuffer();
		void CreateCommandBuffers();
		void CreateSyncObjects();

		void CleanupSwapChain();
		void RecreateSwapChain();

		std::vector<const char*> GetRequiredExtensions();
		bool IsDeviceSuitable(VkPhysicalDevice device_);
		bool DeviceSupportsExtensions(VkPhysicalDevice device_);

		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device_);
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device_);

		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		//Debug stuff
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData){
			std::cerr << "VK Validation Layer: " << pCallbackData->pMessage << std::endl; //TODO - Better message output (use severity, type, etc)
			return VK_FALSE;
		}
	};
}

#endif // !VK_RENDERER_H