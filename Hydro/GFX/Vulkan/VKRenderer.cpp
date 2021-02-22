#include "VKRenderer.h"

#include "App.h"
#include "GFX/Texture.h"
#include "ResourceMgr.h"
#include "Utils.h"
#include "VKShader.h"
#include "VKVertex.h"
#include "VKRenderInfo.h"
#include "Core/Vulkan.h"

#include <algorithm>
#include <chrono>
#include <exception>
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

using namespace Hydro;

vk::PhysicalDeviceMemoryProperties VKRenderer::memoryProperties;

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

VKRenderer::VKRenderer(Window* window_) : window(window_), swapChain(nullptr), swapChainImages(), currentFrame(0), msaaSamples(vk::SampleCountFlagBits::e1){
	Vulkan::state = new VKState(window_);
	
	msaaSamples = Vulkan::GetMaxUsableSampleCount();

	CreateSwapChain();
	CreateImageViews();
	CreateRenderPass();
	CreateDescriptorSetLayout();
	graphicsPipeline = new VKPipeline(Vulkan::Device(), "Resources/CompiledShaders/triangle-vert.spv", "Resources/CompiledShaders/triangle-frag.spv", swapChainExtent, msaaSamples, descriptorSetLayout, renderPass);
	CreateCommandPool();
	CreateColorResources();
	CreateDepthResources();
	CreateFramebuffers();
}

VKRenderer::~VKRenderer(){
	Vulkan::Device().waitIdle();

	CleanupSwapChain();

	Vulkan::Device().destroyDescriptorSetLayout(descriptorSetLayout);

	for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
		Vulkan::Device().destroySemaphore(renderFinishedSemaphores[i]);
		Vulkan::Device().destroySemaphore(imageAvailableSemaphores[i]);
		Vulkan::Device().destroyFence(inFlightFences[i]);
	}
	renderFinishedSemaphores.clear();
	imageAvailableSemaphores.clear();
	inFlightFences.clear();

	Vulkan::Device().destroyCommandPool(commandPool);

	delete Vulkan::state;
	Vulkan::state = nullptr;
}

