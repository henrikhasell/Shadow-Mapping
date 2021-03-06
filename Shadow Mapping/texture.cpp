#include "texture.hpp"
#include <iostream>
#include <SDL2/SDL_image.h>

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#   define R_MASK 0xFF000000
#   define G_MASK 0x00FF0000
#   define B_MASK 0x0000FF00
#   define A_MASK 0x000000FF
#else
#   define R_MASK 0x000000FF
#   define G_MASK 0x0000FF00
#   define B_MASK 0x00FF0000
#   define A_MASK 0xFF000000
#endif

Texture::Texture() : w(0), h(0)
{
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	const GLubyte pixels[] = {
		0xff, 0xff, 0xff, 0xff,
		0xaa, 0xaa, 0xaa, 0xff,
		0xaa, 0xaa, 0xaa, 0xff,
		0xff, 0xff, 0xff, 0xff
	};

	glTexImage2D(GL_TEXTURE_2D, 0, 4,
		2,
		2, 0,
		GL_RGBA, GL_UNSIGNED_BYTE,
		pixels
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Texture::~Texture()
{
	glDeleteTextures(1, &texture);
}

static inline Uint8 *getPixel(SDL_Surface *surface, int x, int y)
{
	return (Uint8*)surface->pixels + (surface->pitch * y) + (x * surface->format->BytesPerPixel);
}

bool Texture::load(const char path[])
{
	bool result = false;

	SDL_Surface *surface = IMG_Load(path);

	if(surface)
	{
		result = load(surface);
		SDL_FreeSurface(surface);
	}

	if(!result)
	{
		std::cerr << "Failed to load " << path << std::endl;
	}

	return result;
}

bool Texture::load(SDL_Surface *surface)
{
	SDL_Surface *optimised = SDL_CreateRGBSurface(0, surface->w, surface->h, 32, R_MASK, G_MASK, B_MASK, A_MASK);

	if(optimised)
	{
		SDL_FillRect(optimised, NULL, 0x000000ff);
		SDL_SetSurfaceAlphaMod(surface, 0xFF);
		SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);

		SDL_BlitSurface(surface, NULL, optimised, NULL);

		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D, 0, 4,
			optimised->w,
			optimised->h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE,
			optimised->pixels
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		w = optimised->w;
		h = optimised->h;

		SDL_FreeSurface(optimised);
	}

	return optimised != NULL;
}

void Texture::colour(GLfloat r, GLfloat g, GLfloat b)
{
	glBindTexture(GL_TEXTURE_2D, texture);

	const GLfloat pixels[] = {
		r, g, b, 0xff
	};

	glTexImage2D(GL_TEXTURE_2D, 0, 4,
		1,
		1, 0,
		GL_RGBA, GL_FLOAT,
		pixels
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}


void Texture::bind(GLenum texture) const
{
	glActiveTexture(texture);
	glBindTexture(GL_TEXTURE_2D, this->texture);
}

int Texture::getW()
{
	return w;
}

int Texture::getH()
{
	return h;
}

#undef R_MASK
#undef G_MASK
#undef B_MASK
#undef A_MASK