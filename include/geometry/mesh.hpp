#pragma once
#include <GL/glew.h> 
#include <SFML/OpenGL.hpp>

#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <utils/intX.hpp>
#include <utils/primitives.hpp>
#include <utils/packedTuple.hpp>
#include <geometry/vertex_comp.hpp>

#include <graphics/meshEffects.hpp>
#include <graphics/shaderImpl.hpp>

#include <graphics/renderable.hpp>
#include <physics/collidable.hpp>
#include <physics/aabb.hpp>

template<vertex_comp... Cs>
class mesh {
public:
	using vertex = packedTuple<vertex_comps::position::type, typename Cs::type...>;
	
	mesh(
		std::vector<typename mesh<Cs...>::vertex>&& vertices,
		std::vector<u16>&& indices
	);

	mesh(
		const std::vector<typename mesh<Cs...>::vertex>& vertices,
		const std::vector<u16>& indices
	);
	
	mesh(const mesh<Cs...>& other);
	mesh(mesh<Cs...>&& other);

	mesh<Cs...>& operator=(const mesh<Cs...>& other);
	mesh<Cs...>& operator=(mesh<Cs...>&& other);

	~mesh();

	void initVAO();

	const std::vector<vertex>& getVertexBuffer() const;

	const std::vector<u16>& getIndexBuffer() const;

	renderable getRenderable(const glm::mat4x4 modelMatrix = glm::identity<glm::mat4x4>()) const;

	renderable getCollidable(const glm::mat4x4 modelMatrix = glm::identity<glm::mat4x4>()) const;

	aabb getBoundingBox() const;

protected:

	std::vector<vertex> vertices;
	std::vector<u16> indices;
	
	GLuint vertexBufferID{ 0 };
	GLuint indexBufferID{ 0 };
	GLuint vaoID{ 0 };

	aabb boundingBox;

public:
	const meshColor* myColor{ nullptr };
	const meshTexture* myTexture{ nullptr };

};