void VKRenderer::Render(){
	Vulkan::Device().waitForFences(1, &inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

	uint32_t imageIndex;
	try{
		imageIndex = Vulkan::Device().acquireNextImageKHR(swapChain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphores[currentFrame], nullptr).value;
	}catch(vk::OutOfDateKHRError err){
		RecreateSwapChain();
		return;
	}catch(vk::SystemError err){
		throw std::runtime_error("Failed to acquire swap chain image! VK Error: " + std::string(err.what()));
	}

	if(imagesInFlight[imageIndex]){
		Vulkan::Device().waitForFences(1, &inFlightFences[imageIndex], VK_TRUE, std::numeric_limits<uint64_t>::max());
	}
	imagesInFlight[imageIndex] = inFlightFences[currentFrame];

	UpdateUniformBuffer(imageIndex);

	vk::SubmitInfo submitInfo{};

	vk::Semaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
	vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];
	vk::Semaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	Vulkan::Device().resetFences(1, &inFlightFences[currentFrame]);

	try{
		Vulkan::GraphicsQueue().submit(submitInfo, inFlightFences[currentFrame]);
	}catch(vk::SystemError err){
		throw std::runtime_error("Failed to submit draw command buffer! VK Error: " + std::string(err.what()));
	}

	vk::PresentInfoKHR presentInfo{};

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	vk::SwapchainKHR swapChains[] = { swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; //Optional
	
	vk::Result result;
	try{
		result = Vulkan::PresentQueue().presentKHR(presentInfo);
	}catch(vk::OutOfDateKHRError err){
		result = vk::Result::eErrorOutOfDateKHR;
	}catch(vk::SystemError err){
		throw std::runtime_error("Failed to present swap chain image! VK Error: " + std::string(err.what()));
	}

	//window->HandleEvents();
	if(result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || windowResized){
		windowResized = false;
		RecreateSwapChain();
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VKRenderer::CreateRenderInfo(MeshRenderer* mesh_){
	_ASSERT(mesh_ != nullptr);
	mesh_->SetRenderInfo(new VKRenderInfo(mesh_->GetModel(), mesh_->GetTexture(), Vulkan::Device(), CreateVertexBuffer(mesh_->GetModel()->vertices), CreateIndexBuffer(mesh_->GetModel()->indices), CreateTextureImage(mesh_->texture)));

	//TODO - This is not the right or best place to do any of this
	CreateUniformBuffers();
	CreateDescriptorPool();
	CreateDescriptorSets();
	CreateCommandBuffers();
	CreateSyncObjects();
}

void VKRenderer::CreateSwapChain(){
	SwapChainSupportDetails swapChainSupport = Vulkan::QuerySwapChainSupport(Vulkan::PhysicalDevice());
	vk::SurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
	vk::PresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
	vk::Extent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount){
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	auto createInfo = vk::SwapchainCreateInfoKHR(
		vk::SwapchainCreateFlagsKHR(),
		Vulkan::Surface(), imageCount, surfaceFormat.format, surfaceFormat.colorSpace, extent, 1, vk::ImageUsageFlagBits::eColorAttachment
	);

	QueueFamilyIndices indices = Vulkan::FindQueueFamilies(Vulkan::PhysicalDevice());
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if(indices.graphicsFamily != indices.presentFamily){
		createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}else{
		createInfo.imageSharingMode = vk::SharingMode::eExclusive;
		createInfo.queueFamilyIndexCount = 0; //Optional
		createInfo.pQueueFamilyIndices = nullptr; //Optional
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = nullptr;

	try{
		swapChain = Vulkan::Device().createSwapchainKHR(createInfo);
	}catch(vk::SystemError err){
		throw std::runtime_error("Failed to create VK swap chain! VK Error: " + std::string(err.what()));
	}

	swapChainImages = Vulkan::Device().getSwapchainImagesKHR(swapChain);

	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;
}

void VKRenderer::CreateImageViews(){
	swapChainImageViews.clear();
	swapChainImageViews.resize(swapChainImages.size());

	for(uint32_t i = 0; i < swapChainImages.size(); i++){
		swapChainImageViews[i] = CreateImageView(swapChainImages[i], swapChainImageFormat, vk::ImageAspectFlagBits::eColor, 1);
	}
}

void VKRenderer::CreateRenderPass(){
	vk::AttachmentDescription colorAttachment{};
	colorAttachment.format = swapChainImageFormat;
	colorAttachment.samples = msaaSamples;
	colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
	colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
	colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
	colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

	vk::AttachmentDescription depthAttachment{};
	depthAttachment.format = FindDepthFormat();
	depthAttachment.samples = msaaSamples;
	depthAttachment.loadOp = vk::AttachmentLoadOp::eClear;
	depthAttachment.storeOp = vk::AttachmentStoreOp::eDontCare;
	depthAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	depthAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	depthAttachment.initialLayout = vk::ImageLayout::eUndefined;
	depthAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

	vk::AttachmentDescription colorAttachmentResolve{};
	colorAttachmentResolve.format = swapChainImageFormat;
	colorAttachmentResolve.samples = vk::SampleCountFlagBits::e1; //TODO - Should this be msaaSamples?
	colorAttachmentResolve.loadOp = vk::AttachmentLoadOp::eDontCare;
	colorAttachmentResolve.storeOp = vk::AttachmentStoreOp::eStore;
	colorAttachmentResolve.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	colorAttachmentResolve.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	colorAttachmentResolve.initialLayout = vk::ImageLayout::eUndefined;
	colorAttachmentResolve.finalLayout = vk::ImageLayout::ePresentSrcKHR;

	vk::AttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

	vk::AttachmentReference depthAttachmentRef{};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

	vk::AttachmentReference colorAttachmentResolveRef{};
	colorAttachmentResolveRef.attachment = 2;
	colorAttachmentResolveRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

	vk::SubpassDescription subpass{};
	subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;
	subpass.pResolveAttachments = &colorAttachmentResolveRef;

	vk::SubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	//dependency.srcAccessMask = 0;
	dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;

	std::array<vk::AttachmentDescription, 3> attachments = { colorAttachment, depthAttachment, colorAttachmentResolve };
	vk::RenderPassCreateInfo renderPassInfo{};
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	try{
		renderPass = Vulkan::Device().createRenderPass(renderPassInfo);
	}catch(vk::SystemError err){
		throw std::runtime_error("Failed to create render pass! VK Error: " + std::string(err.what()));
	}
}

void VKRenderer::CreateDescriptorSetLayout(){
	vk::DescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = vk::DescriptorType::eUniformBuffer;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eVertex;
	uboLayoutBinding.pImmutableSamplers = nullptr; //Optional

	vk::DescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = vk::DescriptorType::eCombinedImageSampler;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eFragment;

	std::array<vk::DescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
	vk::DescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	try{
		descriptorSetLayout = Vulkan::Device().createDescriptorSetLayout(layoutInfo);
	}catch(vk::SystemError err){
		throw std::runtime_error("VK: Failed to create descriptor set layout! VK Error: " + std::string(err.what()));
	}
}

void VKRenderer::CreateFramebuffers(){
	swapChainFramebuffers.clear();
	swapChainFramebuffers.resize(swapChainImageViews.size());

	for(size_t i = 0; i < swapChainImageViews.size(); i++){
		std::array<vk::ImageView, 3> attachments = {
			colorImage->imageView,
			depthImage->imageView,
			swapChainImageViews[i]
		};

		vk::FramebufferCreateInfo framebufferInfo{};
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;

		try{
			swapChainFramebuffers[i] = Vulkan::Device().createFramebuffer(framebufferInfo);
		}catch(vk::SystemError err){
			throw std::runtime_error("Failed to create framebuffer! VK Error: " + std::string(err.what()));
		}
	}
}

void VKRenderer::CreateCommandPool(){
	QueueFamilyIndices queueFamilyIndices = Vulkan::FindQueueFamilies(Vulkan::PhysicalDevice());

	vk::CommandPoolCreateInfo poolInfo{};
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	try{
		commandPool = Vulkan::Device().createCommandPool(poolInfo);
	}catch(vk::SystemError err){
		throw std::runtime_error("Failed to create command pool! VK Error: " + std::string(err.what()));
	}
}

void VKRenderer::CreateColorResources(){
	colorImage = new VKImage(Vulkan::Device(), swapChainExtent.width, swapChainExtent.height, 1, msaaSamples, swapChainImageFormat, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eColorAttachment, vk::MemoryPropertyFlagBits::eDeviceLocal);
	colorImage->imageView = CreateImageView(colorImage->image, swapChainImageFormat, vk::ImageAspectFlagBits::eColor, 1); //TODO - There has to be a better way to create image views for these
}

void VKRenderer::CreateDepthResources(){
	vk::Format depthFormat = FindDepthFormat();
	depthImage = new VKImage(Vulkan::Device(), swapChainExtent.width, swapChainExtent.height, 1, msaaSamples, depthFormat, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eDepthStencilAttachment, vk::MemoryPropertyFlagBits::eDeviceLocal);
	depthImage->imageView = CreateImageView(depthImage->image, depthFormat, vk::ImageAspectFlagBits::eDepth, 1);

	TransitionImageLayout(depthImage, depthFormat, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal, 1);
}

vk::ImageView VKRenderer::CreateImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags, uint32_t mipLevels_){
	auto createInfo = vk::ImageViewCreateInfo();
	createInfo.image = image;
	createInfo.viewType = vk::ImageViewType::e2D;
	createInfo.format = format;
	createInfo.components.r = vk::ComponentSwizzle::eIdentity;
	createInfo.components.g = vk::ComponentSwizzle::eIdentity;
	createInfo.components.b = vk::ComponentSwizzle::eIdentity;
	createInfo.components.a = vk::ComponentSwizzle::eIdentity;
	createInfo.subresourceRange.aspectMask = aspectFlags;
	createInfo.subresourceRange.baseMipLevel = 0;
	createInfo.subresourceRange.levelCount = mipLevels_;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.layerCount = 1;

	try{
		return Vulkan::Device().createImageView(createInfo);
	}catch(vk::SystemError err){
		throw std::runtime_error("Failed to create image views! VK Error: " + std::string(err.what()));
	}
}

void VKRenderer::GenerateMipmaps(VKImage* image, vk::Format imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels_){
	//Check if image format supports linear blitting
	vk::FormatProperties formatProperties = Vulkan::PhysicalDevice().getFormatProperties(imageFormat);

	if(!(formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eSampledImageFilterLinear)){
		throw std::runtime_error("VK: Texture image format does not support linear blitting!");
	}

	vk::CommandBuffer commandBuffer = BeginSingleTimeCommand();

	vk::ImageMemoryBarrier barrier{};
	barrier.image = image->image;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.levelCount = 1;

	int32_t mipWidth = texWidth;
	int32_t mipHeight = texHeight;

	for(uint32_t i = 1; i < mipLevels_; i++){
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
		barrier.newLayout = vk::ImageLayout::eTransferSrcOptimal;
		barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;

		commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eTransfer, static_cast<vk::DependencyFlagBits>(0), nullptr, nullptr, barrier);

		vk::ImageBlit blit{};
		blit.srcOffsets[0] = { 0, 0, 0 };
		blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
		blit.srcSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
		blit.srcSubresource.mipLevel = i - 1;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount = 1;
		blit.dstOffsets[0] = { 0, 0, 0 };
		blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
		blit.dstSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
		blit.dstSubresource.mipLevel = i;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount = 1;

		commandBuffer.blitImage(image->image, vk::ImageLayout::eTransferSrcOptimal, image->image, vk::ImageLayout::eTransferDstOptimal, blit, vk::Filter::eLinear);

		barrier.oldLayout = vk::ImageLayout::eTransferSrcOptimal;
		barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
		barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

		commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, static_cast<vk::DependencyFlagBits>(0), nullptr, nullptr, barrier);

		if(mipWidth > 1) mipWidth /= 2;
		if(mipHeight > 1) mipHeight /= 2;
	}

	barrier.subresourceRange.baseMipLevel = mipLevels_ - 1;
	barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
	barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
	barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

	commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, static_cast<vk::DependencyFlagBits>(0), nullptr, nullptr, barrier);

	EndSingleTimeCommand(commandBuffer);
}

