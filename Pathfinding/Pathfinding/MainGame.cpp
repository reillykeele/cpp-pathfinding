#include "MainGame.h"

#include <iostream>

#include <SDLEngine/Sprite.h>

#include "SDLEngine/ResourceManager.h"
#include "SDLEngine/SDLEngine.h"

MainGame::MainGame() :
	_screenWidth(600),
	_screenHeight(400),
	_gameState(GameState::PLAY),
	_fps(0),
	_maxFPS(60.0f),
	_time(0)
{
	_camera.init(_screenWidth, _screenHeight);
}

MainGame::~MainGame() = default;

void MainGame::run()
{
	initSystems();

	gameLoop();
}

void MainGame::initSystems()
{
	SDLEngine::init();

	_window.create("Pathfinding", _screenWidth, _screenHeight, 0);

	initShaders();

	_fpsLimiter.init(_maxFPS);
	_spriteBatch.init();
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
		_fpsLimiter.begin();

		processInput();

		_time += 0.01f;

		_camera.update();

		drawGame();

		_fps = _fpsLimiter.end();

		// Print the FPS every 10 frames.
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 1000)
		{
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}

	}
}

void MainGame::processInput()
{
	SDL_Event ev;

	static const float CAMERA_SPEED = 2.0f;
	static const float SCALE_SPEED = 0.1f;

	while(SDL_PollEvent(&ev))
	{
		switch(ev.type)
		{
			case SDL_QUIT: 
				_gameState = GameState::EXIT;
				break;

			case SDL_KEYDOWN:
				_inputManager.pressKey(ev.key.keysym.sym);
				break;
			case SDL_KEYUP:
				_inputManager.releaseKey(ev.key.keysym.sym);
				break;

			case SDL_MOUSEMOTION:
				_inputManager.setMouseCoords(ev.motion.x, ev.motion.y);
			case SDL_MOUSEBUTTONDOWN:
				_inputManager.pressKey(ev.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				_inputManager.releaseKey(ev.button.button);
				break;
		}
	}
	
	if (_inputManager.isKeyPressed(SDLK_w))
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));

	if (_inputManager.isKeyPressed(SDLK_s))
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));

	if (_inputManager.isKeyPressed(SDLK_d))
		_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));

	if (_inputManager.isKeyPressed(SDLK_a))
		_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));

	if (_inputManager.isKeyPressed(SDLK_e))
		_camera.setScale(_camera.getScale() + SCALE_SPEED);

	if (_inputManager.isKeyPressed(SDLK_q))
		_camera.setScale(_camera.getScale() - SCALE_SPEED);

	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT))
	{
		auto mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = _camera.convertScreenToWorld(mouseCoords);
		std::cout << mouseCoords.x << " " << mouseCoords.y << std::endl;
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

	// Set the camera matrix
	const auto pLocation = _colorProgram.getUniformLocation("P");
	auto cameraMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_spriteBatch.begin();

	glm::vec4 pos(0.0f, 0.0f, 100.0f, 100.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	auto texture = SDLEngine::ResourceManager::getTexture("Textures/cat-pack/cat-icon.png");
	SDLEngine::Color color { color.r = 255, color.g = 255, color.b = 255, color.a = 255 };

	_spriteBatch.draw(pos, uv, texture.id, 0.0f, color);

	_spriteBatch.end();

	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);

	_colorProgram.unuse();

	_window.swap();
}
