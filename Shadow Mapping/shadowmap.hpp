#ifndef SHADOWMAP_HPP
#define SHADOWMAP_HPP

#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "program.hpp"
#include "shader.hpp"
#include "object.hpp"
#include "model.hpp"
#include "mesh.hpp"

class ShadowMap
{
public:
	static constexpr GLsizei hRes = 2048;
	static constexpr GLsizei vRes = 2048;

	glm::mat4 projectionMatrix;
	glm::vec3 direction;

	ShadowMap();
	~ShadowMap();

	bool initialise();
	void setDirection(GLfloat x, GLfloat y, GLfloat z);
	void begin() const;
	void end() const;
	void draw(const Object &object) const;
	void draw(const Model &model) const;
	void draw(const Mesh &mesh) const;
	void bind(GLenum texture = GL_TEXTURE1) const;
private:
	Program program;
	Shader vertexShader;
	Shader fragmentShader;

	GLuint frameBuffer;
	GLuint depthTexture;

	GLuint uniform_projectionMatrix;
	GLuint uniform_modelMatrix;
};


#endif