VKImage* VKRenderer::CreateTextureImage(Texture* texture_){
	_ASSERT(texture_ != nullptr);

	mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texture_->Width(), texture_->Height())))) + 1;
	VKBuffer stagingBuffer = VKBuffer(Vulkan::Device(), texture_->Size(), vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
	stagingBuffer.MapMemory(texture_->Pixels(), texture_->Size());

	VKImage* textureImage = new VKImage(Vulkan::Device(), texture_->Width(), texture_->Height(), mipLevels, vk::SampleCountFlagBits::e1, vk::Format::eR8G8B8A8Srgb, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal);

	TransitionImageLayout(textureImage, vk::Format::eR8G8B8A8Srgb, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, mipLevels);
	CopyBufferToImage(&stagingBuffer, textureImage, static_cast<uint32_t>(texture_->Width()), static_cast<uint32_t>(texture_->Height()));

	GenerateMipmaps(textureImage, vk::Format::eR8G8B8A8Srgb, texture_->Width(), texture_->Height(), mipLevels);

	textureImage->imageView = CreateImageView(textureImage->image, vk::Format::eR8G8B8A8Srgb, vk::ImageAspectFlagBits::eColor, mipLevels);
	textureImage->sampler = CreateTextureSampler();

	return textureImage;
}

vk::Sampler VKRenderer::CreateTextureSampler(){
	vk::SamplerCreateInfo samplerInfo{};
	samplerInfo.magFilter = vk::Filter::eLinear;
	samplerInfo.minFilter = vk::Filter::eLinear;
	samplerInfo.addressModeU = vk::SamplerAddressMode::eRepeat;
	samplerInfo.addressModeV = vk::SamplerAddressMode::eRepeat;
	samplerInfo.addressModeW = vk::SamplerAddressMode::eRepeat;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = 16.0f;
	samplerInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;	
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = vk::CompareOp::eAlways;
	samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = static_cast<float>(mipLevels);

	try{
		return Vulkan::Device().createSampler(samplerInfo);
	}catch(vk::SystemError err){
		throw std::runtime_error("Failed to create texture sampler! VK Error: " + std::string(err.what()));
	}
}

