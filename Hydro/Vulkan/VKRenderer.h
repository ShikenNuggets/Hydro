#ifndef VK_RENDERER_H
#define VK_RENDERER_H

#include <iostream>
#include <optional>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "VKVertex.h"
#include "Window.h"
#include "GFX/Renderer.h"
#include "GFX/Model.h"

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

	struct UniformBufferObject{
		//Things in this structure should be explicitly aligned in case we modify it
		//If we don't rendering could break entirely
		alignas(16) Matrix4 model;
		alignas(16) Matrix4 view;
		alignas(16) Matrix4 proj;
	};

	class VKRenderer : public Renderer{
	public:
		VKRenderer(Window* window_);
		~VKRenderer();

		void Render();

		static void OnResize(int width, int height);

	private:
		Window* window;
		vk::UniqueInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;
		vk::PhysicalDevice physicalDevice;
		vk::UniqueDevice device;
		vk::Queue graphicsQueue;
		vk::Queue presentQueue;
		vk::SurfaceKHR surface;
		vk::SwapchainKHR swapChain;
		std::vector<vk::Image> swapChainImages;
		vk::Format swapChainImageFormat;
		vk::Extent2D swapChainExtent;
		std::vector<vk::ImageView> swapChainImageViews;
		std::vector<vk::Framebuffer> swapChainFramebuffers;
		vk::RenderPass renderPass;
		vk::DescriptorSetLayout descriptorSetLayout;
		vk::PipelineLayout pipelineLayout;
		vk::Pipeline graphicsPipeline;
		vk::CommandPool commandPool;
		std::vector<vk::CommandBuffer> commandBuffers;
		std::vector<vk::Semaphore> imageAvailableSemaphores;
		std::vector<vk::Semaphore> renderFinishedSemaphores;
		std::vector<vk::Fence> inFlightFences;
		std::vector<vk::Fence> imagesInFlight;
		size_t currentFrame;
		static volatile bool windowResized; //TODO - This is a hack
		Model* model;
		vk::Buffer vertexBuffer;
		vk::DeviceMemory vertexBufferMemory;
		vk::Buffer indexBuffer;
		vk::DeviceMemory indexBufferMemory;
		std::vector<vk::Buffer> uniformBuffers;
		std::vector<vk::DeviceMemory> uniformBuffersMemory;
		vk::DescriptorPool descriptorPool;
		std::vector<vk::DescriptorSet> descriptorSets;
		uint32_t mipLevels;
		vk::Image textureImage;
		vk::DeviceMemory textureImageMemory;
		vk::ImageView textureImageView;
		vk::Sampler textureSampler;
		vk::Image depthImage;
		vk::DeviceMemory depthImageMemory;
		vk::ImageView depthImageView;
		vk::SampleCountFlagBits msaaSamples;
		vk::Image colorImage;
		vk::DeviceMemory colorImageMemory;
		vk::ImageView colorImageView;

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
		void CreateDescriptorSetLayout();
		void CreateGraphicsPipeline();
		void CreateFramebuffers();
		void CreateCommandPool();
		void CreateColorResources();
		void CreateDepthResources();
		void CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels_, vk::SampleCountFlagBits numSamples, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Image& image, vk::DeviceMemory& imageMemory);
		vk::ImageView CreateImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags, uint32_t mipLevels_);
		void GenerateMipmaps(vk::Image image, vk::Format imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels_);
		void CreateTextureImage();
		void CreateTextureImageView();
		void CreateTextureSampler();
		void CreateBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Buffer& buffer, vk::DeviceMemory& bufferMemory);
		void CopyBuffer(vk::Buffer sourceBuffer, vk::Buffer destBuffer, vk::DeviceSize size);
		void CreateVertexBuffer();
		void CreateIndexBuffer();
		void CreateUniformBuffers();
		void CreateDescriptorPool();
		void CreateDescriptorSets();
		void CreateCommandBuffers();
		void CreateSyncObjects();

		void CleanupSwapChain();
		void RecreateSwapChain();

		std::vector<const char*> GetRequiredExtensions();
		bool IsDeviceSuitable(const vk::PhysicalDevice& device_);
		bool DeviceSupportsExtensions(const vk::PhysicalDevice& device_);

		QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice device_);
		SwapChainSupportDetails QuerySwapChainSupport(const vk::PhysicalDevice& device_);

		vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
		vk::PresentModeKHR ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
		vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);

		uint32_t FindMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);
		vk::Format FindSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features);
		vk::Format FindDepthFormat();
		bool HasStencilComponent(vk::Format format);

		void UpdateUniformBuffer(uint32_t currentImage);

		vk::CommandBuffer BeginSingleTimeCommand();
		void EndSingleTimeCommand(vk::CommandBuffer commandBuffer);

		void TransitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, uint32_t mipLevels_);
		void CopyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height);

		vk::SampleCountFlagBits GetMaxUsableSampleCount();

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