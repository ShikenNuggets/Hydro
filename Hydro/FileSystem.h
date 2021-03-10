#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <vector>
#include <string>
#include <map>

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

		static bool FileExists(std::string file_);
		static std::vector<std::string> ReadFile(std::string file_);
		static std::string ReadFileToString(std::string file_);
		static std::vector<char> ReadBinaryFile(const std::string& file_);
		static void WriteToFile(std::string file_, std::string content_, WriteType type_ = WriteType::append);

		static void ReadRecords(std::string file_, std::map<std::string, std::map<std::string, std::string>>& records_);
		static void WriteRecords(std::string file_, const std::map<std::string, std::map<std::string, std::string>>& records_, WriteType type_ = WriteType::clear);

		static std::string GetFileNameFromPath(const std::string& path_);
	};
}

#endif FILE_SYSTEM_H