void VKRenderer::CopyBuffer(VKBuffer* sourceBuffer, VKBuffer* destBuffer, vk::DeviceSize size){
	_ASSERT(sourceBuffer != nullptr);
	_ASSERT(destBuffer != nullptr);

	vk::CommandBuffer commandBuffer = BeginSingleTimeCommand();

	vk::BufferCopy copyRegion{};
	copyRegion.size = size;
	commandBuffer.copyBuffer(sourceBuffer->buffer, destBuffer->buffer, copyRegion);

	EndSingleTimeCommand(commandBuffer);
}

VKBuffer* VKRenderer::CreateVertexBuffer(const std::vector<VKVertex>& vertices_){
	vk::DeviceSize bufferSize = sizeof(vertices_[0]) * vertices_.size();
	VKBuffer stagingBuffer = VKBuffer(Vulkan::Device(), bufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
	stagingBuffer.MapMemory(vertices_.data(), bufferSize);

	VKBuffer* vertexBuffer = new VKBuffer(Vulkan::Device(), bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal);
	CopyBuffer(&stagingBuffer, vertexBuffer, bufferSize);

	return vertexBuffer;
}

VKBuffer* VKRenderer::CreateIndexBuffer(const std::vector<uint32_t>& indices_){
	vk::DeviceSize bufferSize = sizeof(indices_[0]) * indices_.size();
	VKBuffer stagingBuffer = VKBuffer(Vulkan::Device(), bufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
	stagingBuffer.MapMemory(indices_.data(), bufferSize);

	VKBuffer* indexBuffer = new VKBuffer(Vulkan::Device(), bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal);
	CopyBuffer(&stagingBuffer, indexBuffer, bufferSize);

	return indexBuffer;
}

void VKRenderer::CreateUniformBuffers(){
	vk::DeviceSize bufferSize = sizeof(UniformBufferObject);

	uniformBuffers.clear();
	uniformBuffers.reserve(swapChainImages.size());
	for(size_t i = 0; i < swapChainImages.size(); i++){
		uniformBuffers.push_back(new VKBuffer(Vulkan::Device(), bufferSize, vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent));
	}
}

void VKRenderer::CreateDescriptorPool(){
	vk::DescriptorPoolSize poolSize{};
	poolSize.type = vk::DescriptorType::eUniformBuffer;
	poolSize.descriptorCount = static_cast<uint32_t>(swapChainImages.size());

	vk::DescriptorPoolCreateInfo poolInfo{};
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;
	poolInfo.maxSets = static_cast<uint32_t>(swapChainImages.size());

	try{
		descriptorPool = Vulkan::Device().createDescriptorPool(poolInfo);
	}catch(vk::SystemError err){
		throw std::runtime_error("Failed to create descriptor pool! VK Error: " + std::string(err.what()));
	}
}

void VKRenderer::CreateDescriptorSets(){
	//TODO - FOR TESTING PURPOSES ONLY
	auto renderInfo = Test_GetObjectRenderInfo();
	//Back to your regularly scheduled code

	std::vector<vk::DescriptorSetLayout> layouts(swapChainImages.size(), descriptorSetLayout);
	vk::DescriptorSetAllocateInfo allocInfo{};
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImages.size());
	allocInfo.pSetLayouts = layouts.data();

	try{
		descriptorSets = Vulkan::Device().allocateDescriptorSets(allocInfo);
	}catch(vk::SystemError err){
		throw std::runtime_error("Failed to allocate descriptor sets! VK Error: " + std::string(err.what()));
	}

	for(size_t i = 0; i < swapChainImages.size(); i++){
		vk::DescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers[i]->buffer;
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		vk::DescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		imageInfo.imageView = renderInfo->image->imageView;
		imageInfo.sampler = renderInfo->image->sampler;

		std::array<vk::WriteDescriptorSet, 2> descriptorWrites{};
		descriptorWrites[0].dstSet = descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = vk::DescriptorType::eUniformBuffer;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1].dstSet = descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = vk::DescriptorType::eCombinedImageSampler;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		Vulkan::Device().updateDescriptorSets(descriptorWrites, nullptr);
	}
}

