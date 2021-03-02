#include "Model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

using namespace Hydro;

ModelLoader* ModelLoader::instance = nullptr;

Model::Model(const std::vector<Vertex>& vertices_, const std::vector<uint32_t>& indices_) : Resource(), vertices(vertices_), indices(indices_){}

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
	std::vector<Vertex> vertices;
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
			Vertex vertex{
				Vector3(attrib.vertices[3 * index.vertex_index + 0], attrib.vertices[3 * index.vertex_index + 1], attrib.vertices[3 * index.vertex_index + 2]),
				Color(1.0f, 1.0f, 1.0f),
				Vector2(attrib.texcoords[2 * index.texcoord_index + 0], 1.0f - attrib.texcoords[2 * index.texcoord_index + 1])
			};

			//TODO - vertex de-duplication
			vertices.push_back(vertex);
			indices.push_back(static_cast<uint32_t>(indices.size()));
		}
	}

	return Model(vertices, indices);
}

Model ModelLoader::Test_LoadModel(const std::string& path_){
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path_, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
		//Debug::LogError("AssImp could not load model! AssImp Error: " + std::string(importer.GetErrorString()));
		//Return the empty mesh list so that we know an error has occured
		return Model(vertices, indices);
	}

	//Recursively process every node of the AssImp scene
	ProcessNode(scene->mRootNode, scene, vertices, indices);
	return Model(vertices, indices);
}

void ModelLoader::ProcessNode(const aiNode* node, const aiScene* scene, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices){
	for(unsigned int i = 0; i < node->mNumMeshes; i++){
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		for(unsigned int j = 0; j < mesh->mNumVertices; j++){
			vertices.push_back(Vertex(
				Vector3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z),
				Color(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z),
				Vector2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y)
			));
		}

		for(unsigned int j = 0; j < mesh->mNumFaces; j++){
			for(unsigned int k = 0; k < mesh->mFaces[j].mNumIndices; k++){
				indices.push_back(mesh->mFaces[j].mIndices[k]);
			}
		}
	}

	for(unsigned int i = 0; i < node->mNumChildren; i++){
		ProcessNode(node->mChildren[i], scene, vertices, indices);
	}
}