#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <vector>
#include <string>
#include <map>

#include <json.hpp>

namespace Hydro{
	class FileSystem{
	public:
		enum class WriteType{
			append,
			clear,
			overwrite
		};

		static bool Initialize();
		static void Destroy();

		static bool FileExists(const std::string& file_);
		static std::vector<std::string> ReadFile(const std::string& file_);
		static std::string ReadFileToString(const std::string& file_);
		static std::vector<char> ReadBinaryFile(const std::string& file_);
		static nlohmann::json ReadJsonFile(const std::string& file_);

		static void WriteToFile(const std::string& file_, const std::string& content_, WriteType type_ = WriteType::append);

		static void ReadRecords(const std::string& file_, std::map<std::string, std::map<std::string, std::string>>& records_);
		static void WriteRecords(const std::string& file_, const std::map<std::string, std::map<std::string, std::string>>& records_, WriteType type_ = WriteType::clear);

		static std::string GetFileNameFromPath(const std::string& path_);
	};
}

#endif FILE_SYSTEM_H