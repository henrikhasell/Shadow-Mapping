#include "font.hpp"
#include <iostream>

Font::Font() : font(nullptr)
{

}

Font::~Font()
{
	if(font)
	{
		TTF_CloseFont(font);
	}
}

bool Font::load(const char path[], int size)
{
	if(font)
	{
		TTF_CloseFont(font);
	}

	font = TTF_OpenFont(path, size);

	if(font == nullptr)
	{
		std::cerr << "Failed to open font " << path << std::endl;
	}

	return font != nullptr;
}


void Font::render(
	Shape &shape,
	Texture &texture,
	const char string[])
{
	SDL_Colour colour;

	colour.r = 0xff;
	colour.g = 0xff;
	colour.b = 0xff;
	colour.a = 0xff;

	SDL_Surface *surface = TTF_RenderText_Blended(font, string, colour);

	if(surface)
	{
		shape.square(
			0.0f,
			0.0f,
			(GLfloat)surface->w,
			(GLfloat)surface->h);
		texture.load(surface);
	}
}
