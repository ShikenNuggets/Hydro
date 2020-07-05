#ifndef VK_SHADER_H
#define VK_SHADER_H

#include <fstream>
#include <vector>

#include <vulkan/vulkan.h>

namespace Hydro{
	class VKShader{
	public:
		VKShader(const std::string& vertFile, const std::string& fragFile);
		~VKShader();

		VkShaderModule GetShaderModule() const{ return shaderModule; }

		static std::vector<char> ReadFile(const std::string& fileName);

	private:
		VkShaderModule shaderModule;
	};
}

#endif // !VK_SHADER_H