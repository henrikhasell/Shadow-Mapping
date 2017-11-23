#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL.h>
#include "graphics.hpp"
#include "camera.hpp"

class Window
{
public:
	Window();
	~Window();
	bool initialise();
	void handle(const SDL_MouseMotionEvent &motion);
	void handle(const SDL_WindowEvent &window);
	void handle(const Uint8 *keyboardState);
	void projection(int width, int height);
	void draw();
	Graphics *graphics;
private:
	SDL_Window *window;
	SDL_GLContext context;
	Camera camera;
	Uint32 lastTimeStep;
};

#endif