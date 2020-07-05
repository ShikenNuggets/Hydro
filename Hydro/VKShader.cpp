#include "VKShader.h"

using namespace Hydro;

VKShader::VKShader(const std::string& vertFile, const std::string& fragFile){
}

VKShader::~VKShader(){
}

std::vector<char> VKShader::ReadFile(const std::string& fileName){
	std::ifstream file(fileName, std::ios::ate | std::ios::binary);
	if(!file.is_open()){
		throw std::exception("Failed to open file!");
	}

	size_t fileSize = static_cast<size_t>(file.tellg());
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}