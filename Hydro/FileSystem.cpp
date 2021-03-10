#include "FileSystem.h"

#include <fstream>

using namespace Hydro;

bool FileSystem::Initialize(){
	return true;
}

void FileSystem::Destroy(){
}

bool FileSystem::FileExists(std::string file_){
	std::fstream filestream;
	filestream.open(file_, std::ios::in);

	if(filestream.is_open()){
		return true;
	}
	else{
		return false;
	}
}

std::vector<std::string> FileSystem::ReadFile(std::string file_){
	std::vector<std::string> fileContents;

	std::fstream filestream;
	filestream.open(file_, std::ios::in);

	if(!filestream.is_open()){
		//Debug::LogError("Could not open " + file_ + " for reading!", __FILE__, __LINE__);
		return fileContents;
	}

	std::string line;
	while(std::getline(filestream, line)){
		fileContents.push_back(line);
	}

	//Close the file when we're done
	filestream.close();
	return fileContents;
}

std::string FileSystem::ReadFileToString(std::string file_){
	std::string result;
	std::vector<std::string> fileContents = ReadFile(file_);
	for(std::string s : fileContents){
		result += s + "\n";
	}

	return result;
}

std::vector<char> FileSystem::ReadBinaryFile(const std::string& file_){
	_ASSERT(!file_.empty());

	std::ifstream file(file_, std::ios::ate | std::ios::binary);
	if(!file.is_open()){
		throw std::exception("Failed to open file!");
	}

	size_t fileSize = static_cast<size_t>(file.tellg());
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}

void FileSystem::WriteToFile(std::string file_, std::string content_, WriteType type_){
	std::fstream filestream;

	switch(type_){
	case FileSystem::WriteType::append:
		filestream.open(file_, std::ios::out | std::ios_base::app);
		break;
	case FileSystem::WriteType::clear:
		filestream.open(file_, std::ios::out | std::ios_base::trunc);
		break;
	case FileSystem::WriteType::overwrite:
		filestream.open(file_, std::ios::out);
		break;
	default:
		break;
	}

	if(!filestream.is_open()){
		//Debug::LogError("Could not open " + file_ + " for writing!", __FILE__, __LINE__);
		return;
	}

	filestream << content_;
	filestream.close();
}

void FileSystem::ReadRecords(std::string file_, std::map<std::string, std::map<std::string, std::string>>& records_){
	std::fstream filestream;
	filestream.open(file_, std::ios::in);

	if(!filestream.is_open()){
		//Debug::LogError("Could not open " + file_ + " for reading!", __FILE__, __LINE__);
		return;
	}

	std::string currentSectionName = "";
	std::string line;
	while(std::getline(filestream, line)){
		if(line.size() >= 1 && line.at(0) == '['){
			auto first = line.find('[') + 1;
			auto last = line.find_last_of(']');
			currentSectionName = line.substr(first, last - first);
			records_.insert(std::make_pair(currentSectionName, std::map<std::string, std::string>()));
		}
		else if(line.size() >= 1){
			//Use '=' as a delimeter to get the key and the value from this line
			std::string key = line.substr(0, line.find('='));
			std::string value = line.substr(line.find('=') + 1);

			for(auto& val : records_){
				if(val.first == currentSectionName){
					val.second.insert(std::make_pair(key, value));
				}
			}
		}
	}
}

void FileSystem::WriteRecords(std::string file_, const std::map<std::string, std::map<std::string, std::string>>& records_, WriteType type_){
	std::fstream filestream;

	switch(type_){
	case FileSystem::WriteType::append:
		filestream.open(file_, std::ios::out | std::ios::app);
		break;
	case FileSystem::WriteType::clear:
		filestream.open(file_, std::ios::out | std::ios::trunc);
		break;
	case FileSystem::WriteType::overwrite:
		filestream.open(file_, std::ios::out);
		break;
	default:
		break;
	}

	if(!filestream.is_open()){
		//Debug::LogError("Could not open " + file_ + " for writing!", __FILE__, __LINE__);
		return;
	}

	bool firstLine = true;
	for(const auto& value : records_){
		if(!firstLine){
			filestream << std::endl << std::endl;
		}
		else{
			firstLine = false;
		}

		filestream << "[" << value.first << "]";
		for(const auto& innerValue : value.second){
			filestream << std::endl << innerValue.first << "=" << innerValue.second;
		}
	}

	filestream.close();
}

std::string FileSystem::GetFileNameFromPath(const std::string& path_){
	char sep = '/';

	#ifdef HYDRO_PLATFORM_WIN32
	sep = '\\';
	#endif

	size_t i = path_.rfind(sep, path_.length());
	if(i != std::string::npos){
		return(path_.substr(i + 1, path_.length() - i));
	}
	return "";
}