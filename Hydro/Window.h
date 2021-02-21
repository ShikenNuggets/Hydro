#ifndef WINDOW_H
#define WINDOW_H

#include <string>

#include <SDL.h>
#include <vulkan/vulkan.hpp>

namespace Hydro{
	class Window{
	public:
		enum class State{
			Hidden,
			Visible,
			Minimized,
			Maximized
		};

		Window(int w_ = 800, int h_ = 600, const std::string& name_ = "Hydro");
		~Window();

		bool HandleEvents();
		SDL_Window* GetSDLWindow() const{ return window; }

		int Width() const{ return width; }
		int Height() const{ return height; }
		std::string Name() const{ return name; }
		State GetState() const{ return state; }

		//Graphics API specific stuff
		vk::SurfaceKHR CreateVKSurface(const vk::Instance& instance_);

	private:
		SDL_Window* window;
		int width, height;
		std::string name;
		State state;

		void HandleWindowEvent(SDL_Event e);
	};
}

#endif //!WINDOW_H