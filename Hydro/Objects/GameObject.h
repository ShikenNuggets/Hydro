#ifndef HYDRO_GAME_OBJECT_H
#define HYDRO_GAME_OBJECT_H

#include <vector>

#include "Objects/Component.h"
#include "Objects/Transform.h"

//TODO - TEMP
#include "GFX/MeshRenderer.h"

namespace Hydro{
	class GameObject{
	public:
		GameObject() : transform(), components(){
		}

		void AddComponent(Component* comp_){
			_ASSERT(comp_ != nullptr);
			components.push_back(comp_);
		}

		template <class T> T* GetComponent(){
			static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
			T* comp = nullptr;

			//Check every component on this object
			for(Component* c : components){
				//Cast this Component* to a T* (whatever type of component you're looking for)
				comp = dynamic_cast<T*>(c);
				//If the cast was successful (did not return nullptr), then we have that type of component, so return it
				if(comp != nullptr){
					return comp;
				}
			}

			//If no component of this type was found, return nullptr
			return nullptr;
		}

	private:
		Transform transform;
		std::vector<Component*> components;
	};
}

#endif //!HYDRO_GAME_OBJECT_H