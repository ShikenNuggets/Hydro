#include "ResourceMgr.h"

#include "Debug.h"
#include "FileSystem.h"
#include "GFX/Texture.h"
#include "GFX/Model.h"

using namespace Hydro;

constexpr const char* RESOURCE_FILE = "Resources/resources.json";

ResourceMgr::ResourceMgr() : resources(){
	if(FileSystem::FileExists(RESOURCE_FILE)){
		auto json = FileSystem::ReadJsonFile(RESOURCE_FILE);
		
		if(json.empty() || json.is_null() || !json.is_array()){
			//An error has occurred with JSON parsing
		}

		for(auto& e : json){
			if(e.empty() || e.is_null()){
				Debug::Assert(false, "An error has occurred while parsing JSON!");
				continue;
			}

			if(!e.contains("type") || !e.contains("name") || !e.contains("location")){
				Debug::Assert(false, "A resource is missing required parameters! All resources must have a [name], [type], and [location].");
				continue;
			}

			if(e["type"] == "Model"){
				resources.emplace(e["name"], new Model(ModelLoader::Get()->Test_LoadModel(e["location"])));
			}else if(e["type"] == "Texture"){
				resources.emplace(e["name"], new Texture(TextureLoader::Get()->LoadImage(e["location"])));
			}else{
				Debug::Assert(false, "Resource [" + e["name"].dump() + "] has invalid type [" + e["type"].dump() + "]!");
				continue;
			}
		}
	}

	//TODO - This is temp. Load all this crap from a JSON file or something
	//resources.emplace("RoomModel", new Model(ModelLoader::Get()->Test_LoadModel("Resources/Models/viking_room.obj")));
	//resources.emplace("RoomTexture", new Texture(TextureLoader::Get()->LoadImage("Resources/Textures/viking_room.png")));
}

ResourceMgr::~ResourceMgr(){
	for(auto &r : resources){
		delete r.second;
		r.second = nullptr;
	}
	resources.clear();
}

//template <class T>
//inline T* Hydro::ResourceMgr::Get(const std::string& resourceName_){
//	static_assert(std::is_base_of<Resource, T>::value, "T must inherit from Resource!");
//
//	auto search = resources.find(resourceName_);
//	if(search == resources.end()){
//		return nullptr;
//	}
//
//	T* value = dynamic_cast<T*>(*search);
//	return value;
//}