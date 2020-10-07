#ifndef VK_SHADER_H
#define VK_SHADER_H

#include <string>
#include <vector>

#include <vulkan/vulkan.hpp>

namespace Hydro{
	class VKShader{
	public:
		VKShader(vk::Device device_, const std::string& vertFile, const std::string& fragFile);
		~VKShader();

		vk::ShaderModule GetVertModule() const{ return vert; }
		vk::ShaderModule GetFragModule() const{ return frag; }

	private:
		vk::Device device;
		vk::ShaderModule vert;
		vk::ShaderModule frag;

		vk::ShaderModule CreateShaderModule(const std::vector<char>& code);
	};
}

#endif // !VK_SHADER_H