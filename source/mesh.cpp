#include <mesh.hpp>

#include <packedTuple.hpp>
#include <primitives.hpp>
#include <utils.hpp>

#include <shader.hpp>

template<vertex_comp... Cs>
mesh<Cs...>::mesh(
	std::vector<typename mesh<Cs...>::vertex>&& vertexBuffer,
	std::vector<u16>&& indexBuffer
) : vertexBuffer{ std::move(vertexBuffer) }, indexBuffer{ std::move(indexBuffer) } {}

template<vertex_comp... Cs>
void mesh<Cs...>::initVAO() {

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size(), &vertexBuffer[0], GL_STATIC_DRAW);

	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.size() * sizeof(u16), &indexBuffer[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	static constexpr size_t componentSizes[]{ sizeof(typename Cs::type)... };
	size_t byteOffset = 0;
	for (size_t i = 0; i < sizeof...(Cs); i++) {
		glVertexAttribPointer(i, componentSizes[i], GL_FLOAT, GL_FALSE, sizeof(Cs)..., (GLvoid*)byteOffset);
		glEnableVertexAttribArray(i);
		byteOffset += componentSizes[i];
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

template<vertex_comp... Cs>
mesh<Cs...>::~mesh() {
	glDeleteBuffers(1, &vertexBufferID);
	glDeleteBuffers(1, &indexBufferID);
}

template<vertex_comp... Cs>
meshInstance mesh<Cs...>::getInstance(const glm::mat4x4 modelMatrix) const {
	if (vaoID) {
		return meshInstance(vaoID, indices.size(), modelMatrix, attributes);
	} else {
		throw std::runtime_error("Cannot create mesh instance without initialized vao");
	}
}

template<vertex_comp... Cs>
mesh<Cs...>::mesh(const mesh<Cs...>& other) :
	vertices{ other.vertices },
	indices{ other.indices },
	attributes{ other.attributes } {}

template<vertex_comp... Cs>
mesh<Cs...>::mesh(mesh<Cs...>&& other) :
	vertices{ std::move(other.vertices) },
	indices{ std::move(other.indices) },
	vaoID{ other.vaoID },
	vertexBufferID{ other.vertexBufferID },
	indexBufferID{ other.indexBufferID },
	attributes{ std::move(other.attributes) }
{	
	other.vaoID = 0;
	other.vertexBufferID = 0;
	other.indexBufferID = 0;
}

template<vertex_comp... Cs>
mesh<Cs...>& mesh<Cs...>::operator=(const mesh<Cs...>& other) {	
	vertices = other.vertices;
	indices = other.indices;
	attributes = other.attributes;
	glDeleteBuffers(1, &vertexBufferID);
	glDeleteBuffers(1, &indexBufferID);
	return *this;
}

template<vertex_comp... Cs>
mesh<Cs...>& mesh<Cs...>::operator=(mesh<Cs...>&& other) {
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




