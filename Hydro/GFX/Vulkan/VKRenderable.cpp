#include "VKRenderable.h"

#include "App.h"
#include "GFX/Vulkan/Vulkan.h"

using namespace Hydro;

VKRenderable::VKRenderable(const std::string& modelName_) : model(nullptr), vertexBuffer(nullptr), indexBuffer(nullptr){
	model = App::GetInstance()->GetResourceMgr()->Get<Model>(modelName_);

	vertexBuffer = Vulkan::CreateVertexBuffer(model->vertices);
	indexBuffer = Vulkan::CreateIndexBuffer(model->indices);
}

VKRenderable::~VKRenderable(){
	delete vertexBuffer;
	delete indexBuffer;
}