#pragma once
#include <geometry/mesh.hpp>

template<vertex_comp... Cs>
mesh<Cs...>::mesh(
	const std::vector<typename mesh<Cs...>::vertex>& vertexBuffer,
	const std::vector<u16>& indexBuffer
) : vertices{ vertexBuffer }, indices{ indexBuffer } {
	/*
	std::cout << "vertices:\n";
	for (size_t i = 0; i < vertexBuffer.size(); i++) {
		float* numbers = (float*)&vertexBuffer[i];
		std::cout << "{ { ";
		for (size_t j = 0; j < 3; j++) {
			std::cout << numbers[j] << ", ";
		}
		std::cout << "}, { ";

		for (size_t j = 0; j < 2; j++) {
			std::cout << numbers[3 + j] << ", ";
		}

		std::cout << "}, { ";

		for (size_t j = 0; j < 3; j++) {
			std::cout << numbers[5 + j] << ", ";
		}
		std::cout << "} },\n";
	}

	std::cout << "indices:\n";
	for (u16 index : indices) {
		std::cout << index << ' ';
	}
	std::cout << std::endl;
*/
}

template<vertex_comp... Cs>
mesh<Cs...>::mesh(
	std::vector<typename mesh<Cs...>::vertex>&& vertexBuffer,
	std::vector<u16>&& indexBuffer
) : vertices{ std::move(vertexBuffer) }, indices{ std::move(indexBuffer) } {
}

template<vertex_comp... Cs>
mesh<Cs...>::~mesh() {
	if (vertexBufferID)
		glDeleteBuffers(1, &vertexBufferID);
	if (indexBufferID)
		glDeleteBuffers(1, &indexBufferID);
}

template<vertex_comp... Cs>
mesh<Cs...>::mesh(const mesh<Cs...>& other) :
	vertices{ other.vertices },
	indices{ other.indices },
	myColor{ other.myColor },
	myTexture{ other.myTexture }
{}

template<vertex_comp... Cs>
mesh<Cs...>::mesh(mesh<Cs...>&& other):
	vertices{ std::move(other.vertices) },
	indices{ std::move(other.indices) },
	vaoID{ other.vaoID },
	vertexBufferID{ other.vertexBufferID },
	indexBufferID{ other.indexBufferID },
	myColor{ other.myColor },
	myTexture{ other.myTexture }
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
	myColor = other.myColor;
	myTexture = other.myTexture;

	return *this;
}

template<vertex_comp... Cs>
mesh<Cs...>& mesh<Cs...>::operator=(mesh<Cs...>&& other){
	glDeleteBuffers(1, &vertexBufferID);
	glDeleteBuffers(1, &indexBufferID);

	vertices = std::move(other.vertices);
	indices = std::move(other.indices);
	
	vaoID = other.vaoID;
	vertexBufferID = other.vertexBufferID;
	indexBufferID = other.indexBufferID;

	myColor = other.myColor;
	myTexture = other.myTexture;

	other.vertexBufferID = 0;
	other.indexBufferID = 0;
	other.vaoID = 0;

	other.myColor = nullptr;
	other.myTexture = nullptr;

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
renderable mesh<Cs...>::getRenderable(const glm::mat4x4 modelMatrix) const {
	if (vaoID) {
		return renderable(vaoID, indices.size(), modelMatrix, myColor, myTexture);
	} else {
		throw std::runtime_error("Cannot create mesh instance without initialized vao");
	}
}

template<vertex_comp... Cs>
renderable mesh<Cs...>::getCollidable(const glm::mat4x4 modelMatrix) const {
	return renderable(vaoID, indices.size(), modelMatrix, myColor, myTexture);
}

template<vertex_comp... Cs>
aabb mesh<Cs...>::getBoundingBox() const {
	return aabb::createBoundingBox(&vertices[0], vertices.size());
}


template<vertex_comp... Cs>
const std::vector<typename mesh<Cs...>::vertex>& mesh<Cs...>::getVertexBuffer() const {
	return vertices;
}

template<vertex_comp... Cs>
const std::vector<u16>& mesh<Cs...>::getIndexBuffer() const {
	return indices;
}
