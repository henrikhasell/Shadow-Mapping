#include "window.hpp"
#include <iostream>

Window::Window() :
	window(nullptr),
	context(nullptr),
	graphics(nullptr)
{
	lastTimeStep = SDL_GetTicks();
}

Window::~Window()
{
	if(graphics)
	{
		delete graphics;
	}
	if(context)
	{
		SDL_GL_DeleteContext(context);
	}
	if(window)
	{
		SDL_DestroyWindow(window);
	}
}

bool Window::initialise()
{
	window = SDL_CreateWindow("Renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	if(window)
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		context = SDL_GL_CreateContext(window);

		if(!context)
		{
			std::cerr << "Failed to create OpenGL context." << SDL_GetError() << std::endl;
		}
		else
		{
			GLenum glewStatus = glewInit();

			if(glewStatus == GLEW_OK)
			{
				graphics = new Graphics();

				bool success = graphics->initialise();

				if(success)
				{
					glEnable(GL_DEPTH_TEST);
					glEnable(GL_CULL_FACE);
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

					projection(800, 600);
				}

				return success;
			}
			else
			{
				std::cerr << "Failed to initialise GLEW: " << glewGetErrorString(glewStatus) << std::endl;
			}
		}
	}
	else
	{
		std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
	}

	return false;
}

void Window::projection(int width, int height)
{
	graphics->projection(width, height);
}

void Window::handle(const SDL_MouseMotionEvent &motion)
{
	if(SDL_GetRelativeMouseMode() == GL_TRUE)
	{
		camera.handle(motion);
	}
}

void Window::handle(const SDL_WindowEvent &window)
{
	if(window.event == SDL_WINDOWEVENT_RESIZED)
	{
		const Sint32 w = window.data1;
		const Sint32 h = window.data2;

		projection(w, h);
	}
}

void Window::handle(const Uint8 *keyboardState)
{
	for(Uint32 time = SDL_GetTicks(); lastTimeStep - time > 1000 / 60; lastTimeStep += 1000 / 60)
	{
		camera.handle(keyboardState);
		graphics->step();
	}
	if(keyboardState[SDL_SCANCODE_O])
	{
		const glm::vec3 direction = -camera.forward();

		graphics->setDirection(
			direction.x,
			direction.y,
			direction.z);
	}
}

void Window::draw()
{
	graphics->draw(camera);
	SDL_GL_SwapWindow(window);
}