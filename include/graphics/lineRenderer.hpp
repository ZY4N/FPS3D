#pragma once

#include <graphics/renderer.hpp>
#include <graphics/shaderImpl.hpp>

struct lineRenderer : public renderer {

	defaultShader* main;
	lineShader* lines;
	glm::vec4 lineColor;

	lineRenderer(defaultShader* main, lineShader* lines, glm::vec4 color) : 
		main{ main }, lines{ lines }, lineColor{ std::move(color) } {};

	void render(
		const std::vector<renderable>& renderables,
		const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix
	) override;
};
