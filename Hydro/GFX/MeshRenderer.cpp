#include "MeshRenderer.h"

#include "App.h"

using namespace Hydro;

MeshRenderer::MeshRenderer(const std::string& modelName_, const std::string& textureName_) : Component(), model(nullptr), texture(nullptr){
	_ASSERT(!modelName_.empty() && !textureName_.empty());

	model = App::GetInstance()->GetResourceMgr()->Get<Model>(modelName_);
	texture = App::GetInstance()->GetResourceMgr()->Get<Texture>(textureName_);

	_ASSERT(model != nullptr && texture != nullptr);

	App::GetRenderer()->AddMesh(this);
}

MeshRenderer::~MeshRenderer(){
	App::GetRenderer()->RemoveMesh(this);

	delete renderInfo;
	renderInfo = nullptr;
}

void MeshRenderer::SetRenderInfo(RenderInfo* info_){
	_ASSERT(info_ != nullptr);
	renderInfo = info_;
}