#include "shadowmap.hpp"
#include <iostream>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

ShadowMap::ShadowMap() :
	vertexShader(GL_VERTEX_SHADER),
	fragmentShader(GL_FRAGMENT_SHADER),
	projectionMatrix(1.0f)
{
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, vRes, hRes, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ShadowMap::~ShadowMap()
{
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteTextures(1, &depthTexture);
}

bool ShadowMap::initialise()
{
	bool success = vertexShader.load("shaders/shadowmap/vertex.glsl") && fragmentShader.load("shaders/shadowmap/fragment.glsl");

	if(success)
	{
		success = program.link(vertexShader, fragmentShader);

		if(success)
		{
			program.bindAttributeLocation(0, "in_Position");
			uniform_projectionMatrix = program.getUniformLocation("projectionMatrix");
			uniform_modelMatrix = program.getUniformLocation("modelMatrix");
		}
		else
		{
			std::cerr << "Failed to link program." << std::endl;
		}
	}

	return success;
}

void ShadowMap::setDirection(GLfloat x, GLfloat y, GLfloat z)
{
	direction = glm::normalize(glm::vec3(x, y, z));

	projectionMatrix = glm::ortho(
		-10.0f,
		+10.0f,
		+10.0f,
		-10.0f,
		-10.0f,
		+20.0f);

	projectionMatrix *= glm::lookAt(
		direction * 10.0f,
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));

	program.use();
	glUniformMatrix4fv(uniform_projectionMatrix, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void ShadowMap::begin() const
{
	program.use();
	glEnableVertexAttribArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ShadowMap::end() const
{
	glDisableVertexAttribArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::draw(const Object &object) const
{
	if(object.model)
	{
		const glm::mat4 modelMatrix = object.modelMatrix();
		glUniformMatrix4fv(uniform_modelMatrix, 1, GL_FALSE, &modelMatrix[0][0]);
		draw(*object.model);
	}
}

void ShadowMap::draw(const Model &model) const
{
	for(const Mesh &mesh : model.meshes)
	{
		draw(mesh);
	}
}

void ShadowMap::draw(const Mesh &mesh) const
{
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBuffer);
	glDrawElements(GL_TRIANGLES, mesh.indices, GL_UNSIGNED_INT, (void*)0);
}

void ShadowMap::bind(GLenum texture) const
{
	glActiveTexture(texture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
}