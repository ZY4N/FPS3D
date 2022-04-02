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
	yaw = -90.f;
	roll = 0.f;

	calculateVectors();
}


void camera::update(float deltaT, int dx, int dy) {

	static constexpr float maxSpeed = 0.01f;
	static constexpr float mouseSensitivity = 0.01f;
	static constexpr float friction = 1.0f - 0.6f;

	yaw += dx * mouseSensitivity * deltaT;
	pitch -= dy * mouseSensitivity * deltaT;

	yaw = fmod(yaw, 360.f);
	pitch = std::min(std::max(pitch, -89.f), 89.f);

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
}

void camera::calculateVectors() {
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

const glm::mat4 camera::getViewMatrix() {
	calculateVectors();
	return matrix = glm::lookAt(position, position + front, up);
}
