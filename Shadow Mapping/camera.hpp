#pragma once

#include <SDL2/SDL.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

struct Camera
{
	Camera();

	void handle(const SDL_MouseMotionEvent &motion);
	void handle(const Uint8 *keyboard);
	void look(float x, float y, float z);

	glm::vec3 forward() const;
	glm::mat4 view() const;

	glm::vec3 position;
	float pitch;
	float yaw;
};
