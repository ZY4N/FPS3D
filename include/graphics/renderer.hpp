#pragma once

#include <graphics/shaderImpl.hpp>
#include <graphics/renderable.hpp>
#include <glm/mat4x4.hpp>
#include <vector>

struct renderer {

	virtual void render(
		const std::vector<renderable>& renderables,
		const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix
	) = 0;

};
