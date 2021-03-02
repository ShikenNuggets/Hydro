#ifndef HYDRO_DEBUG_H
#define HYDRO_DEBUG_H

#include <iostream>
#include <string>

#include <SDL.h>

namespace Hydro{
	class Debug{
	public:
		inline static void Assert(bool condition_){
			#ifdef _DEBUG
			_ASSERT(condition_);
			#endif //_DEBUG
		}

		inline static void Assert(bool condition_, const std::string& message_){
			#ifdef _DEBUG
			if(!condition_){
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Assert failed!", message_.c_str());
			}
			#endif //_DEBUG
		}

		inline static void Log(const std::string& message_){
			#ifdef _DEBUG
			std::cout << message_ << std::endl;
			//TODO - Output to file, and... whatever else we should do here
			#endif //_DEBUG
		}
	};
}

#endif //!HYDRO_DEBUG_H