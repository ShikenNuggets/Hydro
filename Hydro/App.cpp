#include "App.h"

#include <iostream>

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
	std::cout << "App shutdown" << std::endl; //TODO - Debug logging
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
	std::cout << "App startup" << std::endl; //TODO - Debug logging
	window = new Window();
	resourceMgr = new ResourceMgr();
	renderer = new VKRenderer(window);
}

void App::Run(){
	Initialize();

	bool isRunning = true;
	while(isRunning){
		isRunning = window->HandleEvents();
		renderer->Render();
	}
}