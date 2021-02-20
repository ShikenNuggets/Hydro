#include "VKRenderInfo.h"

using namespace Hydro;

VKRenderInfo::VKRenderInfo(Model* model_, Texture* texture_, const vk::Device& device_, VKBuffer* vertexBuffer_, VKBuffer* indexBuffer_, VKImage* textureImage_) : RenderInfo(model_, texture_), device(device_), vertexBuffer(vertexBuffer_), indexBuffer(indexBuffer_), image(textureImage_){
	_ASSERT(vertexBuffer_ != nullptr);
	_ASSERT(indexBuffer_ != nullptr);
	_ASSERT(textureImage_ != nullptr);
}

VKRenderInfo::~VKRenderInfo(){
	delete image;
	delete indexBuffer;
	delete vertexBuffer;
}