#pragma once

#define DEBUG

#include <GL/glew.h> 
#include <SFML/OpenGL.hpp>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <utils/intX.hpp>
#include <graphics/meshEffects.hpp>
#include <graphics/shaderImpl.hpp>

struct renderable {
	GLuint vba;
	size_t numIndices;
	glm::mat4x4 transform;

	const meshColor* myColor;
	const meshTexture* myTexture;
	
	renderable(
		GLuint vba,
		size_t numIndices,
		const glm::mat4x4& transform,
		const meshColor* myColor,
		const meshTexture* myTexture
	) : vba{ vba },
		numIndices{ numIndices },
		transform{ transform },
		myColor{ myColor },
		myTexture{ myTexture }
	{}

	template<string_literal... Ns>
	void render(shader<Ns...>& s) {
/*
		s.template set<"modelMat">(transform);

		s.bind();
		glBindVertexArray(vba);

		if (myColor) myColor->preRender(s);
		if (myTexture) myTexture->preRender(s);

		s.bind();
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);
		
		if (myColor) myColor->postRender(s);
		if (myTexture) myTexture->postRender(s);

		s.bind();
		glBindVertexArray(0);
		s.unbind();
		*/
	}
};
