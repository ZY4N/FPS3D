#include <shader.hpp>

std::pair<char*, GLint> shader::loadSource(const std::string& filename) {
	std::ifstream file(filename);

	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	
	if (size == 0 || size == -1) {
		//throw std::runtime_error(std::string("Cannot open shader source file '") + filename + '\'');
	}

	char* src = new char[size + 1];
	src[size] = 0;

	file.seekg(0);
	file.read(src, size);
	file.close();

	return { src, size };
}

GLuint shader::compileShader(GLenum type, const GLchar* src, GLint* length) {
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

shader shader::shaderFromOBJ(
	const char* vertexStart, const char* vertexEnd,
	const char* fragmentStart, const char* fragmentEnd
) {
	return createShader(
		shaderSource{ GL_VERTEX_SHADER, vertexStart, (vertexEnd - vertexStart) - 1 },
		shaderSource{ GL_FRAGMENT_SHADER, fragmentStart, (fragmentEnd - fragmentStart) - 1 }
	);
}

shader shader::loadShader(
	const std::string& vertexFile,
	const std::string& fragmentFile
) {
	auto [ vertexSrc, vertexLen ] = loadSource(vertexFile);
	auto [ fragmentSrc, fragmentLen ] = loadSource(fragmentFile);

	return createShader(
		shaderSource{ GL_VERTEX_SHADER, vertexSrc, vertexLen },
		shaderSource{ GL_FRAGMENT_SHADER, fragmentSrc, fragmentLen }
	);

	delete[] vertexSrc;
	delete[] fragmentSrc;
}


template<shaderSourceType... Ts>
shader shader::createShader(Ts... shaderSources) {
	
	std::array<GLint, 3> shaderIDs{ 0, 0, 0 }; 
	GLuint programID = glCreateProgram(); 

	auto shaderID = shaderIDs.begin();
	constexpr_for([&](const auto& shader) {
		if (shader.len) {
			*shaderID = compileShader(shader.type, shader.src, (GLint*)&shader.len);
			glAttachShader(programID, *shaderID++);
		}
	}, shaderSources...);

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

	for (auto shaderID : shaderIDs) {
		if (shaderID) {
			glDeleteShader(shaderID);
		}
	}

	return shader(programID);
}

shader::~shader() {
	glDeleteProgram(id);
}

GLuint shader::getID() {
	return id;
}

void shader::bind() {
	glUseProgram(id);
}

void shader::unbind() {
	glUseProgram(0);
}

