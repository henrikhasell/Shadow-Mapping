#include "object.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

Object::Object() :
	position(0.0f, 0.0f, 0.0f),
	rotation(1.0f, 0.0f, 0.0f, 0.0f),
	texture(nullptr),
	model(nullptr)
{

}

void Object::rotate(
	GLfloat x,
	GLfloat y,
	GLfloat z,
	GLfloat amount)
{
	rotation = glm::rotate(rotation, amount, glm::vec3(x, y, z));
}

glm::mat4 Object::modelMatrix() const
{
	return glm::translate(glm::mat4(1.0f), position) * glm::toMat4(rotation);
}