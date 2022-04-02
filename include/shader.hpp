#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <array>

#include <GL/glew.h> 
#include <SFML/OpenGL.hpp>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <templateUtils.hpp>

struct shaderSource {
	GLenum type;
	const char* src;
	long len;
};
template<typename T>
concept shaderSourceType = std::same_as<T, shaderSource>;

class shader {
private:
	GLuint id;

	static std::pair<char*, GLint> loadSource(const std::string& filename) ;
	static GLuint compileShader(GLenum type, const GLchar* src, GLint* length = NULL);

public:
	shader(GLuint programID) : id{ programID } {}

	static shader shaderFromOBJ(
		const char* vertexStart, const char* vertexEnd,
		const char* fragmentStart, const char* fragmentEnd
	);
	
	static shader loadShader(
		const std::string& vertexFile,
		const std::string& fragmentFile
	);

	template<shaderSourceType... Ts>
	static shader createShader(Ts... shaderSources);

	~shader();

	GLuint getID();

	void bind();

	void unbind();

	template<typename T>
	void set(const GLchar* name, const T& value) {
		bind();
		const GLint valueID = glGetUniformLocation(id, name);
		if constexpr (std::same_as<T, glm::mat4x4>)
			glUniformMatrix4fv(valueID, 1, false, glm::value_ptr(value));
		else if constexpr (std::same_as<T, glm::mat3x3>)
			glUniformMatrix3fv(valueID, 1, false, glm::value_ptr(value));
		else if constexpr (std::same_as<T, glm::fvec4>)
			glUniform4fv(valueID, 1, glm::value_ptr(value));
		else if constexpr (std::same_as<T, glm::fvec3>)
			glUniform3fv(valueID, 1, glm::value_ptr(value));
		else if constexpr (std::same_as<T, glm::fvec2>)
			glUniform2fv(valueID, 1, glm::value_ptr(value));
		else if constexpr (std::same_as<T, GLfloat>)
			glUniform1f(valueID, value);
		else if constexpr (std::same_as<T, GLint>)
			glUniform1i(valueID, value);
		else T::_unimplemented_function;
		unbind();
	}

};



