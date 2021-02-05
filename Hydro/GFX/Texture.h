#ifndef HYDRO_IMAGE_LOADER_H
#define HYDRO_IMAGE_LOADER_H

#include "Resource.h"

#include <string>

#include <SDL_image.h>

namespace Hydro{
	class Texture : public Resource{
	public:
		Texture(SDL_Surface* surface_);
		~Texture();

		int Width() const{ return surface->w; };
		int Height() const{ return surface->h; };
		size_t Size() const{ return static_cast<size_t>(surface->h) * static_cast<size_t>(surface->pitch); };
		void* Pixels() const{ return surface->pixels; };

	private:
		SDL_Surface* surface;
	};

	//This is the shittiest singleton I've ever created
	class TextureLoader{
	public:
		static TextureLoader* Get();

		Texture LoadImage(const std::string& path_);

	private:
		TextureLoader();
		~TextureLoader();

		static TextureLoader* instance;
	};
}

#endif //!HYDRO_IMAGE_LOADER_H