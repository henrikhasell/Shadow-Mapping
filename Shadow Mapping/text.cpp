#include "text.hpp"
#include <glm/gtc/matrix_transform.hpp>

Text::Text() : position(0.0f, 0.0f)
{

}

void Text::render(Font &font, const char string[])
{
	font.render(shape, texture, string);
}

glm::mat4 Text::modelMatrix() const
{
	return glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
}