#ifndef HYDRO_RESOURCE_MGR_H
#define HYDRO_RESOURCE_MGR_H

#include "Resource.h"

#include <map>
#include <string>

namespace Hydro{
	class ResourceMgr{
	public:
		ResourceMgr();
		~ResourceMgr();

		template <class T>
		T* Get(const std::string& resourceName_){
			static_assert(std::is_base_of<Resource, T>::value, "T must inherit from Resource!");
			if(resources.find(resourceName_) == resources.end()){
				return nullptr;
			}

			T* value = dynamic_cast<T*>(resources[resourceName_]);
			return value;
		}

	private:
		std::map<std::string, Resource*> resources;
	};
}

#endif //!HYDRO_RESOURCE_MGR_H