#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "window.hpp"

bool InitialiseApplication()
{
	if(SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		if(TTF_Init() == 0)
		{
			if(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == IMG_INIT_PNG | IMG_INIT_JPG)
			{
				return true;
			}
			else
			{
				std::cerr << "Failed to initialise SDL2_image: " << IMG_GetError() << std::endl;
			}

			TTF_Quit();
		}
		else
		{
			std::cerr << "Failed to initialise SDL2_ttf: " << TTF_GetError() << std::endl;
		}

		SDL_Quit();
	}
	else
	{
		std::cerr << "Failed to initialise SDL2: " << SDL_GetError() << std::endl;
	}

	return false;
}

void CleanupApplication()
{
	TTF_Quit();
	SDL_Quit();
}

int main(int argc, char *argv[])
{
	if(InitialiseApplication() == true)
	{
		Window window;

		if(window.initialise() == true)
		{
			bool finished = false;

			const Uint8 *keyboard = SDL_GetKeyboardState(nullptr);

			while(!finished)
			{
				SDL_Event event;

				while(SDL_PollEvent(&event) > 0)
				{
					if(event.type == SDL_QUIT)
					{
						finished = true;
					}
					else if(event.type == SDL_WINDOWEVENT)
					{
						window.handle(event.window);
					}
					else if(event.type == SDL_MOUSEMOTION)
					{
						SDL_SetRelativeMouseMode(event.motion.state ? SDL_TRUE : SDL_FALSE);
						window.handle(event.motion);
					}
				}

				window.handle(keyboard);
				window.draw();

				if(glGetError() != GL_NO_ERROR)
				{
					std::cerr << "OpenGL error occured!" << std::endl;
				}
			}
		}
	}

	CleanupApplication();

	return 0;
}