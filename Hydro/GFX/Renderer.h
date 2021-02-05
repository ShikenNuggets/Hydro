#ifndef HYDRO_RENDERER_H
#define HYDRO_RENDERER_H

#include <list>

#include "GFX/Camera.h"
#include "GFX/MeshRenderer.h"

namespace Hydro{
	class Renderer{
	public:
		virtual void Render() = 0;

		void AddCamera(Camera* camera_);
		void AddMesh(MeshRenderer* mesh_);

		void RemoveCamera(Camera* camera_);
		void RemoveMesh(MeshRenderer* mesh_);

	private:
		std::list<Camera*> cameras;
		std::list<MeshRenderer*> meshes;
	};
}

#endif //!HYDRO_RENDERER_H