#include "App.h"

#include <iostream>

#include "Debug.h"
#include "GFX/OpenGL/GLRenderer.h"

using namespace Hydro;

App* App::instance = nullptr;

App* App::GetInstance(){
	if(instance == nullptr){
		instance = new App();
	}

	return instance;
}

#ifdef _DEBUG
void App::DeleteInstance(){
	if(instance != nullptr){
		delete instance;
		instance = nullptr;
	}
}
#endif //_DEBUG

App::App() : window(nullptr), resourceMgr(nullptr), renderer(nullptr){
}

App::~App(){
	Debug::Log("App shutdown");
	if(renderer != nullptr){
		delete renderer;
		renderer = nullptr;
	}

	if(window != nullptr){
		delete window;
		window = nullptr;
	}
}

void App::Initialize(){
	Debug::Log("App startup");
	window = new Window(API::Vulkan);
	resourceMgr = new ResourceMgr();
	renderer = new VKRenderer(window);
	
	currentScene = new Scene();
}

void App::Run(){
	Initialize();

	bool isRunning = true;
	while(isRunning){
		isRunning = window->HandleEvents();
		renderer->Render();
	}
}