void VKRenderer::CreateCommandBuffers(){
	//TODO - FOR TESTING PURPOSES ONLY
	auto renderInfo = Test_GetObjectRenderInfo();
	//Back to your regularly scheduled code

	commandBuffers.clear();
	commandBuffers.resize(swapChainFramebuffers.size());

	vk::CommandBufferAllocateInfo allocInfo{};
	allocInfo.commandPool = commandPool;
	allocInfo.level = vk::CommandBufferLevel::ePrimary;
	allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

	try{
		commandBuffers = Vulkan::Device().allocateCommandBuffers(allocInfo);
	}catch(vk::SystemError err){
		throw std::runtime_error("Failed to allocate command buffers! VK Error: " + std::string(err.what()));
	}

	for(size_t i = 0; i < commandBuffers.size(); i++){
		vk::CommandBufferBeginInfo beginInfo{};
		//beginInfo.flags = 0; //Optional
		beginInfo.pInheritanceInfo = nullptr; //Optional

		try{
			commandBuffers[i].begin(beginInfo);
		}catch(vk::SystemError err){
			throw std::runtime_error("Failed to begin recording command buffer! VK Error: " + std::string(err.what()));
		}

		vk::RenderPassBeginInfo renderPassInfo{};
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = swapChainFramebuffers[i];

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapChainExtent;

		std::array<vk::ClearValue, 2> clearValues{};
		clearValues[0].color = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		commandBuffers[i].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
			commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline->pipeline);
			vk::Buffer vertexBuffers[] = { renderInfo->vertexBuffer->buffer };
			vk::DeviceSize offsets[] = { 0 };
			commandBuffers[i].bindVertexBuffers(0, 1, vertexBuffers, offsets);
			commandBuffers[i].bindIndexBuffer(renderInfo->indexBuffer->buffer, 0, vk::IndexType::eUint32);
			commandBuffers[i].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, graphicsPipeline->layout, 0, descriptorSets[i], nullptr);
			commandBuffers[i].drawIndexed(static_cast<uint32_t>(renderInfo->model->indices.size()), 1, 0, 0, 0);
		commandBuffers[i].endRenderPass();

		try{
			commandBuffers[i].end();
		}catch(vk::SystemError err){
			throw std::runtime_error("Failed to record VK command buffer! VK Error: " + std::string(err.what()));
		}
	}
}

