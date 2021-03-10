#include "Window.h"

#include "App.h"
#include "Debug.h"
#include "GFX/Vulkan/VKRenderer.h"

#include <exception>

#include <SDL_vulkan.h>

using namespace Hydro;

Window::Window(API api_, int w_, int h_, const std::string& name_) : window(nullptr), size(w_, h_), name(name_), state(State::Hidden), graphicsAPI(api_), glContext(nullptr){
	Debug::Assert(size.x > 0 && size.y > 0, "Window size cannot be zero!");

	if(SDL_Init(SDL_INIT_EVERYTHING) > 0){
		throw std::exception("Could not initialize SDL!");
	}

	int apiFlag = SDL_WINDOW_OPENGL;
	if(api_ == API::Vulkan){
		apiFlag = SDL_WINDOW_VULKAN;
	}

	window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.x, size.y, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE | apiFlag);
	Debug::Assert(window, SDL_GetError());
	if(window == nullptr){
		throw std::exception("Could not create Window!");
	}
	state = State::Visible;
}

Window::~Window(){
	GL_DestroyContext();

	if(window != nullptr){
		SDL_DestroyWindow(window);
		window = nullptr;
	}

	SDL_Quit();
}

bool Window::HandleEvents(){
	SDL_Event e;
	while(SDL_PollEvent(&e) != 0){
		switch(e.type){
			case SDL_QUIT:
				return false;
			case SDL_WINDOWEVENT:
				HandleWindowEvent(e);
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			default:
				break;
		}
	}

	return true;
}

void Window::HandleWindowEvent(SDL_Event e){
	_ASSERT(e.type == SDL_WINDOWEVENT); //Nothing other than Window events should be passed to this function
	_ASSERT(e.window.windowID == SDL_GetWindowID(window)); //If these don't match, there may be multiple windows

	int w, h;
	switch(e.window.event){
		case SDL_WINDOWEVENT_SHOWN:
			state = State::Visible;
			break;
		case SDL_WINDOWEVENT_HIDDEN:
			state = State::Hidden;
			break;
		case SDL_WINDOWEVENT_MINIMIZED:
			state = State::Minimized;
			break;
		case SDL_WINDOWEVENT_MAXIMIZED:
			state = State::Maximized;
			SDL_GetWindowSize(window, &w, &h);
			size = ScreenCoordinate(w, h);
			App::GetRenderer()->OnResize(size.x, size.y); //TODO - Use a proper Event system instead
			break;
		case SDL_WINDOWEVENT_RESTORED:
			state = State::Visible;
			SDL_GetWindowSize(window, &w, &h);
			size = ScreenCoordinate(w, h);
			App::GetRenderer()->OnResize(size.x, size.y); //TODO - Use a proper Event system instead
			break;
		case SDL_WINDOWEVENT_RESIZED:
			SDL_GetWindowSize(window, &w, &h);
			size = ScreenCoordinate(w, h);
			App::GetRenderer()->OnResize(size.x, size.y);
			break;
		default:
			break;
	}
}

void Window::GL_CreateContext(){
	Debug::Assert(graphicsAPI == API::OpenGL);
	glContext = SDL_GL_CreateContext(window);

	//Ignore deprecated functionality
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//Set the OpenGL version to 3.3 (this covers most computers)
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//Set up the depth buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
	//Sets up for Multi sampling/ Anti Aliasing 
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	#ifdef HYDRO_DEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	#endif //HYDRO_DEBUG
}

void Window::GL_DestroyContext(){
	if(glContext != nullptr){
		Debug::Assert(graphicsAPI == API::OpenGL);
		SDL_GL_DeleteContext(glContext);
		glContext = nullptr;
	}
}

void Window::GL_UpdateWindow(){
	Debug::Assert(graphicsAPI == API::OpenGL);
	SDL_GL_SwapWindow(window);
}

vk::SurfaceKHR Window::VK_CreateSurface(const vk::Instance& instance_){
	Debug::Assert(graphicsAPI == API::Vulkan);

	VkSurfaceKHR rawSurface;
	if(!SDL_Vulkan_CreateSurface(window, instance_, &rawSurface)){
		throw std::runtime_error("Could not create VK Surface!");
	}

	return rawSurface;
}

std::vector<const char*> Window::VK_GetExtensions(){
	Debug::Assert(graphicsAPI == API::Vulkan);

	unsigned int sdlExtensionCount = 0;
	if(!SDL_Vulkan_GetInstanceExtensions(window, &sdlExtensionCount, nullptr)){
		throw std::exception("Failed to get VK extension count!");
	}

	std::vector<const char*> sdlExtensions(sdlExtensionCount);
	if(!SDL_Vulkan_GetInstanceExtensions(window, &sdlExtensionCount, sdlExtensions.data())){
		throw std::exception("Failed to get VK extensions!");
	}

	return sdlExtensions;
}