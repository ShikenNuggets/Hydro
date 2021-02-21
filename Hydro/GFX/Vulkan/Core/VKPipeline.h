#ifndef HYDRO_VK_PIPELINE_H
#define HYDRO_VK_PIPELINE_H

#include <vulkan/vulkan.hpp>

namespace Hydro{
	class VKPipeline{
	public:
		VKPipeline(const vk::Device& device_, const std::string& vertShader_, const std::string& fragShader_, vk::Extent2D swapChainExtent_, vk::SampleCountFlagBits msaaSamples_, vk::DescriptorSetLayout descriptorSetLayout_, vk::RenderPass renderPass_);
		~VKPipeline();

	private:
		friend class VKRenderer;

		const vk::Device& device;
		vk::PipelineLayout layout;
		vk::Pipeline pipeline;
	};
}

#endif //!HYDRO_VK_PIPELINE_H