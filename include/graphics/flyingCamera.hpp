#pragma once

#include <graphics/camera.hpp>

class flyingCamera : public camera {
public:

	flyingCamera(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& worldUp) :
		camera(position, direction, worldUp) {}


	void update(float deltaT, int dx, int dy) override;

};
