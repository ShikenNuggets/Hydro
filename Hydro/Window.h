#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>

namespace Hydro{
	class Window{
	public:
		enum class State{
			Hidden,
			Visible,
			Minimized,
			Maximized
		};

		Window();
		~Window();

		bool HandleEvents();
		SDL_Window* GetSDLWindow() const{ return window; }

		int Width() const{ return width; }
		int Height() const{ return height; }
		State GetState() const{ return state; }

	private:
		SDL_Window* window;
		int width, height;
		State state;

		void HandleWindowEvent(SDL_Event e);
	};
}

#endif //!WINDOW_H