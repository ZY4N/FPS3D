#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <SFML/System/Vector2.hpp>


class camera {
public:
	camera(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& worldUp);

	void updateMatrix();

	const glm::mat4 getViewMatrix();

	virtual void update(float deltaT, int dx, int dy) = 0;

private:
	glm::mat4 matrix;

protected:

	glm::vec3 position, velocity, viewDirection;
	glm::vec3 worldUp, front, right, up;

	float pitch, yaw, roll;

};
