#include <graphics/camera.hpp>

#include <cmath>

camera::camera(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& worldUp) :
	position{ position }, viewDirection{ direction },  worldUp{ worldUp }
{
	matrix = glm::mat4(1.f);
	velocity = glm::vec3(0.f, 0.f, 0.f);

	right = glm::vec3(0.f);
	up = worldUp;

	pitch = 0.f;
	yaw = glm::radians(-90.f);
	roll = 0.f;

	updateMatrix();
}

void camera::updateMatrix() {
	front.x = cos(yaw) * cos(pitch);
	front.y = sin(pitch);
	front.z = sin(yaw) * cos(pitch);

	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));

	matrix = glm::lookAt(position, position + front, up);
}

const glm::mat4 camera::getViewMatrix() {
	return matrix;
}
