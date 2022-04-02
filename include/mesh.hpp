#pragma once
#include <GL/glew.h> 
#include <SFML/OpenGL.hpp>

#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <intX.hpp>
#include <utils.hpp>
#include <primitives.hpp>
#include <packedTuple.hpp>
#include <vertex_comp.hpp>

#include <meshAttributes.hpp>
#include <meshInstance.hpp>
#include <shader.hpp>

template<vertex_comp... Cs>
class mesh {
public:
	using vertex = packedTuple<vertex_comps::position, Cs...>;

	mesh(std::vector<vertex>&& vertexBuffer, std::vector<u16>&& indexBuffer);
	
	~mesh();

	mesh(const mesh<Cs...>&);
	mesh(mesh<Cs...>&&);
	
	mesh<Cs...>& operator=(const mesh<Cs...>&);
	mesh<Cs...>& operator=(mesh<Cs...>&&);

	void initVAO();

	void addAttribute(mesh_attr::attribute* attr);

	void removeAttribute(mesh_attr::attribute* attr);

	meshInstance getInstance(const glm::mat4x4 modelMatrix = glm::identity<glm::mat4x4>()) const;

protected:

	std::vector<vertex> vertices;
	std::vector<u16> indices;
	std::vector<mesh_attr::attribute*> attributes;
	
	GLuint vertexBufferID{ 0 };
	GLuint indexBufferID{ 0 };
	GLuint vaoID{ 0 };

};
