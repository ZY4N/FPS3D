#pragma once

#include <graphics/shader.hpp>

template<string_literal... Ns>
shader<Ns...>::shader(GLuint programID) : id{ programID } {
	size_t valueIndex = 0;
	((valueIDs[valueIndex++] = glGetUniformLocation(id, Ns.value)), ...);
}

template<string_literal... Ns>
std::pair<char*, GLint> shader<Ns...>::loadSource(const std::string& filename) {
	std::ifstream file(filename);

	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	
	if (size == 0 || size == SIZE_MAX) {
		file.close();
		std::cout << "Cannot open shader source file '" << filename << "'.\n";
		return { nullptr, 0 };
	}

	char* src = new char[size + 1];
	src[size] = 0;

	file.seekg(0);
	file.read(src, size);
	file.close();

	return { src, size };
}

template<string_literal... Ns>
GLuint shader<Ns...>::compileShader(GLenum type, const GLchar* src, GLint* length) {
	char infoLog[512];
	GLint success;

	GLuint shader = glCreateShader(type);

	glShaderSource(shader, 1, &src, length);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "Could not compile shader " << type << "\n";
		std::cout << infoLog << "\n";
		throw "broken shader";
	}
	return shader;
}


template<string_literal... Ns>
shader<Ns...> shader<Ns...>::loadShader(
	const std::string& vertexFile,
	const std::string& geometryFile,
	const std::string& fragmentFile
) {
	auto [ vertexSrc, vertexLen ] = loadSource(vertexFile);
	auto [ geometrySrc, geometryLen ] = loadSource(geometryFile);
	auto [ fragmentSrc, fragmentLen ] = loadSource(fragmentFile);

	return createShader(
		vertexSrc, vertexLen,
		geometrySrc, geometryLen,
		fragmentSrc, fragmentLen
	);

	delete[] vertexSrc;
	delete[] geometrySrc;
	delete[] fragmentSrc;
}


template<string_literal... Ns>
shader<Ns...> shader<Ns...>::createShader(
	const char* vertexSrc	, long vertexSrcLength,
	const char* geometrySrc	, long geometrySrcLength,
	const char* fragmentSrc	, long fragmentSrcLength
){
	struct shaderInfo {
		GLenum type{ 0 };
		const char* src{ nullptr };
		long len{ 0 };
		GLint id{ 0 };
	};

	std::array<shaderInfo, 3> shaders {
		shaderInfo{ .type = GL_VERTEX_SHADER	, .src = vertexSrc	, .len = vertexSrcLength	},
		shaderInfo{ .type = GL_GEOMETRY_SHADER	, .src = geometrySrc, .len = geometrySrcLength	},
		shaderInfo{ .type = GL_FRAGMENT_SHADER	, .src = fragmentSrc, .len = fragmentSrcLength	}
	};

	GLuint programID = glCreateProgram(); 

	for (auto& shader : shaders) {
		if (shader.len) {
			shader.id = compileShader(shader.type, shader.src, (GLint*)&shader.len);
			glAttachShader(programID, shader.id);
		} else {
			std::string shaderName;
			switch (shader.type) {
				case GL_VERTEX_SHADER:		shaderName = "vertex";		break;
				case GL_GEOMETRY_SHADER:	shaderName = "geometry";	break;
				case GL_FRAGMENT_SHADER:	shaderName = "fragment";	break;
				default: shaderName = "unknown";
			}
			std::cout << "Using default " << shaderName << " shader." << std::endl;
		}
	}

	glLinkProgram(programID);

	char infoLog[512];
	GLint success;
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	
	if (!success) {
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		std::cout << "Failed linking program" << "\n";
		std::cout << infoLog << "\n";
	}

	glUseProgram(0);

	for (const auto& shader : shaders) {
		if (shader.id) {
			glDeleteShader(shader.id);
		}
	}

	return shader(programID);
}

template<string_literal... Ns>
shader<Ns...>::~shader() {
	glDeleteProgram(id);
}

template<string_literal... Ns>
GLuint shader<Ns...>::getID() {
	return id;
}

template<string_literal... Ns>
void shader<Ns...>::bind() {
	glUseProgram(id);
}

template<string_literal... Ns>
void shader<Ns...>::unbind() {
	glUseProgram(0);
}

