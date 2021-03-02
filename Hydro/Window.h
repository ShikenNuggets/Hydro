#ifndef WINDOW_H
#define WINDOW_H

#include <string>

#include <SDL.h>
#include <GL/glew.h>
#include <vulkan/vulkan.hpp>

#include "GFX/ScreenCoordinate.h"

namespace Hydro{
	enum class API{
		OpenGL,
		Vulkan
	};

	class Window{
	public:
		enum class State{
			Hidden,
			Visible,
			Minimized,
			Maximized
		};

		Window(API api_, int w_ = 800, int h_ = 600, const std::string& name_ = "Hydro");
		~Window();

		bool HandleEvents();
		SDL_Window* GetSDLWindow() const{ return window; }

		int Width() const{ return size.x; }
		int Height() const{ return size.y; }
		ScreenCoordinate Size() const{ return size; }
		std::string Name() const{ return name; }
		State GetState() const{ return state; }

		//OpenGL
		void GL_CreateContext();
		void GL_DestroyContext();
		void GL_UpdateWindow();

		//Vulkan
		vk::SurfaceKHR VK_CreateSurface(const vk::Instance& instance_);
		std::vector<const char*> VK_GetExtensions();

	private:
		SDL_Window* window;
		ScreenCoordinate size;
		std::string name;
		State state;
		API graphicsAPI;

		//OpenGL
		SDL_GLContext glContext;

		void HandleWindowEvent(SDL_Event e);
	};
}

#endif //!WINDOW_H