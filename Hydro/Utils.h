#ifndef HYDRO_UTILS_H
#define HYDRO_UTILS_H

#include <vector>

namespace Hydro{
	class Utils{
	public:
		template <class T>
		static bool Has(std::vector<T> list, const T& value){
			return (std::find(list.begin(), list.end(), value) != list.end());
		}
	};
}

#endif //!HYDRO_UTILS_H