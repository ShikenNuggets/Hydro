#ifndef HYDRO_GL_TEXTURE_H
#define HYDRO_GL_TEXTURE_H

#include "GFX/Texture.h"

#include <GL/glew.h>

namespace Hydro{
	class GLTexture{
	public:
		explicit GLTexture(const Texture* texture_);
		~GLTexture();

		GLuint TextureID();
	private:
		GLuint textureID;
	};
}

#endif //!HYDRO_GL_TEXTURE_H