#include "VKPipeline.h"

#include "GFX/Vulkan/VKShader.h"
#include "GFX/Vulkan/VKVertex.h"

using namespace Hydro;

VKPipeline::VKPipeline(const vk::Device& device_, const std::string& vertShader_, const std::string& fragShader_, vk::Extent2D swapChainExtent_, vk::SampleCountFlagBits msaaSamples_, vk::DescriptorSetLayout descriptorSetLayout_, vk::RenderPass renderPass_) : device(device_){
	//Created on stack so this gets cleaned up automatically when we're done with it
	VKShader shader = VKShader(device, vertShader_, fragShader_);

	vk::PipelineShaderStageCreateInfo shaderStages[] = {
		vk::PipelineShaderStageCreateInfo(
			vk::PipelineShaderStageCreateFlags(),
			vk::ShaderStageFlagBits::eVertex,
			shader.GetVertModule(),
			"main"
		),

		vk::PipelineShaderStageCreateInfo(
			vk::PipelineShaderStageCreateFlags(),
			vk::ShaderStageFlagBits::eFragment,
			shader.GetFragModule(),
			"main"
		)
	};

	auto bindingDescription = VKVertex::GetBindingDescription();
	auto attributeDescription = VKVertex::GetAttributeDescription();

	vk::PipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription; //Optional
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescription.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescription.data(); //Optional

	vk::PipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	vk::Viewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChainExtent_.width;
	viewport.height = (float)swapChainExtent_.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	vk::Rect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent_;

	vk::PipelineViewportStateCreateInfo viewportState{};
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	vk::PipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = vk::PolygonMode::eFill;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = vk::CullModeFlagBits::eBack;
	rasterizer.frontFace = vk::FrontFace::eCounterClockwise;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; //Optional
	rasterizer.depthBiasClamp = 0.0f; //Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; //Optional

	vk::PipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sampleShadingEnable = VK_TRUE;
	multisampling.rasterizationSamples = msaaSamples_;
	multisampling.minSampleShading = 0.01f; //Optional
	multisampling.pSampleMask = nullptr; //Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; //Optional
	multisampling.alphaToOneEnable = VK_FALSE; //Optional

	vk::PipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = vk::CompareOp::eLess;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f; //Optional
	depthStencil.maxDepthBounds = 1.0f; //Optional
	depthStencil.stencilTestEnable = VK_FALSE;
	//depthStencil.front{}; //Optional
	//depthStencil.back{}; //Optional

	vk::PipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eOne; //Optional
	colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eZero; //Optional
	colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd; //Optional
	colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne; //Optional
	colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero; //Optional
	colorBlendAttachment.alphaBlendOp = vk::BlendOp::eAdd; //Optional

	vk::PipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = vk::LogicOp::eCopy; //Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; //Optional
	colorBlending.blendConstants[1] = 0.0f; //Optional
	colorBlending.blendConstants[2] = 0.0f; //Optional
	colorBlending.blendConstants[3] = 0.0f; //Optional

	vk::DynamicState dynamicStates[] = {
		vk::DynamicState::eViewport,
		vk::DynamicState::eLineWidth
	};

	vk::PipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.dynamicStateCount = 2;
	dynamicState.pDynamicStates = dynamicStates;

	vk::PipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.setLayoutCount = 1; //Optional
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout_; //Optional
	pipelineLayoutInfo.pushConstantRangeCount = 0; //Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; //Optional

	try{
		layout = device.createPipelineLayout(pipelineLayoutInfo);
	}
	catch(vk::SystemError err){
		throw std::runtime_error("Failed to create pipeline layout! VK Error: " + std::string(err.what()));
	}

	vk::GraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = nullptr; //Optional
	pipelineInfo.layout = layout;
	pipelineInfo.renderPass = renderPass_;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = nullptr; //Optional
	pipelineInfo.basePipelineIndex = -1; //Optional

	try{
		pipeline = device.createGraphicsPipeline(nullptr, pipelineInfo).value;
	}catch(vk::SystemError err){
		throw std::runtime_error("Failed to create graphics pipeline! VK Error: " + std::string(err.what()));
	}
}

VKPipeline::~VKPipeline(){
	device.destroyPipeline(pipeline);
	device.destroyPipelineLayout(layout);
}