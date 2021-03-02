#ifndef HYDRO_GL_RENDER_INFO_H
#define HYDRO_GL_RENDER_INFO_H

#include "GFX/RenderInfo.h"
#include "GFX/OpenGL/GLVAO.h"
#include "GFX/OpenGL/GLBuffer.h"

namespace Hydro{
	class GLRenderInfo : public RenderInfo{
	public:
		GLRenderInfo(Model* model_, Texture* texture_);

		void Render();

	private:
		GLVAO vao;
		GLBuffer vbo;
		GLBuffer ebo;
	};
}

#endif //!HYDRO_GL_RENDER_INFO_H