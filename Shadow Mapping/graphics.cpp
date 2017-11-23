#include "graphics.hpp"

Graphics::Graphics() : width(0), height(0)
{

}

bool Graphics::initialise()
{
	bool success = renderer.initialise() && orthographic.initialise() && shadowMap.initialise();

	if(success)
	{
		crateModel.load("models/crate/crate.obj");
		torusModel.load("models/torus/torus.obj");
		floorModel.load("models/floor/floor.obj");

		torusTexture.colour(1.0f, 0.0f, 0.0f);
		floorTexture.load("textures/Floor.jpg");

		crateObject.model = &crateModel;
		torusObject.model = &torusModel;
		floorObject.model = &floorModel;

		crateObject.texture = &crateTexture;
		torusObject.texture = &torusTexture;
		floorObject.texture = &floorTexture;

		crateObject.position.x = 0.0f;
		crateObject.position.y = 1.0f;
		crateObject.position.z = 0.0f;

		torusObject.position.x = 2.0f;
		torusObject.position.y = 2.5f;
		torusObject.position.z = 2.5f;

		defaultFont.load("fonts/NanumGothic-Regular.ttf", 22);
		infoText.render(defaultFont, "Press 'O' to manipulate the shadow direction.");
		infoText.position.y = 22.0f;

		shadowDisplayShape.square(0.0f, 44.0f, 100.0f, 100.0f);
		shadowDisplaySprite.shape = &shadowDisplayShape;

		setDirection(1.0f, 1.0f, 1.0f);
	}

	return success;
}

void Graphics::setDirection(GLfloat x, GLfloat y, GLfloat z)
{
	shadowMap.setDirection(x, y, z);
	renderer.view(shadowMap);

	const std::string message = "Sun direction: (" +
		std::to_string(shadowMap.direction.x) + ", " +
		std::to_string(shadowMap.direction.y) + ", " +
		std::to_string(shadowMap.direction.z) + ")";

	directionText.render(defaultFont, message.c_str());
}

void Graphics::projection(GLsizei width, GLsizei height)
{
	this->width = width;
	this->height = height;

	const GLfloat widthf = (GLfloat)width;
	const GLfloat heightf = (GLfloat)height;

	renderer.projection(widthf, heightf);
	orthographic.projection(widthf, heightf);
}

void Graphics::draw(const Camera &camera) const
{
	glViewport(0, 0, ShadowMap::hRes, ShadowMap::vRes);

	shadowMap.begin();
	shadowMap.draw(crateObject);
	shadowMap.draw(torusObject);
	shadowMap.draw(floorObject);
	shadowMap.end();

	glViewport(0, 0, width, height);

	renderer.begin();
	shadowMap.bind();
	renderer.view(camera);
	renderer.draw(crateObject);
	renderer.draw(torusObject);
	renderer.draw(floorObject);
	renderer.end();

	orthographic.begin();
	orthographic.draw(directionText);
	orthographic.draw(infoText);
	shadowMap.bind(GL_TEXTURE0);
	orthographic.draw(shadowDisplaySprite);
	orthographic.end();
}

void Graphics::step()
{
	torusObject.rotate(1.0f, 1.0f, 0.0f, 0.05f);
	torusObject.rotate(0.0f, 1.0f, 0.0f, 0.05f);
	torusObject.rotate(0.0f, 0.0f, 1.0f, 0.05f);
}