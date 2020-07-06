#include "VKShader.h"

#include <stdexcept>

#include "FileSystem.h"

using namespace Hydro;

VKShader::VKShader(VkDevice device_, const std::string& vertFile, const std::string& fragFile) : device(device_), vert(VK_NULL_HANDLE), frag(VK_NULL_HANDLE){
	_ASSERT(device != VK_NULL_HANDLE);
	_ASSERT(!vertFile.empty());
	_ASSERT(!fragFile.empty());

	auto vertShaderCode = FileSystem::ReadBinaryFile(vertFile);
	auto fragShaderCode = FileSystem::ReadBinaryFile(fragFile);

	vert = CreateShaderModule(vertShaderCode);
	frag = CreateShaderModule(fragShaderCode);
}

VKShader::~VKShader(){
	vkDestroyShaderModule(device, frag, nullptr);
	vkDestroyShaderModule(device, vert, nullptr);
}

VkShaderModule VKShader::CreateShaderModule(const std::vector<char>& code){
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data()); //This is gross

	VkShaderModule shaderModule;
	if(vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS){
		throw std::runtime_error("Failed to create shader module!");
	}

	return shaderModule;
}