#ifndef HYDRO_SCENE_H
#define HYDRO_SCENE_H

#include <vector>

#include "Objects/GameObject.h"

namespace Hydro{
	class Scene{
	public:
		//TODO - TEMP, get rid of this constructor ASAP!
		Scene() : gameObjects(){
			GameObject* go1 = new GameObject();
			go1->AddComponent(new MeshRenderer("RoomModel", "RoomTexture"));

			GameObject* go2 = new GameObject();
			go2->GetTransform().position = Vector3(0.0f, 0.0f, 10.0f);
			go2->AddComponent(new Camera());

			gameObjects.push_back(go1);
			gameObjects.push_back(go2);
		}

		~Scene(){
			for(GameObject* go : gameObjects){
				delete go;
			}
		}

		std::vector<GameObject*> gameObjects;
	};
}

#endif //!HYDRO_SCENE_H