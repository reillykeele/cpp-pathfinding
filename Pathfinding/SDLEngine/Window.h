#pragma once

#include <string>

#include <SDL/SDL.h>

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
	private:
		SDL_Window* _sdlWindow;

		int _screenWidth;
		int _screenHeight;
	};

}