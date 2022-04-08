#pragma once

#include <utils/primitives.hpp>
#include <utils/intX.hpp>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

template<typename T>
concept vertex_comp = sizeof(typename T::type) > 0;

namespace vertex_comps {
struct position	{ using type = glm::vec<3, float, glm::packed_highp>; };
struct texCoord	{ using type = glm::vec<2, float, glm::packed_highp>; };
struct normal	{ using type = glm::vec<3, float, glm::packed_highp>; };
struct color	{ using type = color;	 };
}
