#include "sprite.hpp"
#include <glm/gtc/matrix_transform.hpp>

Sprite::Sprite() :
	texture(nullptr),
	shape(nullptr)
{

}

glm::mat4 Sprite::modelMatrix() const
{
	return glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
}