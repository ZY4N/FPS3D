#pragma once

#include <GL/glew.h> 
#include <SFML/OpenGL.hpp>
#include <utils/texture.hpp>
#include <utils/primitives.hpp>
#include <utils/string_literal.hpp>

#include <graphics/meshEffect.hpp>

struct meshTexture {
	GLuint textureID;

	meshTexture(const texture& tex) {
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, tex.width, tex.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	meshTexture(const meshTexture&) = delete;
	meshTexture& operator=(const meshTexture&) = delete;

	~meshTexture() {
		//glActiveTexture(GL_NONE);
		glDeleteTextures(1, &textureID);
	}

	template<string_literal... Ns>
	inline void preRender(shader<Ns...>& s) const {
		s.template set<"colorMerge">(0.0f);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

	template<string_literal... Ns>
	inline void postRender(shader<Ns...>& s) const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};
