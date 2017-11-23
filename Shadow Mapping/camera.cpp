#include "camera.hpp"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#ifndef M_PI_2
// Windows likes to exclude this and
// complain about double/float casts.
#define M_PI_2 1.57079632679489661923f
#endif

Camera::Camera() : position(-9.0f, 10.0f, 9.0f), pitch(0), yaw(0)
{
	look(0.0f, 0.0f, 0.0f);
}

void Camera::handle(const SDL_MouseMotionEvent &motion)
{
	constexpr float sensitivity = 0.005f;

	const float xAmount = (float)motion.xrel * sensitivity;
	const float yAmount = (float)motion.yrel * sensitivity;

	constexpr float minPitch = -M_PI_2 + 0.0001f;
	constexpr float maxPitch = +M_PI_2 - 0.0001f;

	pitch -= yAmount;

	if(pitch > maxPitch)
	{
		pitch = maxPitch;
	}
	else if(pitch < minPitch)
	{
		pitch = minPitch;
	}

	yaw -= xAmount;

}

void Camera::handle(const Uint8 *keyboard)
{
	constexpr float speed = 0.1f;

	glm::vec3 direction(0.0f, 0.0f, 0.0f);

	if(keyboard[SDL_SCANCODE_W])
	{
		direction += forward();
	}
	if(keyboard[SDL_SCANCODE_S])
	{
		direction -= forward();
	}
	if(keyboard[SDL_SCANCODE_A])
	{
		direction.x += cos(yaw);
		direction.z -= sin(yaw);
	}
	if(keyboard[SDL_SCANCODE_D])
	{
		direction.x -= cos(yaw);
		direction.z += sin(yaw);
	}

	if(direction.x != 0.0f || direction.y != 0.0f || direction.z != 0.0f)
	{
		position += glm::normalize(direction) * speed;
	}
}

void Camera::look(float x, float y, float z)
{
	const float xDiff = x - position.x;
	const float zDiff = z - position.z;
	const float yDiff = y - position.y;

	const float hDiff = sqrtf(xDiff * xDiff + zDiff * zDiff);

	pitch = atan2f(yDiff, hDiff);
	yaw = atan2f(xDiff, zDiff);
}

glm::mat4 Camera::view() const
{
	return glm::lookAt(position, position + forward(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 Camera::forward() const
{
	return glm::vec3(cos(pitch) * sin(yaw), sin(pitch), cos(pitch) * cos(yaw));
}