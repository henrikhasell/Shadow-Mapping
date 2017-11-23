#ifndef TEXT_HPP
#define TEXT_HPP

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include "texture.hpp"
#include "sprite.hpp"
#include "font.hpp"

class Text
{
	friend class Orthographic;
public:
	Text();
	void render(Font &font, const char string[]);
	glm::mat4 modelMatrix() const;
	glm::vec2 position;
private:
	Texture texture;
	Shape shape;
};

#endif