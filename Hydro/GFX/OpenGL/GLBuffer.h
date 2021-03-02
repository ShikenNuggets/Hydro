#ifndef HYDRO_GL_BUFFER_H
#define HYDRO_GL_BUFFER_H

#include <GL/glew.h>

namespace Hydro{
	class GLBuffer{
	public:
		GLBuffer(GLenum type_);
		~GLBuffer();

		GLuint id;
		GLenum bufferType;

		void Bind() const;
		void Unbind() const;

		void SetBufferData(size_t size_, const void* data_, GLenum usage_);
		void SetBufferSubData(GLintptr offset_, GLsizeiptr size_, const GLvoid* data_);
	};
}

#endif //!HYDRO_GL_BUFFER_H