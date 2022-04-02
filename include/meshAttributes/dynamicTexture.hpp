#pragma once

#include <GL/glew.h> 
#include <SFML/OpenGL.hpp>
#include <texture.hpp>

#include "meshAttribute.hpp"

#include <primitives.hpp>
#include <iostream>

using namespace mesh_attr;

struct dynamicTexture : public attribute {
	GLuint textureID;

	dynamicTexture(const texture& tex) {
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, tex.width, tex.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	~dynamicTexture() {
		glActiveTexture(GL_NONE);
		glDeleteTextures(1, &textureID);
	}

	inline void preRender(shader& s) override {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

	inline void postRender(shader& s) override {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};