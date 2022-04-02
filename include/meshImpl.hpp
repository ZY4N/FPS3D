#pragma once
#include <mesh.hpp>

template<vertex_comp... Cs>
mesh<Cs...>::mesh(
	const std::vector<typename mesh<Cs...>::vertex>& vertexBuffer,
	const std::vector<u16>& indexBuffer
) : vertices{ vertexBuffer }, indices{ indexBuffer } {
}

template<vertex_comp... Cs>
mesh<Cs...>::mesh(
	std::vector<typename mesh<Cs...>::vertex>&& vertexBuffer,
	std::vector<u16>&& indexBuffer
) : vertices{ std::move(vertexBuffer) }, indices{ std::move(indexBuffer) } {
}

template<vertex_comp... Cs>
mesh<Cs...>::~mesh() {
	glDeleteBuffers(1, &vertexBufferID);
	glDeleteBuffers(1, &indexBufferID);
}

template<vertex_comp... Cs>
mesh<Cs...>::mesh(const mesh<Cs...>& other) :
	vertices{ other.vertices },
	indices{ other.indices },
	attributes{ other.attributes } 
{}

template<vertex_comp... Cs>
mesh<Cs...>::mesh(mesh<Cs...>&& other):
	vertices{ std::move(other.vertices) },
	indices{ std::move(other.indices) },
	attributes{ std::move(other.attributes) },
	vaoID{ other.vaoID },
	vertexBufferID{ other.vertexBufferID },
	indexBufferID{ other.indexBufferID }
{	
	other.vaoID = 0;
	other.vertexBufferID = 0;
	other.indexBufferID = 0;
}

template<vertex_comp... Cs>
mesh<Cs...>& mesh<Cs...>::operator=(const mesh<Cs...>& other) {
	glDeleteBuffers(1, &vertexBufferID);
	glDeleteBuffers(1, &indexBufferID);
	vertexBufferID = 0;
	indexBufferID = 0;
	vaoID = 0;

	vertices = other.vertices;
	indices = other.indices;
	attributes = other.attributes;
	return *this;
}

template<vertex_comp... Cs>
mesh<Cs...>& mesh<Cs...>::operator=(mesh<Cs...>&& other){
	glDeleteBuffers(1, &vertexBufferID);
	glDeleteBuffers(1, &indexBufferID);

	vertices = std::move(other.vertices);
	indices = std::move(other.indices);
	attributes = std::move(other.attributes);

	vaoID = other.vaoID;
	vertexBufferID = other.vertexBufferID;
	indexBufferID = other.indexBufferID;

	other.vertexBufferID = 0;
	other.indexBufferID = 0;
	other.vaoID = 0;

	return *this;
}

template<vertex_comp... Cs>
void mesh<Cs...>::initVAO() {
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(u16), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

	constexpr_for<0, (1 + ... + sizeof(Cs)), 1>([](const auto i) {
		glVertexAttribPointer(i, vertex::sizeOf(i) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)vertex::offsetOf(i));
		glEnableVertexAttribArray(i);
	});

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

template<vertex_comp... Cs>
void mesh<Cs...>::addAttribute(meshEffect* attr) {
	attributes.push_back(attr);
}

template<vertex_comp... Cs>
void mesh<Cs...>::removeAttribute(meshEffect* attr) {
	attributes.erase(std::find(attributes.begin(), attributes.end(), attr));
}

template<vertex_comp... Cs>
meshInstance mesh<Cs...>::getInstance(const glm::mat4x4 modelMatrix) const {
	if (vaoID) {
		return meshInstance(vaoID, indices.size(), modelMatrix, attributes);
	} else {
		throw std::runtime_error("Cannot create mesh instance without initialized vao");
	}
}
