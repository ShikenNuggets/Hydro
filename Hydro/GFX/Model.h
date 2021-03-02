#ifndef HYDRO_MODEL_H
#define HYDRO_MODEL_H

#include <vector>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Resource.h"
#include "Vertex.h"

namespace Hydro{
	class Model : public Resource{
	public:
		Model(const std::vector<Vertex>& vertices_, const std::vector<uint32_t>& indices_);

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
	};

	class ModelLoader{
	public:
		static ModelLoader* Get();

		Model LoadModel(const std::string& path_);
		Model Test_LoadModel(const std::string& path_);

	private:
		ModelLoader();
		~ModelLoader();

		void ProcessNode(const aiNode* node, const aiScene* scene, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);

		static ModelLoader* instance;
	};
}

#endif //!HYDRO_MODEL_H