#include "MainGame.h"
#include "Errors.h"

#include <iostream>
#include <string>

MainGame::MainGame() :
	_window(nullptr),
	_screenWidth(600),
	_screenHeight(400),
	_gameState(GameState::PLAY),
	_time(0)
{
	
}

MainGame::~MainGame() = default;

void MainGame::run()
{
	initSystems();

	_sprites.push_back(new Sprite());
	_sprites.back()->init(-1.0f, -1.0f, 1.0f, 1.0f, "Textures/cat-pack/cat-icon.png");

	_sprites.push_back(new Sprite());
	_sprites.back()->init(0.0f, 0.0f, 1.0f, 1.0f, "Textures/cat-pack/cat-icon.png");


	// _sprite.init(-1.0f, -1.0f, 2.0f, 2.0f, "Textures/cat-pack/cat-icon.png");

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

	initShaders();
}

void MainGame::initShaders()
{
	_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertPosition");
	_colorProgram.addAttribute("vertColor");
	_colorProgram.addAttribute("vertUV");
	_colorProgram.linkShaders();
}

void MainGame::gameLoop()
{
	while(_gameState != GameState::EXIT)
	{
		processInput();

		_time += 0.01f;

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

	_colorProgram.use();

	glActiveTexture(GL_TEXTURE0);

	const auto textureLocation = _colorProgram.getUniformLocation("sampler");
	glUniform1i(textureLocation, 0);

	const auto timeLocation = _colorProgram.getUniformLocation("time");
	glUniform1f(timeLocation, _time);

	for (int i = 0; i < _sprites.size(); i++)
	{
		_sprites[i]->draw();
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	_colorProgram.unuse();

	SDL_GL_SwapWindow(_window);
}
