#ifndef HYDRO_GL_VAO_H
#define HYDRO_GL_VAO_H

#include <GL/glew.h>

namespace Hydro{
	class GLVAO{
	public:
		GLVAO();
		~GLVAO();

		GLuint id;

		void Bind() const;
		void Unbind() const;

		void SetupVertexAttribute(unsigned int id_, unsigned int num_, const unsigned int stride_, const GLvoid* offset_);
		void SetupVertexIntAttribute(unsigned int id_, unsigned int num_, unsigned int stride_, const GLvoid* offset_);
	};
}

#endif //!HYDRO_GL_VAO_H