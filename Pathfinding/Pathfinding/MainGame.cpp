#include "MainGame.h"

#include <iostream>
#include <string>

void fatalError(std::string errMsg)
{
	std::cout << errMsg << std::endl;

	SDL_Quit();
}

MainGame::MainGame()
{
	_window = nullptr;

	_screenWidth = 600;
	_screenHeight = 400;

	_gameState = GameState::PLAY;
}

MainGame::~MainGame() = default;

void MainGame::run()
{
	initSystems();

	gameLoop();
}

void MainGame::initSystems()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	_window = SDL_CreateWindow("Pathfinding", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);

	if(_window == nullptr)
		fatalError("Failed to create SDL window.");

	auto glContext = SDL_GL_CreateContext(_window);
	if(glContext == nullptr)
		fatalError("Failed to create GL context.");

	auto glewErr = glewInit();
	if (glewErr != GLEW_OK)
		fatalError("Failed to initialize GLEW.");

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void MainGame::gameLoop()
{
	while(_gameState != GameState::EXIT)
	{
		processInput();

		_sprite.init(-1.0f, -1.0f, 1.0f, 1.0f);

		drawGame();
	}
}

void MainGame::processInput()
{
	SDL_Event ev;

	while(SDL_PollEvent(&ev))
	{
		switch(ev.type)
		{
			case SDL_QUIT: 
				_gameState = GameState::EXIT;
				break;
		}
	}
}

void MainGame::drawGame()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_sprite.draw();

	SDL_GL_SwapWindow(_window);
}
