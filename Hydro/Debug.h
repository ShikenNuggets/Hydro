#ifndef HYDRO_DEBUG_H
#define HYDRO_DEBUG_H

#include <iostream>
#include <string>

#include <SDL.h>

#include "FileSystem.h"

namespace Hydro{
	class Debug{
	public:
		inline static void Assert(bool condition_){
			#ifdef HYDRO_DEBUG
			_ASSERT(condition_);
			#endif //HYDRO_DEBUG
		}

		inline static void Assert(bool condition_, const std::string& message_){
			#ifdef HYDRO_DEBUG
			if(!condition_){
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Assert failed!", message_.c_str(), nullptr);
				__debugbreak();
			}
			#endif //HYDRO_DEBUG
		}

		inline static void Log(const std::string& message_){
			#ifdef HYDRO_DEBUG
			LogMessage(message_);
			#endif //HYDRO_DEBUG
		}

		inline static void Log(const std::string& message_, const char* file_, int line_){
			#ifdef HYDRO_DEBUG
			LogMessage(message_ + " [" + FileSystem::GetFileNameFromPath(file_) + " : " + std::to_string(line_) + "]");
			#endif //HYDRO_DEBUG
		}

		inline static void LogWarning(const std::string& message_){
			LogMessage("WARNING: " + message_);
		}

		inline static void LogWarning(const std::string& message_, const char* file_, int line_){
			LogMessage("WARNING: " + message_ + " [" + FileSystem::GetFileNameFromPath(file_) + " : " + std::to_string(line_) + "]");
		}

		inline static void LogError(const std::string& message_){
			LogMessage("ERROR: " + message_);
		}

		inline static void LogError(const std::string& message_, const char* file_, int line_){
			LogMessage("ERROR: " + message_ + " [" + FileSystem::GetFileNameFromPath(file_) + " : " + std::to_string(line_) + "]");
		}

	private:
		inline static void LogMessage(const std::string& message_){
			std::cout << message_ << std::endl;
			//TODO - Output to file, and... whatever else we should do here
		}
	};
}

#endif //!HYDRO_DEBUG_H