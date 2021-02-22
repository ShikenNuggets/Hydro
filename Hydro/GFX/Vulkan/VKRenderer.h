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
#include "GFX/Vulkan/VKRenderInfo.h"
#include "GFX/Vulkan/Core/VKBuffer.h"
#include "GFX/Vulkan/Core/VKImage.h"
#include "GFX/Vulkan/Core/VKPipeline.h"
#include "GFX/Vulkan/Core/VKState.h"

namespace Hydro{
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

	protected:
		virtual void CreateRenderInfo(MeshRenderer* mesh_) override;

	private:
		VKRenderInfo* Test_GetObjectRenderInfo();

		Window* window;
		
		vk::SwapchainKHR swapChain;
		std::vector<vk::Image> swapChainImages;
		vk::Format swapChainImageFormat;
		vk::Extent2D swapChainExtent;
		std::vector<vk::ImageView> swapChainImageViews;
		std::vector<vk::Framebuffer> swapChainFramebuffers;
		vk::RenderPass renderPass;
		vk::DescriptorSetLayout descriptorSetLayout;
		VKPipeline* graphicsPipeline;

		vk::CommandPool commandPool;
		std::vector<vk::CommandBuffer> commandBuffers;
		std::vector<vk::Semaphore> imageAvailableSemaphores;
		std::vector<vk::Semaphore> renderFinishedSemaphores;
		std::vector<vk::Fence> inFlightFences;
		std::vector<vk::Fence> imagesInFlight;
		size_t currentFrame;
		std::vector<VKBuffer*> uniformBuffers;
		vk::DescriptorPool descriptorPool;
		std::vector<vk::DescriptorSet> descriptorSets;
		uint32_t mipLevels;
		VKImage* depthImage;
		vk::SampleCountFlagBits msaaSamples;
		VKImage* colorImage;

		static constexpr int MAX_FRAMES_IN_FLIGHT = 3;
		static const std::vector<const char*> validationLayers;
		static const std::vector<const char*> deviceExtensions;
		static vk::PhysicalDeviceMemoryProperties memoryProperties;

		void CreateSwapChain();
		void CreateImageViews();
		void CreateRenderPass();
		void CreateDescriptorSetLayout();
		void CreateFramebuffers();
		void CreateCommandPool();
		void CreateColorResources();
		void CreateDepthResources();
		vk::ImageView CreateImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags, uint32_t mipLevels_);
		void GenerateMipmaps(VKImage* image, vk::Format imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels_);
		VKImage* CreateTextureImage(Texture* texture_);
		vk::Sampler CreateTextureSampler();
		void CopyBuffer(VKBuffer* sourceBuffer, VKBuffer* destBuffer, vk::DeviceSize size);
		VKBuffer* CreateVertexBuffer(const std::vector<VKVertex>& vertices_);
		VKBuffer* CreateIndexBuffer(const std::vector<uint32_t>& indices_);
		void CreateUniformBuffers();
		void CreateDescriptorPool();
		void CreateDescriptorSets();
		void CreateCommandBuffers();
		void CreateSyncObjects();

		void CleanupSwapChain();
		void RecreateSwapChain();

		vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
		vk::PresentModeKHR ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
		vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);

		vk::Format FindSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features);
		vk::Format FindDepthFormat();
		bool HasStencilComponent(vk::Format format);

		void UpdateUniformBuffer(uint32_t currentImage);

		vk::CommandBuffer BeginSingleTimeCommand();
		void EndSingleTimeCommand(vk::CommandBuffer commandBuffer);

		void TransitionImageLayout(VKImage* image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, uint32_t mipLevels_);
		void CopyBufferToImage(VKBuffer* buffer, VKImage* image, uint32_t width, uint32_t height);
	};
}

#endif // !VK_RENDERER_H