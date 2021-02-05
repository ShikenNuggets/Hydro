#ifndef HYDRO_MESH_RENDERER_H
#define HYDRO_MESH_RENDERER_H

#include <string>

#include "Objects/Component.h"

namespace Hydro{
	class MeshRenderer : public Component{
	public:
		MeshRenderer(const std::string& modelName_, const std::string& textureName_);
		~MeshRenderer();
	};
}

#endif //!HYDRO_MESH_RENDERER_H