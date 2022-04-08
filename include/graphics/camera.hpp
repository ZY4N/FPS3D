#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <SFML/System/Vector2.hpp>


class camera {
public:
	camera(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& worldUp);

	void updateMatrix();

	void update(float deltaT, int dx, int dy);

	const glm::mat4 getViewMatrix();

private:

	glm::vec3 position, velocity, viewDirection;
	glm::mat4 matrix;

	glm::vec3 worldUp, front, right, up;

	float pitch, yaw, roll;

};
