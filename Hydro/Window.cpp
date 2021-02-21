#include "Window.h"

#include "App.h"
#include "GFX/Vulkan/VKRenderer.h"

#include <exception>

#include <SDL_vulkan.h>

using namespace Hydro;

Window::Window(int w_, int h_, const std::string& name_) : window(nullptr), width(w_), height(h_), name(name_), state(State::Hidden){
	if(SDL_Init(SDL_INIT_EVERYTHING) > 0){
		throw std::exception("Could not initialize SDL!");
	}

	window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN);
	if(window == nullptr){
		throw std::exception("Could not create Window!");
	}
	state = State::Visible;
}

Window::~Window(){
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
			SDL_GetWindowSize(window, &width, &height);
			App::GetRenderer()->OnResize(width, height);
			App::GetRenderer()->OnResize(width, height); //TODO - Use a proper Event system instead
			break;
		case SDL_WINDOWEVENT_RESTORED:
			state = State::Visible;
			SDL_GetWindowSize(window, &width, &height);
			App::GetRenderer()->OnResize(width, height); //TODO - Use a proper Event system instead
			break;
		case SDL_WINDOWEVENT_RESIZED:
			SDL_GetWindowSize(window, &width, &height);
			App::GetRenderer()->OnResize(width, height); //TODO - Use a proper Event system instead
			break;
		default:
			break;
	}
}

vk::SurfaceKHR Window::CreateVKSurface(const vk::Instance& instance_){
	VkSurfaceKHR rawSurface;
	if(!SDL_Vulkan_CreateSurface(window, instance_, &rawSurface)){
		throw std::runtime_error("Could not create VK Surface!");
	}

	return rawSurface;
}