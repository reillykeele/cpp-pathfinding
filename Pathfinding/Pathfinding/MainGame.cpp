#include "MainGame.h"

#include <iostream>

#include <SDLEngine/Sprite.h>
#include "SDLEngine/SDLEngine.h"

MainGame::MainGame() :
	_screenWidth(600),
	_screenHeight(400),
	_gameState(GameState::PLAY),
	_fps(0),
	_maxFPS(60.0f),
	_frameTime(0),
	_time(0)
{
}

MainGame::~MainGame() = default;

void MainGame::run()
{
	initSystems();

	_sprites.push_back(new SDLEngine::Sprite());
	_sprites.back()->init(-1.0f, -1.0f, 1.0f, 1.0f, "Textures/cat-pack/cat-icon.png");

	_sprites.push_back(new SDLEngine::Sprite());
	_sprites.back()->init(0.0f, 0.0f, 1.0f, 1.0f, "Textures/cat-pack/cat-icon.png");

	gameLoop();
}

void MainGame::initSystems()
{
	SDLEngine::init();

	_window.create("Pathfinding", _screenWidth, _screenHeight, 0);

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
		float startTicks = SDL_GetTicks64();

		processInput();

		_time += 0.01f;

		drawGame();

		calculateFPS();
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10)
		{
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}

		float frameTicks = SDL_GetTicks64() - startTicks;
		if (1000.0f / _maxFPS > frameTicks)
		{
			SDL_Delay(1000.0f / _maxFPS - frameTicks);
		}

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

	_window.swap();
}

void MainGame::calculateFPS()
{
	static const int NUM_SAMPLES = 10;
	static float frameTimes[NUM_SAMPLES];
	static int currFrame = 0;

	static float prevTicks = SDL_GetTicks64();

	float currTicks;
	currTicks = SDL_GetTicks64();

	_frameTime = currTicks - prevTicks;
	frameTimes[currFrame % NUM_SAMPLES] = _frameTime;

	prevTicks = currTicks;

	currFrame++;
	int count = std::min(currFrame, NUM_SAMPLES);

	float frameTimeAverage = 0;
	for (int i = 0; i < count; i++)
		frameTimeAverage += frameTimes[i];

	frameTimeAverage /= count;

	if (frameTimeAverage > 0)
		_fps = 1000.0f / frameTimeAverage;
	else
		_fps = 60.0f;

	
}
