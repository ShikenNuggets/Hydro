#include <iostream>

#include "App.h"

int main(int argc, char* argv[]){
	Hydro::App::GetInstance()->Run();

#ifdef _DEBUG
	Hydro::App::DeleteInstance();
	std::cout << "The program is now complete. Press enter to exit." << std::endl;
	std::cin.get();
#endif //_DEBUG
	return 0;
}