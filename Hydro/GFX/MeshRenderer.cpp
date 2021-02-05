#include "MeshRenderer.h"

#include "App.h"

using namespace Hydro;

MeshRenderer::MeshRenderer(const std::string& modelName_, const std::string& textureName_) : Component(){
	App::GetRenderer()->AddMesh(this);
}

MeshRenderer::~MeshRenderer(){
	App::GetRenderer()->RemoveMesh(this);
}