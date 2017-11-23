#include "renderer.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Renderer::Renderer() :
	vertexShader(GL_VERTEX_SHADER),
	fragmentShader(GL_FRAGMENT_SHADER)
{

}

bool Renderer::initialise()
{
	bool success = vertexShader.load("shaders/world/vertex.glsl") && fragmentShader.load("shaders/world/fragment.glsl");

	if(success)
	{
		success = program.link(vertexShader, fragmentShader);

		if(success)
		{
			program.bindAttributeLocation(0, "in_Position");
			program.bindAttributeLocation(1, "in_Normal");
			program.bindAttributeLocation(2, "in_TexCoord");

			uniform_projectionMatrix = program.getUniformLocation("projectionMatrix");
			uniform_shadowMatrix = program.getUniformLocation("shadowMatrix");
			uniform_modelMatrix = program.getUniformLocation("modelMatrix");
			uniform_viewMatrix = program.getUniformLocation("viewMatrix");
			uniform_viewPosition = program.getUniformLocation("viewPosition");
			uniform_sunDirection = program.getUniformLocation("sunDirection");

			const GLint textureSampler = program.getUniformLocation("textureSampler");
			const GLint shadowSampler = program.getUniformLocation("shadowSampler");

			program.use();

			glUniform1i(textureSampler, 0);
			glUniform1i(shadowSampler, 1);
		}
		else
		{
			std::cerr << "Failed to link program." << std::endl;
		}

	}

	return success;
}

void Renderer::projection(float width, float height) const
{
	glm::mat4 projection = glm::perspective(45.0f, width / height, 0.1f, 1000.0f);
	program.use();
	glUniformMatrix4fv(uniform_projectionMatrix, 1, GL_FALSE, &projection[0][0]);
}

void Renderer::view(const ShadowMap &shadowMap) const
{
	program.use();

	const glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	const glm::mat4 shadowMatrix = biasMatrix * shadowMap.projectionMatrix;
	glUniformMatrix4fv(uniform_shadowMatrix, 1, GL_FALSE, &shadowMatrix[0][0]);
	glUniform3fv(uniform_sunDirection, 1, &shadowMap.direction[0]);
}

void Renderer::view(const Camera &camera) const
{
	const glm::mat4 view = camera.view();
	glUniformMatrix4fv(uniform_viewMatrix, 1, GL_FALSE, &view[0][0]);
	glUniform3fv(uniform_viewPosition, 1, &camera.position[0]);
}

void Renderer::begin() const
{
	program.use();
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::end() const
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Renderer::draw(const Object &object) const
{
	if(object.model)
	{
		const glm::mat4 modelMatrix = object.modelMatrix();
		glUniformMatrix4fv(uniform_modelMatrix, 1, GL_FALSE, &modelMatrix[0][0]);

		if(object.texture)
		{
			object.texture->bind();
		}

		draw(*object.model);
	}
}

void Renderer::draw(const Model &model) const
{
	for(const Mesh &mesh : model.meshes)
	{
		draw(mesh);
	}
}

void Renderer::draw(const Mesh &mesh) const
{
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.normalBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.texCoordBuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBuffer);
	glDrawElements(GL_TRIANGLES, mesh.indices, GL_UNSIGNED_INT, (void*)0);
}