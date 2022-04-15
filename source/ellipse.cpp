#include <geometry/meshes/ellipse.hpp>

shapeMesh meshes::createEllipse(u32 detail) {

	std::vector<typename shapeMesh::vertex> vertexBuffer;
	std::vector<u16> indexBuffer;

	vertexBuffer.reserve(detail + 1);
	indexBuffer.reserve(detail * 3);

	const glm::vec3 normal{ 0, 1, 0 };

	vertexBuffer.emplace_back(
		glm::vec3{ 0.0f, 0, 0.0f },
		glm::vec2{ 0.5f, 0.5f },
		normal
	);

	vertexBuffer.emplace_back(
		glm::vec3{ 0.5f, 0, 0.0f },
		glm::vec2{ 1.0f, 0.5f },
		normal
	);

	float angleSize = 2.0f * M_PI / detail;
	for (u32 i = 0; i < detail; i++) {
		float angle = i * angleSize;

		float x = cos(angle) * 0.5;
		float y = sin(angle) * 0.5;

		vertexBuffer.emplace_back(
			glm::vec3{ x, 0, y },
			glm::vec2{ x + 0.5f, y + 0.5f },
			normal
		);

		size_t newIndex = vertexBuffer.size() - 1; 

		indexBuffer.push_back(newIndex - 1);
		indexBuffer.push_back(newIndex);
		indexBuffer.push_back(0);

	}


	indexBuffer.push_back(vertexBuffer.size() - 1);
	indexBuffer.push_back(1);
	indexBuffer.push_back(0);

	return shapeMesh(std::move(vertexBuffer), std::move(indexBuffer));

}


