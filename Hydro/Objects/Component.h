#ifndef HYDRO_COMPONENT_H
#define HYDRO_COMPONENT_H

namespace Hydro{
	//Forward declaration
	class GameObject;

	class Component{
	public:
		virtual ~Component(){}

		GameObject* GetObject() const{ return gameObject; }
		void SetObject(GameObject* go_){ gameObject = go_; }

	private:
		GameObject* gameObject;
	};
}

#endif //!HYDRO_COMPONENT_H