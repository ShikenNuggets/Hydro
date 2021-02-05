#include "Renderer.h"

using namespace Hydro;

void Renderer::AddCamera(Camera* camera_){
	_ASSERT(std::find(cameras.begin(), cameras.end(), camera_) != cameras.end()); //Tried to add a camera that's already been added
	cameras.push_back(camera_);
}

void Renderer::AddMesh(MeshRenderer* mesh_){
	_ASSERT(std::find(meshes.begin(), meshes.end(), mesh_) != meshes.end()); //Tried to add a camera that's already been added
	meshes.push_back(mesh_);
}

void Renderer::RemoveCamera(Camera* camera_){
	_ASSERT(std::find(cameras.begin(), cameras.end(), camera_) != cameras.end()); //Tried to remove a camera that doesn't exist
	cameras.remove(camera_);
}

void Renderer::RemoveMesh(MeshRenderer* mesh_){
	_ASSERT(std::find(meshes.begin(), meshes.end(), mesh_) != meshes.end()); //Tried to remove a camera that doesn't exist
	meshes.remove(mesh_);
}