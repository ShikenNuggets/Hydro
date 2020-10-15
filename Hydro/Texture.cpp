#include "Texture.h"

#include <stdexcept>
#include <string>

#include <SDL_image.h>

using namespace Hydro;

TextureLoader* TextureLoader::instance = nullptr;

Texture::Texture(SDL_Surface* surface_) : Resource(), surface(surface_){
	_ASSERT(surface != nullptr);
}

Texture::~Texture(){
	SDL_FreeSurface(surface);
	surface = nullptr;
}

TextureLoader::TextureLoader(){
	auto flags = IMG_INIT_PNG | IMG_INIT_JPG;
	if((IMG_Init(flags) & flags) != flags){
		throw std::runtime_error("SDL_image could not initialize! SDL_image Error: " + std::string(IMG_GetError()));
	}
}

TextureLoader::~TextureLoader(){
	IMG_Quit();
}

TextureLoader* TextureLoader::Get(){
	if(instance == nullptr){
		instance = new TextureLoader();
	}

	return instance;
}

Texture TextureLoader::LoadImage(const std::string& path_){
	SDL_Surface* textureSurface = IMG_Load(path_.c_str());
	if(textureSurface == NULL || textureSurface->pixels == NULL){
		throw std::runtime_error("Could not load image! SDL_image Error: " + std::string(IMG_GetError()));
	}

	return Texture(textureSurface);
}