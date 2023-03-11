#include "Window.h"

#include <GL/glew.h>

#include "Errors.h"

namespace SDLEngine
{

	Window::Window(): _sdlWindow(nullptr), _screenWidth(0), _screenHeight(0), _r(0), _g(0), _b(0), _a(1)
	{
	}

	Window::~Window()
	{
	}

	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned currFlags)
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

		glClearColor(_r, _g, _b, _a);

		// Set VSYNC
		SDL_GL_SetSwapInterval(0);

		// Enable alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}

	void Window::swap() { SDL_GL_SwapWindow(_sdlWindow); }

	void Window::setClearColor(int r, int g, int b, int a)
	{
		_r = r / 255.0;
		_g = g / 255.0;
		_b = b / 255.0;
		_a = a / 255.0;

		glClearColor(_r, _g, _b, _a);
	}
}
