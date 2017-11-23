#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "shadowmap.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "program.hpp"
#include "texture.hpp"
#include "object.hpp"
#include "model.hpp"
#include "mesh.hpp"

class Renderer
{
public:
	Renderer();
	bool initialise();
	void projection(float width, float height) const;
	void view(const ShadowMap &shadowMap) const;
	void view(const Camera &camera) const;
	void begin() const;
	void end() const;
	void draw(const Object &object) const;
	void draw(const Model &model) const;
	void draw(const Mesh &mesh) const;
private:
	Shader vertexShader;
	Shader fragmentShader;
	Program program;

	GLuint uniform_projectionMatrix;
	GLuint uniform_shadowMatrix;
	GLuint uniform_modelMatrix;
	GLuint uniform_viewMatrix;
	GLuint uniform_viewPosition;
	GLuint uniform_sunDirection;
};

#endif