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

App::App(){
	std::cout << "App startup" << std::endl;
	window = new Window();
	renderer = new VKRenderer(window);
}

App::~App(){
	std::cout << "App shutdown" << std::endl;
	if(renderer != nullptr){
		delete renderer;
		renderer = nullptr;
	}

	if(window != nullptr){
		delete window;
		window = nullptr;
	}
}

void App::Run(){
	bool isRunning = true;
	while(isRunning){
		isRunning = window->HandleEvents();
		renderer->Render();
	}
}