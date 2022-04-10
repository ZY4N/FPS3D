#pragma once

#include <vector>
#include <glm/mat4x4.hpp>
#include <utils/packedTuple.hpp>
#include <geometry/vertex_comp.hpp>
#include <physics/aabb.hpp>

template<vertex_comp... Cs>
struct collidable {
	const std::vector<packedTuple<typename vertex_comps::position::type, typename Cs::type...>>& vertexBuffer;
	glm::mat4 transform;
	aabb boundingBox;
};