void VKRenderer::CreateSyncObjects(){
	imageAvailableSemaphores.clear();
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.clear();
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.clear();
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	imagesInFlight.clear();
	imagesInFlight.resize(swapChainImages.size(), nullptr);

	vk::FenceCreateInfo fenceInfo{};
	fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;

	try{
		for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
			imageAvailableSemaphores[i] = Vulkan::Device().createSemaphore({});
			renderFinishedSemaphores[i] = Vulkan::Device().createSemaphore({});
			inFlightFences[i] = Vulkan::Device().createFence({ vk::FenceCreateFlagBits::eSignaled });
		}
	}catch(vk::SystemError err){
		throw std::runtime_error("Failed to create VK sync objects for frame! VK Error: " + std::string(err.what()));
	}
}

void VKRenderer::CleanupSwapChain(){
	delete colorImage;
	delete depthImage;

	for(auto fb : swapChainFramebuffers){
		Vulkan::Device().destroyFramebuffer(fb);
	}
	swapChainFramebuffers.clear();

	Vulkan::Device().freeCommandBuffers(commandPool, commandBuffers);

	delete graphicsPipeline;
	graphicsPipeline = nullptr;

	Vulkan::Device().destroyRenderPass(renderPass);

	for(auto img : swapChainImageViews){
		Vulkan::Device().destroyImageView(img);
	}
	swapChainImageViews.clear();

	Vulkan::Device().destroySwapchainKHR(swapChain);

	for(VKBuffer* buffer : uniformBuffers){
		delete buffer;
	}
	uniformBuffers.clear();

	Vulkan::Device().destroyDescriptorPool(descriptorPool);
}

