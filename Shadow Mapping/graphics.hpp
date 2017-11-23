#pragma once

#include "renderer.hpp"
#include "orthographic.hpp"
#include "shadowmap.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "object.hpp"
#include "font.hpp"
#include "text.hpp"
#include "shape.hpp"

class Graphics
{
public:
	Graphics();
	bool initialise();
	void setDirection(GLfloat x, GLfloat y, GLfloat z);
	void projection(GLsizei width, GLsizei height);
	void draw(const Camera &camera) const;
	void step();
private:
	Renderer renderer;
	Orthographic orthographic;
	ShadowMap shadowMap;

	Model crateModel;
	Model torusModel;
	Model floorModel;

	Object crateObject;
	Object torusObject;
	Object floorObject;

	Texture crateTexture;
	Texture torusTexture;
	Texture floorTexture;

	Font defaultFont;
	Text directionText;
	Text infoText;

	Shape shadowDisplayShape;
	Sprite shadowDisplaySprite;

	GLsizei width;
	GLsizei height;
};