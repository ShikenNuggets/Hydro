#ifndef APP_H
#define APP_H

#include "Window.h"
#include "ResourceMgr.h"
#include "Vulkan/VKRenderer.h"

namespace Hydro{
	class App{
	public:
		static App* GetInstance();

#ifdef _DEBUG
		static void DeleteInstance(); //Only use this for testing proper shutdown, never use this in production
#endif //_DEBUG

		void Run();

		static int GetWindowWidth(){ if(GetInstance()->window) return GetInstance()->window->Width(); else return 0; };
		static int GetWindowHeight(){ if(GetInstance()->window) return GetInstance()->window->Height(); else return 0; };
		static Renderer* GetRenderer(){ return GetInstance()->renderer; }
		static ResourceMgr* GetResourceMgr(){ return GetInstance()->resourceMgr; }

	private:
		static App* instance;

		App();
		~App();

		void Initialize();

		Window* window;
		Renderer* renderer;
		ResourceMgr* resourceMgr;
	};
}

#endif //!APP_H