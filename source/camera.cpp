#include <graphics/camera.hpp>

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
	static constexpr float maxAngle = (M_PI / 2.0f) - glm::epsilon<float>();
	pitch = std::min(std::max(pitch, -maxAngle), maxAngle);

	glm::vec3 acceleration(0.f, 0.f, 0.f);
	
	using kb = sf::Keyboard;
	static const glm::fvec3 noMove{ 0, 0, 0 }; 
	acceleration += kb::isKeyPressed(kb::W) ? front : kb::isKeyPressed(kb::S) ? -front : noMove;
	acceleration += kb::isKeyPressed(kb::D) ? right : kb::isKeyPressed(kb::A) ? -right : noMove;
	acceleration += kb::isKeyPressed(kb::Space) ? worldUp : kb::isKeyPressed(kb::LControl) ? -worldUp : noMove;
	
	if (glm::length(acceleration) > glm::epsilon<float>())
		velocity += glm::normalize(acceleration);
	
	velocity *= friction;
	
	const float speed = glm::length(velocity);
	if (speed > maxSpeed)
		velocity *= maxSpeed / speed;

	const float speedBonus = kb::isKeyPressed(kb::LShift) ? 2.f : 1.f;
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
