#include "ResourceMgr.h"

#include "Texture.h";
#include "Model.h";

using namespace Hydro;

ResourceMgr::ResourceMgr() : resources(){
	//TODO - This is temp. Load all this crap from a JSON file or something
	resources.emplace("RoomModel", new Model(ModelLoader::Get()->LoadModel("Resources/Models/viking_room.obj")));
	resources.emplace("RoomTexture", new Texture(TextureLoader::Get()->LoadImage("Resources/Textures/viking_room.png")));
}

ResourceMgr::~ResourceMgr(){
	for(auto r : resources){
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