void VKRenderer::RecreateSwapChain(){
	//This might be a bad idea... but we can't create a swap chain for a minimized window :/ TODO - Get rid of this shitty hack
	window->HandleEvents();
	while(window->GetState() == Window::State::Minimized){
		window->HandleEvents();
	}

	Vulkan::Device().waitIdle();

	CleanupSwapChain();

	CreateSwapChain();
	CreateImageViews();
	CreateRenderPass();
	graphicsPipeline = new VKPipeline(Vulkan::Device(), "Resources/CompiledShaders/triangle-vert.spv", "Resources/CompiledShaders/triangle-frag.spv", swapChainExtent, msaaSamples, descriptorSetLayout, renderPass);
	CreateColorResources();
	CreateDepthResources();
	CreateFramebuffers();
	CreateUniformBuffers();
	CreateDescriptorPool();
	CreateDescriptorSets();
	CreateCommandBuffers();
}

vk::Format VKRenderer::FindSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features){
	for(vk::Format format : candidates){
		vk::FormatProperties props = Vulkan::PhysicalDevice().getFormatProperties(format);

		if(tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features){
			return format;
		}else if(tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features){
			return format;
		}
	}

	throw std::runtime_error("VK: Failed to find supported format!");
}

vk::Format VKRenderer::FindDepthFormat(){
	return FindSupportedFormat(
		{ vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint },
		vk::ImageTiling::eOptimal,
		vk::FormatFeatureFlagBits::eDepthStencilAttachment
	);
}

bool VKRenderer::HasStencilComponent(vk::Format format){
	return format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint;
}

