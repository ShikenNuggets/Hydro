#ifndef GL_MAIN_FBO_H
#define GL_MAIN_FBO_H

#include "GLFBO.h"

namespace Hydro{
	class GLMainFBO : public GLFBO{
	public:
		GLMainFBO(const ScreenCoordinate& screenSize_);
		virtual ~GLMainFBO() override;

		GLuint GetColorTexture(){
			return colorTexture;
		}

	protected:
		GLuint colorTexture;
	};
}

#endif //!GL_MAIN_FBO_H