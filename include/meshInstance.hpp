#pragma once

#include <GL/glew.h> 
#include <SFML/OpenGL.hpp>

#include <intX.hpp>
#include <meshEffect.hpp>
#include <shader.hpp>

#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

struct meshInstance {
	GLuint vba;
	size_t numIndices;
	glm::mat4x4 transform;
	std::vector<meshEffect*> attributes;
	
	meshInstance(
		GLuint vba,
		size_t numIndices,
		const glm::mat4x4& transform,
		const std::vector<meshEffect*>& attributes
	) : vba{ vba },
		numIndices{ numIndices },
		transform{ transform },
		attributes{ attributes }
	{}

	void addAttribute(meshEffect* attr) {
		attributes.push_back(attr);
	}

	void removeAttribute(meshEffect* attr) {
		attributes.erase(std::find(attributes.begin(), attributes.end(), attr));
	}

	void render(shader& s) {
		//transform = glm::rotate(transform, 0.01f, { 1, 0, 0 });
		//transform = glm::rotate(transform, 0.01f, { 0, 1, 0 });
		//transform = glm::rotate(transform, 0.01f, { 0, 0, 1 });
		s.set("modelMat", transform);

		s.bind();
		glBindVertexArray(vba);

		for (auto attribute : attributes) {
			attribute->preRender(s);
			s.bind();
		}

		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);
		
		for (auto attribute : attributes) {
			attribute->postRender(s);
			s.bind();
		}

		glBindVertexArray(0);
		s.unbind();
	}
};
