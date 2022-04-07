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

#include <string_literal.hpp>
#include <templateUtils.hpp>

template<string_literal... Ns>
class shader {
private:
	GLuint id;
	GLint valueIDs[sizeof...(Ns)];

	static std::pair<char*, GLint> loadSource(const std::string& filename) ;
	static GLuint compileShader(GLenum type, const GLchar* src, GLint* length = NULL);

public:
	shader(GLuint programID);
	
	static shader loadShader(
		const std::string& vertexFile,
		const std::string& fragmentFile
	);

	static shader createShader(
		const char* vertexSrc, long vertexSrcLength,
		const char* geometrySrc, long geometrySrcLength,
		const char* fragmentSrc, long fragmentSrcLength
	);

	~shader();

	GLuint getID();

	void bind();

	void unbind();

	//template<string_literal N, typename T>
	//void set(const T& value);

	template<string_literal N, typename T>
	void set(const T& value) {
		constexpr size_t valueIndex = string_index<N, Ns...>();
		GLint valueID;
		if constexpr (valueIndex == -1) {
			//this branch should never be taken
			puts("hi");
			valueID = glGetUniformLocation(id, N.value);		
		} else {
			valueID = valueIDs[valueIndex];
		}
		
		bind();
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
		else if constexpr (std::same_as<T, float>)
			glUniform1f(valueID, value);
		else if constexpr (std::same_as<T, int>)
			glUniform1i(valueID, value);
		else T::_unimplemented_function;
		unbind();
	}
};



