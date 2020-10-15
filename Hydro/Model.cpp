#include "Model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

using namespace Hydro;

ModelLoader* ModelLoader::instance = nullptr;

Model::Model(const std::vector<VKVertex>& vertices_, const std::vector<uint32_t>& indices_) : Resource(), vertices(vertices_), indices(indices_){}

ModelLoader::ModelLoader(){
}

ModelLoader::~ModelLoader(){
}

ModelLoader* ModelLoader::Get(){
	if(instance == nullptr){
		instance = new ModelLoader();
	}

	return instance;
}

Model ModelLoader::LoadModel(const std::string& path_){
	std::vector<VKVertex> vertices;
	std::vector<uint32_t> indices;

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path_.c_str())){
		throw std::runtime_error(warn + err);
	}

	for(const auto& shape : shapes){
		for(const auto& index : shape.mesh.indices){
			VKVertex vertex{
				Vector3(attrib.vertices[3 * index.vertex_index + 0], attrib.vertices[3 * index.vertex_index + 1], attrib.vertices[3 * index.vertex_index + 2]),
				Vector3(1.0f, 1.0f, 1.0f),
				Vector2(attrib.texcoords[2 * index.texcoord_index + 0], 1.0f - attrib.texcoords[2 * index.texcoord_index + 1])
			};

			//TODO - vertex de-duplication
			vertices.push_back(vertex);
			indices.push_back(static_cast<uint32_t>(indices.size()));
		}
	}

	return Model(vertices, indices);
}