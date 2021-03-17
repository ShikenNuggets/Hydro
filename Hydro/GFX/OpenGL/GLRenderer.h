#ifndef HYDRO_GL_RENDERER_H
#define HYDRO_GL_RENDERER_H

#include <GL/glew.h>

#include "GFX/Renderer.h"
#include "GFX/OpenGL/GLMainFBO.h"
#include "GFX/OpenGL/GLShader.h"
#include "GFX/OpenGL/GLVAO.h"
#include "GFX/OpenGL/GLBuffer.h"

namespace Hydro{
	class GLRenderer : public Renderer{
	public:
		GLRenderer(Window* window_);
		~GLRenderer();

		virtual void Render() override;

	protected:
		virtual void CreateRenderInfo(MeshRenderer* mesh_) override;
		virtual void OnResize(int width_, int height_) override;

	private:
		std::string sharedShaderName;
		std::string sharedShaderCode;
		GLMainFBO* mainFBO;

		GLShader* postShader;
		GLVAO* postVAO;
		GLBuffer* postVBO;

		void ClearScreen();
		void SetClearColor(const Color& color_);
		void SetViewport(const ViewportRect& v_);

		static void __stdcall GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
	};
}

#endif //!HYDRO_GL_RENDERER_H