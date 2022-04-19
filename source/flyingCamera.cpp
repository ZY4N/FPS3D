#include <graphics/flyingCamera.hpp>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cmath>

void flyingCamera::update(float deltaT, int dx, int dy) {

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
