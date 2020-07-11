#ifndef VK_VERTEX_H
#define VK_VERTEX_H

#include <array>

#include "Math/Vector.h"

namespace Hydro{
	struct VKVertex{
		VKVertex(const Vector2& pos_, const Vector3& color_, const Vector2& texCoords_) : pos(pos_), color(color_), texCoords(texCoords_){}

		Vector2 pos;
		Vector3 color; //TODO - Use special Color struct
		Vector2 texCoords;

		static VkVertexInputBindingDescription GetBindingDescription(){
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(VKVertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescription(){
			std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(VKVertex, pos);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(VKVertex, color);

			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[2].offset = offsetof(VKVertex, texCoords);

			return attributeDescriptions;
		}
	};
}

#endif //!VK_VERTEX_H