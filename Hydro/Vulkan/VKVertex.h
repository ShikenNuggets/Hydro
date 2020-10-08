#ifndef VK_VERTEX_H
#define VK_VERTEX_H

#include <array>

#include <vulkan/vulkan.hpp>

#include "Math/Vector.h"

namespace Hydro{
	struct VKVertex{
		VKVertex(const Vector3& pos_, const Vector3& color_, const Vector2& texCoords_) : pos(pos_), color(color_), texCoords(texCoords_){}

		Vector3 pos;
		Vector3 color; //TODO - Use special Color struct
		Vector2 texCoords;

		static vk::VertexInputBindingDescription GetBindingDescription(){
			vk::VertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(VKVertex);
			bindingDescription.inputRate = vk::VertexInputRate::eVertex;

			return bindingDescription;
		}

		static std::array<vk::VertexInputAttributeDescription, 3> GetAttributeDescription(){
			std::array<vk::VertexInputAttributeDescription, 3> attributeDescriptions{};
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = vk::Format::eR32G32B32A32Sfloat;
			attributeDescriptions[0].offset = offsetof(VKVertex, pos);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = vk::Format::eR32G32B32A32Sfloat;
			attributeDescriptions[1].offset = offsetof(VKVertex, color);

			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = vk::Format::eR32G32B32A32Sfloat;
			attributeDescriptions[2].offset = offsetof(VKVertex, texCoords);

			return attributeDescriptions;
		}
	};
}

#endif //!VK_VERTEX_H