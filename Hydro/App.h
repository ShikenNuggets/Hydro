#ifndef APP_H
#define APP_H

#include "Window.h"
#include "Vulkan/VKRenderer.h"

namespace Hydro{
	class App{
	public:
		static App* GetInstance();

#ifdef _DEBUG
		static void DeleteInstance(); //Only use this for testing proper shutdown, never use this in production
#endif //_DEBUG

		void Run();

	private:
		static App* instance;

		App();
		~App();

		Window* window;
		VKRenderer* renderer;
	};
}

#endif //!APP_H