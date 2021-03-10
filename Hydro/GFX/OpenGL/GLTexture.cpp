#include "GLTexture.h"

#include "Debug.h"

using namespace Hydro;

GLTexture::GLTexture(const Texture* texture_) : textureID(0){
	//Generate the Texture in OpenGL and get the ID
	glGenTextures(1, &textureID);

	//Bind the generated texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	glGenerateMipmap(GL_TEXTURE_2D);

	//Use alpha if the image supports it, otherwise don't
	int mode = (texture_->BytesPerPixel() == 4) ? GL_RGBA : GL_RGB;
	int precision = (texture_->BytesPerPixel() == 4) ? GL_RGBA8 : GL_RGB8;

	//Wrapping and filtering options
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Load the texture data from the SDL_Surface to the GPU memmory
	glTexImage2D(GL_TEXTURE_2D, 0, precision, texture_->Width(), texture_->Height(), 0, mode, GL_UNSIGNED_BYTE, texture_->Pixels());
	glGenerateMipmap(GL_TEXTURE_2D);
}

GLTexture::~GLTexture(){
	glDeleteTextures(1, &textureID);
}

GLuint GLTexture::TextureID(){
	return textureID;
}