void VKRenderer::UpdateUniformBuffer(uint32_t currentImage){
	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo{};
	//ubo.model = Matrix4::Rotate(180.0f, Vector3::worldUp); //Use this for static, facing "camera"
	ubo.model = Matrix4::Rotate(time * 90.0f, Vector3::worldForward); //Use this for spinning
	ubo.view = Matrix4::LookAt(Vector3(2.0f, 2.0f, 2.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3::worldUp);
	ubo.proj = Matrix4::Perspective(45.0f, swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
	//ubo.proj[5] *= -1;

	uniformBuffers[currentImage]->MapMemory(&ubo, sizeof(ubo));
}

vk::CommandBuffer VKRenderer::BeginSingleTimeCommand(){
	vk::CommandBufferAllocateInfo allocInfo{};
	allocInfo.level = vk::CommandBufferLevel::ePrimary;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	vk::CommandBuffer commandBuffer;
	try{
		commandBuffer = Vulkan::Device().allocateCommandBuffers(allocInfo)[0];
	}catch(vk::SystemError err){
		throw std::runtime_error("Failed to allocate command buffer! VK Error: " + std::string(err.what()));
	}

	vk::CommandBufferBeginInfo beginInfo{};
	beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

	commandBuffer.begin(beginInfo);
	return commandBuffer;
}

void VKRenderer::EndSingleTimeCommand(vk::CommandBuffer commandBuffer){
	commandBuffer.end();

	vk::SubmitInfo submitInfo{};
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	Vulkan::GraphicsQueue().submit(submitInfo, nullptr);
	Vulkan::GraphicsQueue().waitIdle();

	Vulkan::Device().freeCommandBuffers(commandPool, commandBuffer);
}

void VKRenderer::TransitionImageLayout(VKImage* image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, uint32_t mipLevels_){
	vk::CommandBuffer commandBuffer = BeginSingleTimeCommand();

	vk::ImageMemoryBarrier barrier{};
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image->image;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = mipLevels_;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	if(newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal) {
		barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;

		if(HasStencilComponent(format)){
			barrier.subresourceRange.aspectMask |= vk::ImageAspectFlagBits::eStencil;
		}
	}else{
		barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
	}

	vk::PipelineStageFlags sourceStage;
	vk::PipelineStageFlags destinationStage;
	if(oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal){
		barrier.srcAccessMask = static_cast<vk::AccessFlagBits>(0);
		barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

		sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
		destinationStage = vk::PipelineStageFlagBits::eTransfer;
	}else if(oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal){
		barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

		sourceStage = vk::PipelineStageFlagBits::eTransfer;
		destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
	}else if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal){
		barrier.srcAccessMask = static_cast<vk::AccessFlagBits>(0);
		barrier.dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;

		sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
		destinationStage = vk::PipelineStageFlagBits::eEarlyFragmentTests;
	}else{
		throw std::invalid_argument("Unsupported VK layout transition!");
	}

	commandBuffer.pipelineBarrier(sourceStage, destinationStage, static_cast<vk::DependencyFlagBits>(0), nullptr, nullptr, barrier);

	EndSingleTimeCommand(commandBuffer);
}

void VKRenderer::CopyBufferToImage(VKBuffer* buffer, VKImage* image, uint32_t width, uint32_t height){
	vk::CommandBuffer commandBuffer = BeginSingleTimeCommand();

	vk::BufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = { width, height, 1 };

	commandBuffer.copyBufferToImage(buffer->buffer, image->image, vk::ImageLayout::eTransferDstOptimal, region);

	EndSingleTimeCommand(commandBuffer);
}

vk::SurfaceFormatKHR VKRenderer::ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats){
	for(const auto& format : availableFormats){
		if(format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear){
			return format;
		}
	}

	_ASSERT(!availableFormats.empty());
	return availableFormats[0];
}

vk::PresentModeKHR VKRenderer::ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes){
	if(Utils::Has(availablePresentModes, vk::PresentModeKHR::eMailbox)){
		return vk::PresentModeKHR::eMailbox;
	}else if(Utils::Has(availablePresentModes, vk::PresentModeKHR::eImmediate)){
		return vk::PresentModeKHR::eImmediate;
	}
	
	return vk::PresentModeKHR::eFifo; //This is guaranteed to be supported
}

vk::Extent2D VKRenderer::ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities){
	if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()){
		return capabilities.currentExtent;
	}

	vk::Extent2D actualExtent = { static_cast<uint32_t>(window->Width()), static_cast<uint32_t>(window->Height()) };
	actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
	actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

	return actualExtent;
}

VKRenderInfo* VKRenderer::Test_GetObjectRenderInfo(){
	for(MeshRenderer* mesh : meshes){
		return dynamic_cast<VKRenderInfo*>(mesh->renderInfo);
	}

	return nullptr;
}