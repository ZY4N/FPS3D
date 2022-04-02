#include <camera.hpp>

#include <cmath>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>

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

void camera::update(float deltaT, int dx, int dy) {

	static constexpr float maxSpeed = 0.01f;
	static constexpr float mouseSensitivity = 0.0001f;
	static constexpr float friction = 1.0f - 0.6f;

	yaw += dx * mouseSensitivity * deltaT;
	pitch -= dy * mouseSensitivity * deltaT;

	yaw = fmod(yaw, 2 * M_PI);
	pitch = std::min(std::max(pitch, (float)-M_PI), (float)M_PI);

	glm::vec3 acceleration(0.f, 0.f, 0.f);
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		acceleration += front;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		acceleration -= front;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		acceleration -= right;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		acceleration += right;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		acceleration += worldUp;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
		acceleration -= worldUp;
	
	if (glm::length(acceleration) > glm::epsilon<float>())
		velocity += glm::normalize(acceleration);
	
	velocity *= friction;
	
	const float speed = glm::length(velocity);
	if (speed > maxSpeed)
		velocity *= maxSpeed / speed;

	const float speedBonus = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? 2.f : 1.f;
	position += velocity * speedBonus * deltaT;

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
