#pragma once

#include <graphics/renderer.hpp>
#include <graphics/shaderImpl.hpp>

struct defaultRenderer : public renderer {

	defaultShader* main;

	defaultRenderer(defaultShader* main) : 
		main{ main } {};

	void render(
		const std::vector<renderable>& renderables,
		const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix
	) override;
};
