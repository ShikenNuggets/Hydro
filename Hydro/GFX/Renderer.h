#ifndef HYDRO_RENDERER_H
#define HYDRO_RENDERER_H

#include <list>

#include "Window.h"
#include "GFX/Camera.h"
#include "GFX/Color.h"
#include "GFX/MeshRenderer.h"

namespace Hydro{
	class Renderer{
	public:
		Renderer(Window* window_) : window(window_){
			_ASSERT(window != nullptr);
		}

		virtual void Render() = 0;

		void AddCamera(Camera* camera_);
		void AddMesh(MeshRenderer* mesh_);

		void RemoveCamera(Camera* camera_);
		void RemoveMesh(MeshRenderer* mesh_);

		virtual void OnResize(int width, int height);

	protected:
		Window* window;
		std::list<Camera*> cameras;
		std::list<MeshRenderer*> meshes;

		volatile bool windowResized; //TODO - This is a hack

		virtual void CreateRenderInfo(MeshRenderer* mesh_){}
	};
}

#endif //!HYDRO_RENDERER_H