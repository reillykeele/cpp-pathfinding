#include "Window.h"

#include <GL/glew.h>

#include "Errors.h"

namespace SDLEngine
{

	Window::Window(): _sdlWindow(nullptr), _screenWidth(0), _screenHeight(0)
	{
	}

	Window::~Window()
	{
	}

	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned currFlags, ColorRGBA8 clearColor)
	{
		Uint32 flags = SDL_WINDOW_OPENGL;
		if (currFlags & INVISIBLE)
			flags |= SDL_WINDOW_HIDDEN;
		if (currFlags & FULLSCREEN)
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		if (currFlags & BORDERLESS)
			flags |= SDL_WINDOW_BORDERLESS;

		_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);

		if (_sdlWindow == nullptr)
			fatalError("Failed to create SDL window.");

		auto glContext = SDL_GL_CreateContext(_sdlWindow);
		if (glContext == nullptr)
			fatalError("Failed to create GL context.");

		auto glewErr = glewInit();
		if (glewErr != GLEW_OK)
			fatalError("Failed to initialize GLEW.");

		std::printf("*** OpenGL Version: %s ***\n", glGetString(GL_VERSION));

		glClearColor(clearColor.r / 255.0f, clearColor.g / 255.0f, clearColor.b / 255.0f, clearColor.a / 255.0f);

		// Set VSYNC
		SDL_GL_SetSwapInterval(0);

		// Enable alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}

	void Window::swap() { SDL_GL_SwapWindow(_sdlWindow); }

}