#ifndef HYDRO_GL_RENDER_INFO_H
#define HYDRO_GL_RENDER_INFO_H

#include "GFX/RenderInfo.h"
#include "GFX/OpenGL/GLVAO.h"
#include "GFX/OpenGL/GLBuffer.h"
#include "GFX/OpenGL/GLShader.h"
#include "GFX/OpenGL/GLTexture.h"

namespace Hydro{
	class GLRenderInfo : public RenderInfo{
	public:
		GLRenderInfo(Model* model_, Texture* texture_);

		void Render(const Matrix4& model_, const Matrix4& view_, const Matrix4& proj_);

	private:
		GLVAO vao;
		GLBuffer vbo;
		GLBuffer ebo;
		GLTexture texture;
		GLShader shader;
	};
}

#endif //!HYDRO_GL_RENDER_INFO_H