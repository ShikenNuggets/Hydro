#ifndef VK_SHADER_H
#define VK_SHADER_H

#include <fstream>
#include <vector>

#include <vulkan/vulkan.h>

namespace Hydro{
	class VKShader{
	public:
		VKShader(VkDevice device_, const std::string& vertFile, const std::string& fragFile);
		~VKShader();

		VkShaderModule GetVertModule() const{ return vert; }
		VkShaderModule GetFragModule() const{ return frag; }

	private:
		VkDevice device;
		VkShaderModule vert;
		VkShaderModule frag;

		VkShaderModule CreateShaderModule(const std::vector<char>& code);
	};
}

#endif // !VK_SHADER_H