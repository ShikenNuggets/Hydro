#ifndef HYDRO_GL_RENDERER_H
#define HYDRO_GL_RENDERER_H

#include "GFX/Renderer.h"

namespace Hydro{
	class GLRenderer : public Renderer{
	public:
		GLRenderer();
		~GLRenderer();

		virtual void Render() override;

	protected:
		virtual void CreateRenderInfo(MeshRenderer* mesh_) override;
	};
}

#endif //!HYDRO_GL_RENDERER_H