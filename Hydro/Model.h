#ifndef HYDRO_MODEL_H
#define HYDRO_MODEL_H

#include "Resource.h"

#include <vector>

#include "Vulkan/VKVertex.h"

namespace Hydro{
	class Model : public Resource{
	public:
		Model(const std::vector<VKVertex>& vertices_, const std::vector<uint32_t>& indices_);

		std::vector<VKVertex> vertices;
		std::vector<uint32_t> indices;
	};

	class ModelLoader{
	public:
		static ModelLoader* Get();

		Model LoadModel(const std::string& path_);

	private:
		ModelLoader();
		~ModelLoader();

		static ModelLoader* instance;
	};
}

#endif //!HYDRO_MODEL_H