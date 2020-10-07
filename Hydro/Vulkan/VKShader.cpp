#include "VKShader.h"

#include <stdexcept>

#include "FileSystem.h"

using namespace Hydro;

VKShader::VKShader(vk::Device device_, const std::string& vertFile, const std::string& fragFile) : device(device_), vert(nullptr), frag(nullptr){
	_ASSERT(device);
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

vk::ShaderModule VKShader::CreateShaderModule(const std::vector<char>& code){
	vk::ShaderModuleCreateInfo createInfo{ vk::ShaderModuleCreateFlags() };
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data()); //This is gross

	try{
		return device.createShaderModule(createInfo);
	}catch(vk::SystemError err){
		throw std::runtime_error("Failed to create shader module! VK Error: " + std::string(err.what()));
	}
}