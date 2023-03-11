#pragma once

#include <string>

#include <SDL/SDL.h>

#include "Vertex.h"

namespace SDLEngine
{

	enum WindowFlags
	{
		INVISIBLE = 0x1,
		FULLSCREEN = 0x2,
		BORDERLESS = 0x4
	};

	class Window
	{
	public:
		Window();
		~Window();

		int create(std::string windowName, int screenWidth, int screenHeight, unsigned int currFlags);
		void swap();

		int getScreenWidth() const { return _screenWidth; }
		int getScreenHeight() const { return _screenHeight; }

		void setClearColor(int r, int g, int b, int a);

	private:
		SDL_Window* _sdlWindow;

		int _screenWidth;
		int _screenHeight;

		GLclampf _r;
		GLclampf _g;
		GLclampf _b;
		GLclampf _a;
	};

}
