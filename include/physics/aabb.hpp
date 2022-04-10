#pragma once

#include <glm/glm.hpp>

#include <float.h>
#include <utils/packedTuple.hpp>


struct aabb {
	glm::vec3 min{  FLT_MAX,  FLT_MAX,  FLT_MAX };
	glm::vec3 max{ -FLT_MAX, -FLT_MAX, -FLT_MAX };

	float getWidth() { return max.x - min.x; }
	float getHeight() { return max.y - min.y; }
	float getDepth() { return max.z - min.z; }

	static inline aabb createBoundingBox(const glm::vec3* vertices, size_t numVertices) {
		aabb box;
		calcMinMax(vertices, numVertices, box.min, box.max);
		return box;
	}

	template<vertex_comp... Cs>
	static inline aabb createBoundingBox(
		const packedTuple<vertex_comps::position::type, Cs...>* vertices,
		size_t numVertices
	) {
		aabb box;
		calcMinMax(vertices, numVertices, box.min, box.max);
		return box;
	}


	template<vertex_comp... Cs>
	static inline void calcMinMax(
		const packedTuple<vertex_comps::position::type, Cs...>* vertices,
		size_t numVertices,
		glm::vec3& min, glm::vec3& max
	) {
		min = {  FLT_MAX,  FLT_MAX,  FLT_MAX };
		max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
		for (size_t i = 0; i < numVertices; i++) {
			const glm::vec3& vec = vertices[i].template get<0>();
			min = glm::min(min, vec);
			max = glm::max(max, vec);
		}
	}

	static inline void calcMinMax(const glm::vec3* vertices, size_t numVertices, glm::vec3& min, glm::vec3& max) {
		min = {  FLT_MAX,  FLT_MAX,  FLT_MAX };
		max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
		for (size_t i = 0; i < numVertices; i++) {
			min = glm::min(min, vertices[i]);
			max = glm::max(max, vertices[i]);
		}
	}

	void updateSize(const glm::mat4x4& transform) {
		const glm::vec3 vertices[]{
			glm::vec3{transform * glm::vec4{ min.x, min.y , min.z, 1 }},
			glm::vec3{transform * glm::vec4{ min.x, min.y , max.z, 1 }},
			glm::vec3{transform * glm::vec4{ min.x, max.y , min.z, 1 }},
			glm::vec3{transform * glm::vec4{ min.x, max.y , max.z, 1 }},
			glm::vec3{transform * glm::vec4{ max.x, min.y , min.z, 1 }},
			glm::vec3{transform * glm::vec4{ max.x, min.y , max.z, 1 }},
			glm::vec3{transform * glm::vec4{ max.x, max.y , min.z, 1 }},
			glm::vec3{transform * glm::vec4{ max.x, max.y , max.z, 1 }}
		};
		calcMinMax(vertices, sizeof(vertices) / sizeof(glm::vec3), min, max);
	}
};
