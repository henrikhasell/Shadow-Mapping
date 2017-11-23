#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include "texture.hpp"
#include "model.hpp"

struct Object
{
	Object();

	void rotate(
		GLfloat x,
		GLfloat y,
		GLfloat z,
		GLfloat amount);

	glm::mat4 modelMatrix() const;

	glm::vec3 position;
	glm::quat rotation;
	
	Texture *texture;
	Model *model;
};

#endif