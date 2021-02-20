#ifndef APP_H
#define APP_H

#include "Window.h"
#include "ResourceMgr.h"
#include "GFX/Vulkan/VKRenderer.h"
#include "Scenes/Scene.h"

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

		static Scene* CurrentScene(){ return GetInstance()->currentScene; }

	private:
		static App* instance;

		App();
		~App();

		void Initialize();

		Window* window;
		Renderer* renderer;
		ResourceMgr* resourceMgr;
		Scene* currentScene;
	};
}

#endif //!APP_H